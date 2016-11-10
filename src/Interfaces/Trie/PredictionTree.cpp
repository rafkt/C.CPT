#include "../../../include/PredictionTree.h"

using namespace std;

PredictionTree::PredictionTree() : Support(0), item(0){
}
PredictionTree::~PredictionTree(){
	
}
PredictionTree::PredictionTree(uint64_t item) : Support(0), item(item){
}
void PredictionTree::addChild(uint64_t item){
	PredictionTree* child = new PredictionTree(item);
	children.push_back(child);
	child->parent = this;
}
bool PredictionTree::hasChild(uint64_t item){
	for (uint64_t i = 0; i < children.size(); i ++)
		if (children[i]->getItem() == item)return true;
	return false;
}
PredictionTree* PredictionTree::getChild(uint64_t item){
	for (uint64_t i = 0; i < children.size(); i ++)
		if (children[i]->getItem() == item) return children[i];
	return nullptr;
}
uint64_t PredictionTree::getSupport(){
	return Support;
}
uint64_t PredictionTree::getItem(){
	return item;
}
PredictionTree* PredictionTree::getParent(){
	return parent;
}
vector<PredictionTree*> PredictionTree::getChildren(){
	return children;
}