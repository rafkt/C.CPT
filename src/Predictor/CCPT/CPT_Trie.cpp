#include "../../../include/CPT_Trie.h"
#include "../../../include/II_bit_vector.h"
#include "../../../include/CPTPredictor.h"
#include <vector>

using namespace std;

CPT_Trie::CPT_Trie() : PredictionTree(){

}
CPT_Trie::CPT_Trie(uint64_t item) : PredictionTree(item){

}
CPT_Trie::~CPT_Trie(){

}
PredictionTree* CPT_Trie::getParent(){
	return PredictionTree::getParent();
}

void CPT_Trie::addChild(uint64_t item){
	PredictionTree::addChild(item);
}
bool CPT_Trie::hasChild(uint64_t item){
	return PredictionTree::hasChild(item);
}
PredictionTree* CPT_Trie::getChild(uint64_t item){
	return PredictionTree::getChild(item);
}
uint64_t CPT_Trie::getItem(){
	return PredictionTree::getItem();
}

int main(){

	vector<uint64_t> seq1 = {1, 2, 3, 4, 5};
	Sequence* s1 = new Sequence(seq1);

	vector<uint64_t> seq2 = {6, 7, 8, 1};
	Sequence* s2 = new Sequence(seq2);


	vector<uint64_t> seq3 = {10, 1, 12};
	Sequence* s3 = new Sequence(seq3);

	std::vector<Sequence*> v = {s1, s2, s3};

	InvertedIndex* II = new II_bit_vector(v);

	delete II;
	delete s1;
	delete s2;
	delete s3;

	return 0;
}