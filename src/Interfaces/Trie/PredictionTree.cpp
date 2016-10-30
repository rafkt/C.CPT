#include "../../../include/PredictionTree.h"

using namespace std;

PredictionTree::PredictionTree(){

}
PredictionTree::~PredictionTree(){
	
}
PredictionTree::PredictionTree(uint64_t item){

}
void PredictionTree::addChild(uint64_t item){

}
bool PredictionTree::hasChild(uint64_t item){
	return false;
}
PredictionTree* PredictionTree::getChild(uint64_t item){
	return nullptr;
}
uint64_t PredictionTree::getSupport(){
	return 0;
}
uint64_t PredictionTree::getItem(){
	return 0;
}
PredictionTree* PredictionTree::getParrent(){
	return nullptr;
}
vector<PredictionTree*> PredictionTree::getChildren(){
	vector<PredictionTree*> v;
	v.push_back(nullptr);
	return v;
}