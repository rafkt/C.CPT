#include "../../../include/CPTPredictor.h"
#include "../../../include/CPT_Trie.h"
#include "../../../include/II_bit_vector.h"
#include "../../../include/DatabaseHelper.h"
#include <set>
#include <iostream>
#include <math.h>
#include <algorithm>
using namespace std;

CPTPredictor::CPTPredictor(): Predictor(), root(nullptr), LT(nullptr){
}

CPTPredictor::CPTPredictor(vector<Sequence*> trainingSequences, Profile* profile) : Predictor(), profile(profile), trainingSequenceNumber(trainingSequences.size()), nodeNumber(1){
	TAG = "CPT";
	root = new CPT_Trie();
	LT = new PredictionTree*[trainingSequences.size()];
	Train(trainingSequences);
	cout << "Trie node number: " << nodeNumber << endl;
}
CPTPredictor::~CPTPredictor(){
	//delete the CPT_Trie
	if (root) deleteTrie(root);
	//delete II_bit_vector
	if (II) delete II;
	//delete LT
	if  (LT) delete[] LT;
}

float CPTPredictor::memoryInMB(){
	return (8 + 8 * trainingSequenceNumber + 8 + nodeNumber * 32 + (nodeNumber - 1) * 8) * 8 * 1.25 * pow(10, -7) + II->memoryInMB();
}

void CPTPredictor::deleteTrie(PredictionTree* node){
    // recursive case (go to end of trie)
    for (uint64_t i = 0; i < node->getChildren().size(); i++)
    {
        if (node->getChildren()[i]->getChildren().size() > 0)
        {
            deleteTrie(node->getChildren()[i]);
        }else{
	   		delete node->getChildren()[i];
		}
    }

    // base case
    delete node; node = nullptr;

}

void CPTPredictor::sigmaIndex(vector<Sequence*> trainingSet){
	for (Sequence* s : trainingSet){
		uint64_t* items = s->getItems();
		for (uint64_t i = 0; i < s->size(); i++) {
			mapSigmaIndex.insert({items[i], 0});
			if (items[i] == 0){ cerr << "Input error: 0/-1 are not a valid aplhabet items."; exit(1);}
		}
	}
	uint64_t counter = 0;
	for (map<uint64_t, uint64_t>::iterator it = mapSigmaIndex.begin(); it != mapSigmaIndex.end(); it++) it->second = counter++;
}

void CPTPredictor::createII(){
	II = new II_bit_vector(newTrainingSet);
	//delete newTrainingSet since it is not longer needed
	for(uint64_t i = 0; i < newTrainingSet.size(); i++) delete newTrainingSet[i];

	cout << "Total CPT RAW size in megabytes: " << memoryInMB() << endl;
	cout << "-----------------------------------" << endl;
	cout << "II size in megabytes: " << II->memoryInMB() << endl;;
	cout << "Trie size in megabytes: " << ((8 + nodeNumber * 32 + (nodeNumber - 1) * 8) * 8 * 1.25 * pow(10, -7)) << endl;
	cout << "LT size in megabytes: " << ((8 + 8 * trainingSequenceNumber) * 8 * 1.25 * pow(10, -7)) << endl;
}

bool CPTPredictor::Train(std::vector<Sequence*> trainingSequences){
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
	
	
	// //For each line (sequence) in file
	for(uint64_t i = 0; i < newTrainingSet.size(); i++) {
		
		PredictionTree* curNode = root;
		
		//for each item in this sequence
		uint64_t* cur_seq_items = newTrainingSet[i]->getItems();
		for(uint64_t j = 0; j < newTrainingSet[i]->size(); j++) {
			
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
	return true;
}
Sequence* CPTPredictor::Predict(Sequence* target){
	//remove items that were never seen before from the Target sequence before LLCT try to make a prediction
	uint64_t* target_items = target->getItems();
	vector<uint64_t> cleared_target;
	for(uint64_t i = 0; i < target->size(); i++) {
		// if there is no bitset for that item (we have never seen it)
		if(II->itemIsValidAlphabet(target_items[i])){
			// then added to the cleared target.
			cleared_target.push_back(target_items[i]);  
		}
	}

	
	Sequence* prediction = new Sequence();
	uint8_t minRecursion = profile->paramInt("recursiveDividerMin");
	uint8_t maxRecursion = (profile->paramInt("recursiveDividerMax") > cleared_target.size()) ? cleared_target.size() : profile->paramInt("recursiveDividerMax");

	
	for(uint64_t i = minRecursion ; i < cleared_target.size() && prediction->size() == 0 && i < maxRecursion; i++) {
	
		set<uint64_t> hashSidVisited;  // PFV

		Sequence* cleared_target_seq = new Sequence(cleared_target);
		
		//TODO: use those as global parameters
		//TODO: int minSize = (target.size() > 3) ? target.size() - 3 : 1;
		//int minSize = (target.size() > 3) ? target.size() - 3 : 1;
		uint64_t minSize = cleared_target_seq->size() - i;
		bool useLift = false;
		
		//Dividing the target sequence into sub sequences
		vector<Sequence*> subSequences;
		RecursiveDivider(subSequences, cleared_target_seq, minSize);
		
		//For each subsequence, updating the CountTable
		unordered_map<uint64_t, float> countTable;
		for(uint64_t j = 0; j < subSequences.size(); j++) {
			
			//Setting up the weight multiplier for the countTable
			float weight = (float)subSequences[j]->size() / cleared_target_seq->size();
			
			UpdateCountTable(subSequences[j], weight, countTable, hashSidVisited);
		}

		for(uint64_t i = 0; i < subSequences.size(); i++) delete subSequences[i];
	
		//Getting the best sequence out of the CountTable
		Sequence* bstSequence = getBestSequenceFromCountTable(countTable, useLift);
		*prediction = *bstSequence;
		delete bstSequence;
	}

	return prediction;
}
uint64_t CPTPredictor::size(){
	return nodeNumber;
}
std::vector<uint64_t> CPTPredictor::getMatchingSequences(Sequence* target){
	//find all sequences that have all the target's items
	//for each item in the target sequence
	uint64_t* items = target->getItems();
	uint64_t* intersection = II->query(items, target->size());
	vector<uint64_t> indexes;

	for (uint64_t  i = 0; i < II->getSequenceNumber(); i ++)
		if ((intersection[(i) / 64] >> ((i) % 64)) & 1) indexes.push_back(i);
	delete[] intersection;
	return indexes;
}

vector<uint64_t> CPTPredictor::getBranch(uint64_t index){
	PredictionTree* curNode = LT[index];
	vector<uint64_t> branch;
	while(curNode != root) {
		
		branch.push_back(curNode->getItem());
		
		//Going up the tree
		curNode = curNode->getParent();
	}
	reverse(branch.begin(), branch.end());
	// for (uint64_t i : branch) cout << i << " ";
	// cout << endl;
	return branch;
}

bool CPTPredictor::visited(set<uint64_t>& hashSidVisited, uint64_t index){
	if(hashSidVisited.find(reinterpret_cast<uintptr_t>(LT[index])) != hashSidVisited.end()) return true;
	return false;
}
void CPTPredictor::gotVisit(set<uint64_t>& hashSidVisited, uint64_t index){
	hashSidVisited.insert(reinterpret_cast<uintptr_t>(LT[index]));
}

void CPTPredictor::UpdateCountTable(Sequence* target, float weight, std::unordered_map<uint64_t, float>& countTable, set<uint64_t>& hashSidVisited){
	vector<uint64_t> indexes = getMatchingSequences(target); 
		
	//creating an HashMap of the target's item (for O(1) search time)
	set<uint64_t> hashTarget;
	uint64_t* targetItems = target->getItems();
	for(uint64_t i = 0; i < target->size(); i ++) {
		hashTarget.insert(targetItems[i]);
	}
	
	
	//For each branch 
	for(uint64_t i = 0; i < indexes.size(); i++) {
		uint64_t index = indexes[i];

		if(visited(hashSidVisited, index)){
			continue;    
		}   
		
		//Getting the branch's leaf
		// PredictionTree* curNode = LT[index];
		
		// //Transform this branch in a list
		 vector<uint64_t> branch;
		// while(curNode->getParent() != root) {
			
		// 	branch.push_back(curNode->getItem());
			
		// 	//Going up the tree
		// 	curNode = curNode->getParent();
		// }

		branch = getBranch(index);


		set<uint64_t> hashTargetTMP(hashTarget);
		vector<uint64_t>::iterator it;
		for (it = branch.begin(); it != branch.end() && hashTargetTMP.size() > 0; it++) hashTargetTMP.erase(*it);

		for (; it != branch.end(); it++){
			float oldValue = 0;
			if(countTable.find(*it) != countTable.end()) {
				oldValue = countTable[*it];
			}

		// 	//Update the countable with the right weight and value
			float curValue = 1.0 /((float)indexes.size());
			if (!oldValue) countTable.insert({*it, oldValue + weight /((float)indexes.size())});
			else countTable[*it] = oldValue + weight /((float)indexes.size());
			
			gotVisit(hashSidVisited, index);
		}
	}


}
Sequence* CPTPredictor::getBestSequenceFromCountTable(std::unordered_map<uint64_t, float> countTable, bool useLift){

	//Looking for the item with the highest count in the CountTable
	double maxValue = -1;
	double secondMaxValue = -1;
	int maxItem = -1;
	for(unordered_map<uint64_t, float>::iterator it = countTable.begin(); it != countTable.end(); it++) {

		//cout << it->first << ": " << it->second << endl;

		uint64_t cardinality = II->getCardinality(it->first);
		double lift = it->second / cardinality;
		double support = cardinality;
		double confidence = it->second;
		
		double score = confidence; //Use confidence or lift, depending on Parameter.firstVote

		if(score > maxValue) {
			secondMaxValue = maxValue; //saving the old value as the second best
			maxItem = it->first; //saving the new best value
			maxValue = score;
		} 
		else if (score > secondMaxValue) {
			secondMaxValue = score; //updating the second best value
		}
	}
	
	vector<uint64_t> items;

	//Calculating the ratio between the best value and the second best value
	double diff = 1 - (secondMaxValue / maxValue);
	
	//No match
	if(maxItem == -1) {
		//Nothing to do
	} 
	//-If there is no second best value, then the best one is the winner
	//-If there is a max item (at least one item in the CountTable)
	// and it is better than second best according to the voteTreshold
	else if (secondMaxValue == -1 || diff >= 0.0) {
		items.push_back((uint64_t)maxItem);
	}
	//if both the best and the secondBest are "equal"
	else {
		//pick the one with the highest support or lift
		double highestScore = 0;
		int newBestItem = -1;
		for(unordered_map<uint64_t, float>::iterator it = countTable.begin(); it != countTable.end(); it++) {
			
			if(maxValue == it->second) {
				if(uint64_t card = II->getCardinality(it->first)) {
					
					double lift = it->second / card;
					
					double score = lift; //Use confidence or lift, depending on Parameter.secondVote
					
					if(score > highestScore) {
						highestScore = score;
						newBestItem = it->first;
					}
				}
			}
		}			
		items.push_back((uint64_t)newBestItem);
	}
	
	Sequence* predicted = new Sequence(items);	
	return predicted;
}

void CPTPredictor::RecursiveDivider(std::vector<Sequence*>& result, Sequence* target, uint64_t minsize){
	uint64_t size = target->size();

	result.push_back(target); //adding the resulting sequence to the result list

	//if the target is small enough or already too small
	if(size <= minsize) {
		return;
	}

	//Hiding one item at the time from the target
	for(uint64_t toHide = 0; toHide < size; toHide++) {
		
		//Constructing a new sequence from the target without the "toHide" item
		std::vector<uint64_t> v;
		uint64_t* targetItems = target->getItems();
		for(int toUse = 0 ; toUse < size; toUse++) {
			
			if(toUse != toHide) {
				v.push_back(targetItems[toUse]);
			}
		}
		Sequence* newSequence = new Sequence(v);
		RecursiveDivider(result, newSequence, minsize);
	}
}
Sequence* CPTPredictor::sliceBasic(Sequence* sequence,  uint64_t length){
	if(sequence->size() <= length) return sequence;

	vector<uint64_t> v;
	uint64_t* seq_items = sequence->getItems();
	for (uint64_t i = sequence->size() - length; i < sequence->size(); i++) v.push_back(seq_items[i]);
	Sequence* tmp_seq = new Sequence(v);
	//sequence = &tmp_seq;
	
	return tmp_seq;
}
Sequence* CPTPredictor::slice(Sequence* sequence, uint64_t length){
	cerr << "PLEASE USE sliceBasic - slice not yet implemented" << endl;

	return nullptr;
}




// int main(){
// 	Profile* pf = new Profile();
// 	pf->apply();
// 	DatabaseHelper* db = new DatabaseHelper("test.txt", DatabaseHelper::TXT, pf);
// 	CPTPredictor* cpt_pr = new CPTPredictor(db->getDatabase(), pf, db->getSigmaIndex());
// cpt_pr->createII();


// 	// vector<uint64_t> v = {356, 122};
// 	// Sequence* target = new Sequence(v);
// 	// Sequence* predicted = cpt_pr->Predict(target);
// 	// cout << "Prtedicted: " << endl;
// 	// predicted->print();
// 	// cout << endl;
// 	// cout << "Memory size of Predictor: " << cpt_pr->memoryInMB() << "MB";

// 	// delete cpt_pr;
// 	// delete pf;
// 	// delete db;
// 	// delete target;
// 	// delete predicted;
// }
