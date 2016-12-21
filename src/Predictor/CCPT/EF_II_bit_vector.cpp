#include "../../../include/EF_II_bit_vector.h"
#include "../../../include/DatabaseHelper.h"

using namespace std;
using namespace sdsl;

EF_II_bit_vector::EF_II_bit_vector(vector<Sequence*> trainingSet) : II_bit_vector(trainingSet){
	uint64_t bv_size = ceil(sequenceNumber / 64.0);
	for (unordered_map<uint64_t, uint64_t*>::iterator it = bit_vectors_table.begin(); it != bit_vectors_table.end(); it++){
		bit_vector* bitstring = new bit_vector(bv_size, 0);
		uint64_t* bitstring_data = bitstring->data();
		for (uint64_t i = 0; i < bv_size; i++) 
			bitstring_data[i] = it->second[i];
		sd_vector<>* sd_bitstring = new sd_vector<>(*bitstring);
		delete bitstring;
		EF_II_bvs.insert({it->first, sd_bitstring});
	}
	II_bit_vector::~II_bit_vector();
}
EF_II_bit_vector::~EF_II_bit_vector(){
	for (unordered_map<uint64_t, sd_vector<>*>::iterator it = EF_II_bvs.begin(); it != EF_II_bvs.end(); it++){
		delete it->second;
	}
}
uint64_t* EF_II_bit_vector::query(uint64_t* items, uint64_t size){
	return nullptr;
}
uint64_t EF_II_bit_vector::getCardinality(uint64_t item){

	return 0;
}


int main(){

	// vector<uint64_t> seq1 = {1, 2, 3, 4, 5};
	// Sequence* s1 = new Sequence(seq1);

	// vector<uint64_t> seq2 = {6, 7, 8, 1};
	// Sequence* s2 = new Sequence(seq2);


	// vector<uint64_t> seq3 = {10, 1, 12};
	// Sequence* s3 = new Sequence(seq3);

	// std::vector<Sequence*> v = {s1, s2, s3};

	Profile* pf = new Profile();
	pf->apply();

	DatabaseHelper db("BIBLE.txt", DatabaseHelper::TXT, pf);
	// for(uint64_t i = 0; i < db.getDatabase().size(); i++){
	// 	db.getDatabase()[i]->print();
	// }

	InvertedIndex* II = new EF_II_bit_vector(db.getDatabase());
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
	delete pf;
	// delete s1;
	// delete s2;
	// delete s3;

	return 0;
}