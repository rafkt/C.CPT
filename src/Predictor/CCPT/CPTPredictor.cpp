#include "../../../include/CPTPredictor.h"

CPTPredictor::CPTPredictor() : Predictor(){

}
CPTPredictor::~CPTPredictor(){

}
bool CPTPredictor::Train(std::vector<Sequence*> trainingSequences){
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
