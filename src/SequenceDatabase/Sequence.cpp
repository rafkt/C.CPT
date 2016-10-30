#include "../../include/Sequence.h"
#include <stdio.h>
#include <iostream>

using namespace std;

Sequence::Sequence() : _size(0){}

Sequence::Sequence(vector<uint64_t> itemList) : _size(itemList.size()){
	sequenceItems = new uint64_t[_size];
	for(int i = 0; i < itemList.size(); i++){
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

// int main(){
// 	vector<uint64_t> seq = {1, 2, 3, 4, 5};
// 	Sequence s(seq);
// 	s.print();
// 	return 0;
// }