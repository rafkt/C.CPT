#include "../../include/Sequence.h"
#include <stdio.h>
#include <iostream>

using namespace std;

Sequence::Sequence() : _size(0), sequenceItems(nullptr){}

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
uint64_t* Sequence::getItems() const{
	return sequenceItems;
}
uint64_t Sequence::size() const{
 return _size;
}
void Sequence::operator = (const Sequence& S){
	if (this != &S){
		if (sequenceItems) delete[] this->sequenceItems;
		this->_size = S.size();
		uint64_t* s_items = S.getItems();
		this->sequenceItems = new uint64_t[this->_size];
		for (uint64_t i = 0; i < S.size(); i++) this->sequenceItems[i] = s_items[i];
	}
}
std::ostream& operator<<(std::ostream &strm, const Sequence &seq) {
	for(uint64_t i = 0; i < seq._size; i++){
		if (i < seq._size - 1) strm << seq.sequenceItems[i] << " ";
		else strm << seq.sequenceItems[i];
	}
	return strm;
}

// int main(){
// 	vector<uint64_t> seq = {1, 2, 3, 4, 5};
// 	Sequence s(seq);
// 	s.print();
// 	return 0;
// }