#include "../../../include/PredictionTree.h"
#include <iostream>

using namespace std;

PredictionTree::PredictionTree() : Support(0), item(0), parent(nullptr){
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
void PredictionTree::addChild(PredictionTree* child){
	children.push_back(child);
	child->parent = this;
}
void PredictionTree::clearAllLeafs(){
	//for (PredictionTree* nd : children) delete nd;
	children.clear();
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
uint64_t PredictionTree::getItem() const{
	return item;
}
PredictionTree* PredictionTree::getParent(){
	return parent;
}
vector<PredictionTree*>& PredictionTree::getChildren(){
	return children;
}
bool PredictionTree::removeChild(uint64_t item){
	for (uint64_t nd = 0; nd < children.size(); nd++){
		if (children[nd]->item == item){
			if (children[nd]->children.size() > 0){
				for (uint64_t nd_ch = 0; nd_ch < children[nd]->children.size(); nd_ch++)
					children[nd]->children[nd_ch]->parent = this;
				children.insert(children.end(), children[nd]->children.begin(), children[nd]->children.end());
			}
			delete children[nd];
			children.erase(children.begin() + nd);
			return true;
		}
	}
	return false;
}

bool PredictionTree::operator < (const PredictionTree& node) const{
        return (item < node.item);
}