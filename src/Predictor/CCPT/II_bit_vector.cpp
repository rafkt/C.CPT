#include "../../../include/II_bit_vector.h"
#include "../../../include/Sequence.h"

// #include "../../../include/DatabaseHelper.h"
// #include "../../../include/Profile.h"


#include <iostream>
#include <math.h>

using namespace std;
using namespace sdsl;

II_bit_vector::II_bit_vector(std::vector<Sequence*> sq) : InvertedIndex(sq){
	uint64_t bv_size = ceil(_size / 64.0);
	uint64_t set_bit = 1;
	uint64_t select_memory = 0;
	uint64_t bv_memory = 0;
	for (unordered_map<uint64_t, vector<uint64_t>>::iterator it = alphabet2sequences_table.begin(); it != alphabet2sequences_table.end(); it++){
		bit_vector* tmp_bv = new bit_vector(_size, 0);
		uint64_t* tmp_bv_data = tmp_bv->data();
		for (uint64_t i = 0; i < bv_size; i++) tmp_bv_data[i] = 0;
		for (uint64_t i = 0; i < it->second.size(); i++) tmp_bv_data[it->second[i] / 64] |= set_bit << (it->second[i] % 64);
		bit_vectors_table.insert({it->first, tmp_bv});
		//bit_vector::rank_1_type* slc = new bit_vector::rank_1_type(tmp_bv);
		ii_bv_rank.insert({it->first, it->second.size()});
		//delete slc;
		//select_memory += size_in_bytes(*slc);
		bv_memory += size_in_bytes(*tmp_bv);
	}
	delete[] II_database;
	alphabet2sequences_table.erase(alphabet2sequences_table.begin(), alphabet2sequences_table.end());
	cout << "Sigma: " << Sigma << endl;
	cout << "Extra array bytes: " << Sigma * 8 << endl;
	cout << "Total select bytes: " << select_memory << endl;
	cout << "Total BV bytes: " << bv_memory << endl;
}

II_bit_vector::~II_bit_vector(){
	for (unordered_map<uint64_t, bit_vector*>::iterator it = bit_vectors_table.begin(); it != bit_vectors_table.end(); it++){
		if (it->second) delete it->second;
		it->second = nullptr;
	}
	// for (unordered_map<uint64_t, bit_vector::rank_1_type*>::iterator it = ii_bv_rank.begin(); it != ii_bv_rank.end(); it++){
	// 	if (it->second) delete it->second;
	// 	it->second = nullptr;
	// }
}

bool II_bit_vector::itemIsValidAlphabet(uint64_t item){
	if (bit_vectors_table.find(item) == bit_vectors_table.end()) return false;
	return true;
}

vector<uint64_t> II_bit_vector::query_(uint64_t* items, uint64_t size){
	cerr << "NOT IMPLEMTED ON RAW II" << endl;
	std::vector<uint64_t> v;
	return v;
}

uint64_t* II_bit_vector::query(uint64_t* items, uint64_t size){
	uint64_t bitvectorRawSize = sequenceNumber / 64;
	bitvectorRawSize = sequenceNumber%64 != 0 ? bitvectorRawSize + 1 : bitvectorRawSize;

	uint64_t* results = new uint64_t[bitvectorRawSize];
	for (uint64_t j = 0; j < bitvectorRawSize; j++) results[j] = (uint64_t)-1;

	for (uint64_t i = 0; i < size; i++) {
		
		uint64_t* tmp_bv = bit_vectors_table[items[i]]->data();
		//uint64_t* tmp_bvRawData = tmp_bv.data();
		for (uint64_t j = 0; j < bitvectorRawSize; j++) { // for every uint64_t of the bit_vector make a bitwise-and with the result bit_vector
			results[j] = results[j] & tmp_bv[j];
		}
	}
	return results;
}

uint64_t II_bit_vector::getCardinality(uint64_t item){
	// uint64_t cardinalityCounter = 0;
	// uint64_t* tmp_bv = bit_vectors_table[item];
	// for (uint64_t  i = 0; i < sequenceNumber; i ++)
	// 	if ((tmp_bv[(i) / 64] >> ((i) % 64)) & 1) cardinalityCounter++;
	//return (*(ii_bv_rank[item]))(_size - 1);
	return ii_bv_rank[item];
}

// int main(){

// 	// vector<uint64_t> seq1 = {1, 2, 3, 4, 5};
// 	// Sequence* s1 = new Sequence(seq1);

// 	// vector<uint64_t> seq2 = {6, 7, 8, 1};
// 	// Sequence* s2 = new Sequence(seq2);


// 	// vector<uint64_t> seq3 = {10, 1, 12};
// 	// Sequence* s3 = new Sequence(seq3);

// 	// std::vector<Sequence*> v = {s1, s2, s3};
// 	Profile* pf = new Profile();
// 	pf->apply();
// 	DatabaseHelper db("BIBLE.txt", DatabaseHelper::TXT, pf);
// 	// for(uint64_t i = 0; i < db.getDatabase().size(); i++){
// 	// 	db.getDatabase()[i]->print();
// 	// }

// 	InvertedIndex* II = new II_bit_vector(db.getDatabase());
// 	uint64_t counter = 0;
// 	for(uint64_t i = 0; i < 5000; i++){
// 	 	uint64_t* answer = II->query(db.getDatabase()[i]->getItems(), db.getDatabase()[i]->size());
// 	 	for (uint64_t j = 0; j < II->getSequenceNumber(); j++)
// 			if ((answer[(j) / 64] >> ((j) % 64)) & 1) counter++;
// 		delete[] answer;
// 	}

// 	cout << db.getDatabase().size() << endl;

// 	cout << counter << endl;

// 	delete II;
// 	// delete s1;
// 	// delete s2;
// 	// delete s3;

// 	return 0;
// }
