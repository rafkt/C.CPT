#include "../../include/Sequence.h"
#include <stdio.h>
#include <iostream>

using namespace std;

Sequence::Sequence() : _size(0){}

Sequence::Sequence(vector<uint64_t> itemList) : _size(itemList.size()){
	sequenceItems = new uint64_t[_size];
	for(uint64_t i = 0; i < itemList.size(); i++){
		sequenceItems[i] = itemList[i];
	}
}
Sequence::~Sequence(){
	delete[] sequenceItems;
}
void Sequence::print(){
	for(uint64_t i = 0; i < _size; i++){
		cout << sequenceItems[i] << " ";
	}
	cout << endl;
}
uint64_t* Sequence::getItems(){
	return sequenceItems;
}
uint64_t Sequence::size(){
 return _size;
}
void operator = (const Sequence& S){
	if (this != &S){
		delete[] this->sequenceItems;
		this->_size = s.size();
		uint64_t* s_items = s.getItems();
		this->sequenceItems = new uint64_t[this->_size];
		for (uint64_t i = 0; i < S.size(); i++) this->sequenceItems[i] = s_items[i];
	}
}

// int main(){
// 	vector<uint64_t> seq = {1, 2, 3, 4, 5};
// 	Sequence s(seq);
// 	s.print();
// 	return 0;
// }