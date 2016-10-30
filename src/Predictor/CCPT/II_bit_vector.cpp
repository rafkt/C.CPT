#include "../../../include/II_bit_vector.h"
#include "../../../include/Sequence.h"
#include <iostream>

using namespace std;

II_bit_vector::II_bit_vector(std::vector<Sequence*> sq) : InvertedIndex(sq){
	for (unordered_map<uint64_t, vector<uint64_t>>::iterator it = alphabet2sequences_table.begin(); it != alphabet2sequences_table.end(); it++){
		uint64_t bv_size = ceil(_size / 64.0);
		uint64_t* tmp_bv = new uint64_t[bv_size];
		for (uint64_t i = 0; i < bv_size; i++) 
			tmp_bv[i] = 0;
		for (uint64_t i = 0; i < it->second.size(); i++) 
			tmp_bv[it->second[i] / 64] |= 1 << (it->second[i] % 64);
		bit_vectors_table.insert({it->first, tmp_bv});
	}
	delete[] II_database;
}

II_bit_vector::~II_bit_vector(){
	for (unordered_map<uint64_t, uint64_t*>::iterator it = bit_vectors_table.begin(); it != bit_vectors_table.end(); it++)
		delete[] it->second;
}

uint64_t* II_bit_vector::query(uint64_t* items, uint64_t size){
	return nullptr;
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