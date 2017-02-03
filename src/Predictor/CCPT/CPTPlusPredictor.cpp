#include "../../../include/CPTPlusPredictor.h"
#include <vector>
#include <iostream>

#include "../../../include/DatabaseHelper.h"// comment this in along with main after debugging


using namespace std;


CPTPlusPredictor::CPTPlusPredictor(vector<Sequence*> trainingSequences, Profile* pf) : CPTPredictor(){
	profile = pf;
	trainingSequenceNumber= trainingSequences.size();
	nodeNumber = 1;

	TAG = "CPT+";
	root = new CPT_Trie();
	LT = new PredictionTree*[trainingSequences.size()];
	encoder = new Encoder();
	Train(trainingSequences);
	cout << "Trie node number: " << nodeNumber << endl;
}
CPTPlusPredictor::~CPTPlusPredictor(){
	delete encoder;
}
float CPTPlusPredictor::memoryInMB(){
	return 0.0;
}

bool CPTPlusPredictor::Train(std::vector<Sequence*> trainingSequences){
	FIFRaw finder;
	if(profile->paramBool("CCF")){
		std::vector<std::vector<uint64_t>> itemsets = finder.findFrequentItemsets(trainingSequences, profile->paramInt("CCFmin"), profile->paramInt("CCFmax"), profile->paramInt("CCFsup"));
		for(std::vector<uint64_t> i : itemsets) encoder->addEntry(i);
	}

	uint64_t seqId = 0;
		
	// //Slicing sequences, so no sequence has a length > maxTreeHeight
	for(uint64_t i = 0; i < trainingSequences.size(); i++) {
		
		if(trainingSequences[i]->size() > profile->paramInt("splitLength") && profile->paramInt("splitMethod") > 0) {
			if(profile->paramInt("splitMethod") == 1)
				newTrainingSet.push_back(sliceBasic(trainingSequences[i], profile->paramInt("splitLength")));
			else
				newTrainingSet.push_back(slice(trainingSequences[i], profile->paramInt("splitLength")));
		}else{
			Sequence* tmp = new Sequence();
			*tmp = *trainingSequences[i];
			newTrainingSet.push_back(tmp);
		}		
	}

	#ifdef SD_CONSTRACTION
		sigmaIndex(newTrainingSet);
	#endif
	
	vector<Sequence*> encodedSeqs;
	
	// //For each line (sequence) in file
	for(uint64_t i = 0; i < newTrainingSet.size(); i++) {
		
		PredictionTree* curNode = root;
		
		//for each item in this sequence
		//encode sequence and continue like CPT building trie
		Sequence* encoded = encoder->encode(newTrainingSet[i]);
		encodedSeqs.push_back(encoded);

		// Sequence* decoded_tmp = encoder->decode(encoded);

		// for (uint64_t d = 0; d < decoded_tmp->size(); d++)
		// 	if (decoded_tmp->getItems()[d] != newTrainingSet[i]->getItems()[d]) cout << "ERROR" << endl;
		// if (decoded_tmp->size() != newTrainingSet[i]->size()) cout << "SIZE ERROR" << endl;

		//end of encoding, continue as normal

		uint64_t* cur_seq_items = encoded->getItems();//newTrainingSet[i]->getItems();

		for(uint64_t j = 0; j < encoded->size(); j++) {
			
			//if item is not in compact tree then we add it
			if(curNode->hasChild(cur_seq_items[j]) == false) {
				curNode->addChild(cur_seq_items[j]);
				nodeNumber++;
			}
			curNode = curNode->getChild(cur_seq_items[j]);
		}
		
		LT[seqId++] = curNode; //adding <sequence id, last node in sequence>
		 //increment sequence id number
	}

	//pathColapse to be implemented.

	for(Sequence* seq : encodedSeqs) delete seq;

	//after train delete the encoder staff that you dont need anymore
	//delete FIF after train ->> no need

	//Patch collapsing for added compression
	if(profile->paramBool("CBS")) {
		pathCollapse();
	}

	return true;
}
std::vector<uint64_t> CPTPlusPredictor::getBranch(uint64_t index){
	PredictionTree* curNode = LT[index];
	vector<uint64_t> branch;
	while(curNode != root) {
		
		branch.push_back(curNode->getItem());
		
		//Going up the tree
		curNode = curNode->getParent();
	}
	reverse(branch.begin(), branch.end()); //implemented in Encoder
	Sequence* encoded = new Sequence(branch);
	Sequence* decoded = encoder->decode(encoded);
	vector<uint64_t> decoded_items(decoded->getItems(), decoded->getItems() + decoded->size());
	delete encoded;
	// for (uint64_t i : decoded_items) cout << i << " ";
	// cout << endl;
	return decoded_items;
}

/**
 * This compression method can be called on a compressed prediction tree
 * to replace direct branch with a single node.
 * 
 * Once one of these branch has been found, each node a decoded and concatenated
 * to form a single itemset which is then inserted in the encoder and used to
 * replace the branch with a single node. 
 * 
 * As an optimization, the leaf of the branch is the node used to replace the branch,
 * so the Lookup Table for CPT does not have to be updated since it is already pointing
 * to this node.
 */
void CPTPlusPredictor::pathCollapse() {
	
	uint64_t nodeSaved = 0;
	
	//for each sequences registered in the Lookup Table (LT)
	for(uint64_t i = 0; i < trainingSequenceNumber; i++) {
		
		PredictionTree* cur = LT[i];
		PredictionTree* leaf = cur;
		PredictionTree* last = nullptr;
		vector<uint64_t> itemset;
		uint64_t pathLength = 0;
		bool singlePath = true;
		
		//if this cur is a true leaf
		if(cur->getChildren().size() == 0) {
			
			//while the path is singular (starting from the leaf)
			while(singlePath == true) {
				
				//if the current node has multiple children
				if(cur->getChildren().size() > 1 || cur == nullptr) {
					
					if(pathLength != 1) {
						//updating the leaf to be a child of cur
						uint64_t newId = encoder->getIdorAdd(itemset);
						leaf->item = newId;
						leaf->parent = cur;
						
						//updating cur to have the leaf has a child
						uint64_t tmp_size = cur->getChildren().size();
						cur->removeChild(last->item);
						if (tmp_size != cur->getChildren().size() + 1) cout << "ERROR" << endl;
						cur->addChild(leaf);
						if (tmp_size != cur->getChildren().size()) cout << "ERROR" << endl;
						
						//saving the number of node saved
						nodeSaved += pathLength - 1;
					}
					singlePath = false;
				}
				//this node has only one child and so it is added to the itemset 
				else {
					vector<uint64_t> curItemset = encoder->getEntry(cur->item);
					
					vector<uint64_t> tmp(itemset);
					vector<uint64_t> itemset_tmp;
					itemset_tmp.insert(itemset_tmp.end(), curItemset.begin(), curItemset.end());
					itemset_tmp.insert(itemset_tmp.end(), tmp.begin(), tmp.end());
					itemset = itemset_tmp;
					
					cur->clearAllLeafs();
					
					pathLength++;
					
					last = cur;
					cur = cur->parent;
				}
			}			
		}
	}
	
	nodeNumber -= nodeSaved;
	cout << "(PathCollpase) Nodes: " << nodeNumber;
}

int main(){
	Profile* pf = new Profile();
	pf->apply();
	DatabaseHelper* db = new DatabaseHelper("BIBLE.txt", DatabaseHelper::TXT, pf);
	CPTPredictor* cpt_pr = new CPTPlusPredictor(db->getDatabase(), pf);
	cpt_pr->createII();


	vector<uint64_t> v = {356, 122};
	Sequence* target = new Sequence(v);
	Sequence* predicted = cpt_pr->Predict(target);
	cout << "Prtedicted: " << endl;
	predicted->print();
	cout << endl;
	cout << "Memory size of Predictor: " << cpt_pr->memoryInMB() << "MB";

	delete cpt_pr;
	delete pf;
	delete db;
	delete target;
	delete predicted;
}