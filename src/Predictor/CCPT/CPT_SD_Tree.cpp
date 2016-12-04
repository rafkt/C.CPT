#include "../../../include/CPT_SD_Tree.h"

using namespace std;

CPT_SD_Tree::CPT_SD_Tree() : PredictionTree(){

}
CPT_SD_Tree::CPT_SD_Tree(uint64_t item) : PredictionTree(item){

}
CPT_SD_Tree::~CPT_SD_Tree(){

}
PredictionTree* CPT_SD_Tree::getParent(){
	return nullptr;
}

void CPT_SD_Tree::addChild(uint64_t item){
	
}
bool CPT_SD_Tree::hasChild(uint64_t item){
	return false;
}
PredictionTree* CPT_SD_Tree::getChild(uint64_t item){
	return nullptr;
}
uint64_t CPT_SD_Tree::getItem(){
	return 0;
}