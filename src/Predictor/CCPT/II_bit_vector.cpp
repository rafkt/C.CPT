#include "../../../include/II_bit_vector.h"
#include "../../../include/Sequence.h"
#include "../../../include/DatabaseHelper.h"
#include <iostream>

using namespace std;

II_bit_vector::II_bit_vector(std::vector<Sequence*> sq) : InvertedIndex(sq){
	uint64_t bv_size = ceil(_size / 64.0);
	uint64_t set_bit = 1;
	for (unordered_map<uint64_t, vector<uint64_t>>::iterator it = alphabet2sequences_table.begin(); it != alphabet2sequences_table.end(); it++){
		uint64_t* tmp_bv = new uint64_t[bv_size];
		for (uint64_t i = 0; i < bv_size; i++) tmp_bv[i] = 0;
		for (uint64_t i = 0; i < it->second.size(); i++) tmp_bv[it->second[i] / 64] |= set_bit << (it->second[i] % 64);
		bit_vectors_table.insert({it->first, tmp_bv});
	}
	delete[] II_database;
}

II_bit_vector::~II_bit_vector(){
	for (unordered_map<uint64_t, uint64_t*>::iterator it = bit_vectors_table.begin(); it != bit_vectors_table.end(); it++)
		delete[] it->second;
}

uint64_t* II_bit_vector::query(uint64_t* items, uint64_t size){
	uint64_t bitvectorRawSize = sequenceNumber / 64;
	bitvectorRawSize = sequenceNumber%64 != 0 ? bitvectorRawSize + 1 : bitvectorRawSize;

	uint64_t* results = new uint64_t[bitvectorRawSize];
	for (uint64_t j = 0; j < bitvectorRawSize; j++) results[j] = (uint64_t)-1;

	for (uint64_t i = 0; i < size; i++) {

		uint64_t* tmp_bv = bit_vectors_table[items[i]];
		//uint64_t* tmp_bvRawData = tmp_bv.data();
		for (uint64_t j = 0; j < bitvectorRawSize; j++) { // for every uint64_t of the bit_vector make a bitwise-and with the result bit_vector
			results[j] = results[j] & tmp_bv[j];
		}
	}
	return results;
}

uint64_t II_bit_vector::getCardinality(uint64_t item){
	uint64_t cardinalityCounter = 0;
	uint64_t* tmp_bv = bit_vectors_table[item];
	for (uint64_t  i = 0; i < sequenceNumber; i ++)
		if ((tmp_bv[(i) / 64] >> ((i) % 64)) & 1) cardinalityCounter++;
	return cardinalityCounter;
}

int main(){

	// vector<uint64_t> seq1 = {1, 2, 3, 4, 5};
	// Sequence* s1 = new Sequence(seq1);

	// vector<uint64_t> seq2 = {6, 7, 8, 1};
	// Sequence* s2 = new Sequence(seq2);


	// vector<uint64_t> seq3 = {10, 1, 12};
	// Sequence* s3 = new Sequence(seq3);

	// std::vector<Sequence*> v = {s1, s2, s3};

	DatabaseHelper db("BIBLE.txt", DatabaseHelper::TXT);
	// for(uint64_t i = 0; i < db.getDatabase().size(); i++){
	// 	db.getDatabase()[i]->print();
	// }

	InvertedIndex* II = new II_bit_vector(db.getDatabase());
	uint64_t counter = 0;
	for(uint64_t i = 0; i < 5000; i++){
	 	uint64_t* answer = II->query(db.getDatabase()[i]->getItems(), db.getDatabase()[i]->size());
	 	for (uint64_t j = 0; j < II->getSequenceNumber(); j++)
			if ((answer[(j) / 64] >> ((j) % 64)) & 1) counter++;
		delete[] answer;
	 }

	 cout << db.getDatabase().size() << endl;

	 cout << counter << endl;

	delete II;
	// delete s1;
	// delete s2;
	// delete s3;

	return 0;
}