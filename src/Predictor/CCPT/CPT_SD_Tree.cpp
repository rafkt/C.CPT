#include "../../../include/CPT_SD_Tree.h"
#include <iostream>
#include <vector>

using namespace std;

CPT_SD_Tree::CPT_SD_Tree(PredictionTree* trie, PredictionTree** LT, uint64_t nodeNumber){
//good idea to have a node number attribute in every node of trie;
//no need to set up this in advance since the tree will be parced here in Level order - so we can assign the attribute here. Alternatively along with the k-ary bitstring create a vector of PredictionTree*. This will not affect the current memory of CPT_Trie.
//Do next - print a tries nodes in Level order - create main
}
CPT_SD_Tree::~CPT_SD_Tree(){

}
PredictionTree* CPT_SD_Tree::getParent(){
	cerr << "CPT_SD_TREE (getParent): Operation Not supported" << endl;
	return nullptr;
}
vector<uint64_t> CPT_SD_Tree::getNodesToRoot(){
	
	vector<uint64_t> v;
	return v;
}
void CPT_SD_Tree::addChild(uint64_t item){
	cerr << "CPT_SD_TREE (addChild): Operation Not supported" << endl;	
}
bool CPT_SD_Tree::hasChild(uint64_t item){
	cerr << "CPT_SD_TREE (hasChild): Operation Not supported" << endl;
	return false;
}
PredictionTree* CPT_SD_Tree::getChild(uint64_t item){
	cerr << "CPT_SD_TREE (getChild): Operation Not supported" << endl;
	return nullptr;
}
uint64_t CPT_SD_Tree::getItem(){
	cerr << "CPT_SD_TREE (getItem): Operation Not supported" << endl;
	return 0;
}