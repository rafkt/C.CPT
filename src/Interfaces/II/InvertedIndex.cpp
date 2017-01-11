#include "../../../include/InvertedIndex.h"
#include <iostream>
#include <stdio.h>
#include <algorithm>
using namespace std;

InvertedIndex::InvertedIndex(vector<Sequence*> sqDB) : _size(sqDB.size()), sequenceNumber(sqDB.size()), Sigma(0){
	II_database = new Sequence*[_size];
	for (uint64_t i = 0; i < _size; i++){
		II_database[i] = sqDB[i];
	}
	for (uint64_t i = 0; i < _size; i++) {
		Sequence* seq = II_database[i];
		uint64_t* seq_items = seq->getItems();
		for (uint64_t j = 0; j < seq->size(); j++){
			unordered_map<uint64_t, vector<uint64_t>>::iterator it = alphabet2sequences_table.find(seq_items[j]);
			if (it != alphabet2sequences_table.end()){
				if (find(it->second.begin(), it->second.end(), i) == it->second.end()) it->second.push_back(i);
			}else{
				vector<uint64_t> sequencesAppears;
				sequencesAppears.push_back(i);
				alphabet2sequences_table.insert({seq_items[j], sequencesAppears});
				Sigma++;
			}
		}
	}
	cout << "Sigma: " << Sigma << endl;
	cout << "Sequnce Number: " << sequenceNumber << endl;
}

uint64_t InvertedIndex::getSigma(){return Sigma;}
uint64_t InvertedIndex::getSequenceNumber(){return sequenceNumber;}
float InvertedIndex::memoryInMB(){return (8 + Sigma * (8 + 8 * ceil(sequenceNumber/64.0)) + 8 + Sigma * 8) * 8 * 1.25 * pow(10, -7);}

InvertedIndex::~InvertedIndex(){
	//Sequence pointers should be handled by the evaluator
	// for (uint64_t i = 0; i < _size; i++){
	// 	delete II_database[i];
	// }

	//II_database will be deleted by the corresponding
	//II implementation
	//delete[] II_database;
}


// int main(){
// 	vector<uint64_t> seq1 = {1, 2, 3, 4, 5};
// 	Sequence* s1 = new Sequence(seq1);

// 	vector<uint64_t> seq2 = {6, 7, 8, 9};
// 	Sequence* s2 = new Sequence(seq2);


// 	vector<uint64_t> seq3 = {10, 11, 12};
// 	Sequence* s3 = new Sequence(seq3);

// 	std::vector<Sequence*> v = {s1, s2, s3};

// 	InvertedIndex* II = new InvertedIndex(v);


// 	return 0;
// }
