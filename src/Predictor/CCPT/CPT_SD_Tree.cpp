#include "../../../include/CPT_SD_Tree.h"
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

bool cmpNodes (PredictionTree* i, PredictionTree* j) { return (i->getItem() < j->getItem());}

CPT_SD_Tree::CPT_SD_Tree(PredictionTree* trie){
//good idea to have a node number attribute in every node of trie;
//no need to set up this in advance since the tree will be parced here in Level order - so we can assign the attribute here. Alternatively along with the k-ary bitstring create a vector of PredictionTree*. This will not affect the current memory of CPT_Trie.
//Do next - print a tries nodes in Level order - create main
	levelOrderTraverse(trie);
}
CPT_SD_Tree::~CPT_SD_Tree(){

}

void CPT_SD_Tree::levelOrderTraverse(PredictionTree* root){
	if (!root) return;
	queue<PredictionTree*> nodesQueue;
	int nodesInCurrentLevel = 1;
	int nodesInNextLevel = 0;
	nodesQueue.push(root);
	while (!nodesQueue.empty()) {
		PredictionTree *currNode = nodesQueue.front();
		nodesQueue.pop();
		nodesInCurrentLevel--;
		if (currNode) {
			cout << currNode->getItem() << " ";
			sort(currNode->getChildren().begin(), currNode->getChildren().end(), cmpNodes);
			for (uint64_t i = 0; i < currNode->getChildren().size(); i++)
				nodesQueue.push(currNode->getChildren()[i]);
			nodesInNextLevel += currNode->getChildren().size();
		}
		if (nodesInCurrentLevel == 0) {
			cout << endl;
			nodesInCurrentLevel = nodesInNextLevel;
			nodesInNextLevel = 0;
		}
	}

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