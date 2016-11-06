#include "../../../include/CPTPredictor.h"
#include "../../../include/CPT_Trie.h"
#include "../../../include/II_bit_vector.h"
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

}
Sequence* CPTPredictor::getBestSequenceFromCountTable(std::unordered_map<uint64_t, float> countTable, bool useLift){
	return nullptr;
}

static void CPTPredictor::RecursiveDivider(std::vector<Sequence*> result, Sequence* target, uint64_t minsize){
	uint64_t size = target->size();

	result.add(target); //adding the resulting sequence to the result list

	//if the target is small enough or already too small
	if(size <= minSize) {
		return;
	}

	//Hiding one item at the time from the target
	for(int toHide = 0; toHide < size; toHide++) {
		
		//Constructing a new sequence from the target without the "toHide" item
		Sequence newSequence = new Sequence(-1);
		for(int toUse = 0 ; toUse < size; toUse++) {
			
			if(toUse != toHide) {
				newSequence.addItem(target.get(toUse));
			}
		}
		
		RecursiveDivider(result, newSequence, minSize);
	}
}
static std::vector<Sequence*> CPTPredictor::sliceBasic(Sequence* sequence, huint64_t lengt){
	std::vector<Sequence*> v;
	v.push_back(nullptr);
	return v;
}
static std::vector<Sequence*> CPTPredictor::slice(Sequence* sequence, uint64_t length){
	std::vector<Sequence*> v;
	v.push_back(nullptr);
	return v;
}
