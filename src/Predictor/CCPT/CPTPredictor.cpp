#include "../../../include/CPTPredictor.h"
#include "../../../include/CPT_Trie.h"
#include "../../../include/II_bit_vector.h"
#include <set>
using namespace std;

CPTPredictor::CPTPredictor(vector<Sequence*> trainingSequences) : Predictor(){
	TAG = "CPT";
	root = new CPT_Trie();
	II = new II_bit_vector(trainingSequences);
	Train(trainingSequences);
	LT = new CPT_Trie*[trainingSequences.size()];
}
CPTPredictor::~CPTPredictor(){
	//delete the CPT_Trie
	//delete II_bit_vector
	//delete LT
	//delete vector with new sequences created from recursive divider method
	//delete the pointer created on getBestSequenceFromCountTable
}
bool CPTPredictor::Train(std::vector<Sequence*> trainingSequences){
	nodeNumber = 0;
	uint64_t seqId = 0;
	
	return false;
}
Sequence* CPTPredictor::Predict(Sequence* target){
	return nullptr;
}
uint64_t CPTPredictor::size(){
	return 0;
}
std::vector<uint64_t> CPTPredictor::getMatchingSequences(Sequence* target){
	std::vector<uint64_t> v;
	v.push_back(0);
	return v;
}
void CPTPredictor::UpdateCountTable(Sequence* target, float weight, std::unordered_map<uint64_t, float> countTable, std::set<uint64_t> hashSidVisited){
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

		if(hashSidVisited.find(index) != hashSidVisited.end()){
			continue;    
		}   
		
		//Getting the branch's leaf
		PredictionTree* curNode = LT[index];
		
		//Transform this branch in a list
		vector<uint64_t> branch;
		while(curNode->getParent() != root) {
			
			branch.push_back(curNode->getItem());
			
			//Going up the tree
			curNode = curNode->getParent();
		}
		
		set<uint64_t> hashTargetTMP(hashTarget);
		vector<uint64_t>::reverse_iterator r_it;
		for (r_it = branch.rbegin(); r_it != branch.rend() && hashTargetTMP.size() > 0; r_it++) hashTargetTMP.erase(*r_it);

		for (; r_it != branch.rend(); r_it++){
			float oldValue = 0;
			if(countTable.find(*r_it) != countTable.end()) {
				oldValue = countTable[*r_it];
			}

		// 	//Update the countable with the right weight and value
			float curValue = 1.0 /((float)indexes.size());
			countTable.insert({*r_it, oldValue + weight /((float)indexes.size())});
			
			hashSidVisited.insert(index);
		}
	}


}
Sequence* CPTPredictor::getBestSequenceFromCountTable(std::unordered_map<uint64_t, float> countTable, bool useLift){

	//Looking for the item with the highest count in the CountTable
	double maxValue = -1;
	double secondMaxValue = -1;
	int maxItem = -1;
	for(unordered_map<uint64_t, float>::iterator it = countTable.begin(); it != countTable.end(); it++) {
		
		double lift = it->second / II->getCardinality(it->first);
		double support = II->getCardinality(it->first);
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
vector<Sequence*> CPTPredictor::sliceBasic(Sequence* sequence,  uint64_t length){
	vector<Sequence*> cutted; //result
		
	if(sequence->size() <= length){ 
		cutted.push_back(sequence);
		return cutted; //nothing to do for this sequence
	}
	vector<uint64_t> v;
	uint64_t* seq_items = sequence->getItems();
	for (uint64_t i = sequence->size() - length; i < sequence->size(); i++) v.push_back(seq_items[i]);
	Sequence tmp_seq(v);
	sequence = tmp_seq;
	cutted.push_back(sequencee);
	
	return cutted;
}
vector<Sequence*> CPTPredictor::slice(Sequence* sequence, uint64_t length){
	cout << "PLEASE USE sliceBasic - slice not yet implemented" << endl;
	std::vector<Sequence*> v;
	v.push_back(nullptr);
	return v;
}
