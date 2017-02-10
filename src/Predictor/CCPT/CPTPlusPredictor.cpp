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
	CPTPredictor::~CPTPredictor();
	delete encoder;
}
float CPTPlusPredictor::memoryInMB(){
	return CPTPredictor::memoryInMB() + encoder->sizeInMB();
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

	encoder->clearInvDict();
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
	delete decoded;
	// for (uint64_t i : decoded_items) cout << i << " ";
	// cout << endl;
	return decoded_items;
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