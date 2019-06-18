#include "../../../include/EF_II_bit_vector.h"
#include "../../../include/DatabaseHelper.h"
#include <set>

using namespace std;
using namespace sdsl;

EF_II_bit_vector::EF_II_bit_vector(vector<Sequence*> trainingSet, uint64_t* LT, uint64_t nodeNumber) : InvertedIndex(trainingSet), nodeNumber(nodeNumber){

	for (unordered_map<uint64_t, vector<uint64_t>>::iterator it = alphabet2sequences_table.begin(); it != alphabet2sequences_table.end(); it++){
		vector<uint64_t> v = it->second;
		// for (uint64_t i = 0; i < v.size(); i++){
		// 	v[i] = LT[v[i]];
		// }
		sort(v.begin(), v.end());
		Elias_Fano* ef_arr = new Elias_Fano(&v[0], v.size());
		EF_II_bvs.insert({it->first, ef_arr});
	}
	delete[] II_database;
	alphabet2sequences_table.erase(alphabet2sequences_table.begin(), alphabet2sequences_table.end());
}
EF_II_bit_vector::~EF_II_bit_vector(){
	for (unordered_map<uint64_t, Elias_Fano*>::iterator it = EF_II_bvs.begin(); it != EF_II_bvs.end(); it++){
		if (it->second) delete it->second;
		it->second = nullptr;
	}
}

uint64_t EF_II_bit_vector::getSequenceNumber(){
	return nodeNumber;
}

float EF_II_bit_vector::memoryInMB(){
	float counter = 0;
	for (unordered_map<uint64_t, Elias_Fano*>::iterator it = EF_II_bvs.begin(); it != EF_II_bvs.end(); it++){
		counter += it->second->size_in_MB();
	}
	return counter;
}
bool EF_II_bit_vector::itemIsValidAlphabet(uint64_t item){
	if (EF_II_bvs.find(item) == EF_II_bvs.end()) return false;
	return true;
}

uint64_t* EF_II_bit_vector::query(uint64_t* items, uint64_t size){
	cerr << "NOT IMPLEMENTED ON EF_II - USE query_" << endl;
 	return nullptr;
}

vector<uint64_t> EF_II_bit_vector::query_(uint64_t* items, uint64_t size){

	uint64_t minSize = nodeNumber;
	bool flag = false;
	uint64_t first_EF_index = 0;

	//Here start decode EF_bv for the first Query item
	//Then since we have a ef_seqs, we should search these items in the Q[1] corresponding EF. We keep the ones found in a new ef_seqs and we repeat for Q[2]. So the idea is to merge the sets as we decode.

	//obtaining first EF_set belonging to first query item
	uint64_t i = 0;
	vector<uint64_t> ef_seqs;
	Elias_Fano* ef_table;
	do{
		if (EF_II_bvs[items[i]]->getSize() < minSize){
			ef_table = EF_II_bvs[items[i]];
			first_EF_index = i;
			minSize = EF_II_bvs[items[i]]->getSize();
		}
		i++; //here we can consider finding as a firt EF_set the smallest one
	}while (i < size);
	
	//we have an Elias Fano vector to decode and merge with result set
	uint64_t counter = 0;
	uint64_t current_number = 0;
	uint64_t bit_to_start = 0;
	uint64_t word_index;
	uint8_t bit;

	uint64_t bits = ef_table->getBits();
	bit_vector h_part_bv = ef_table->get_H_part();
	uint64_t* h_part = h_part_bv.data();

	for (int f = 0; f < ef_table->getSize(); f++){
		for (uint64_t b = bit_to_start; b < bits; b++){
			word_index = b / 64;
			bit = h_part[word_index] & 1;
			h_part[word_index] >>= 1;
			//h_part[word_index] >>= 1;
			counter += 1 - bit;
			if (bit){
				current_number = counter << ef_table->getL();
				current_number |= ((ef_table->get_L_part()))[f];
				bit_to_start = b + 1;
				break;
			}
		}
		ef_seqs.push_back(current_number);	
	}

	for (int i = 0; i < size; i++){
		Elias_Fano* ef_table =EF_II_bvs[items[i]];
		if (i == first_EF_index) continue;
		vector<uint64_t> tmp;
		for (vector<uint64_t>::iterator it = ef_seqs.begin(); it != ef_seqs.end(); ++it){
			if (ef_table->find(*it)) tmp.push_back(*it);
		}
		ef_seqs = tmp;
	}




	//At this point you should be merging the two results
	//It would be a good idea to check the sizes of each result by an extra experiment. That way you can decide the way to proceed next.
	//cout << ef_seqs.size() << endl;
	// for (vector<uint64_t>::iterator it = ef_seqs.begin(); it != ef_seqs.end(); ++it){
	// 	uint64_t bit =  1;
	// 	II_results[(*it) / 64] |= bit << ((*it) % 64);
	// }

	return ef_seqs;
}
uint64_t EF_II_bit_vector::getCardinality(uint64_t item){
	return EF_II_bvs[item]->getSize();
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

// 	DatabaseHelper* db = new DatabaseHelper("BIBLE.txt", DatabaseHelper::TXT, pf);
// 	// for(uint64_t i = 0; i < db->getDatabase().size(); i++){
// 	// 	db->getDatabase()[i]->print();
// 	// }

// 	InvertedIndex* II = new EF_II_bit_vector(db->getDatabase());
// 	uint64_t counter = 0;
// 	for(uint64_t i = 0; i < 5000; i++){
// 	 	uint64_t* answer = II->query(db->getDatabase()[i]->getItems(), db->getDatabase()[i]->size());
// 	 	for (uint64_t j = 0; j < II->getSequenceNumber(); j++)
// 			if ((answer[(j) / 64] >> ((j) % 64)) & 1) counter++;
// 		delete[] answer;
// 	}

// 	cout << db->getDatabase().size() << endl;

// 	cout << counter << endl;

// 	delete II;
// 	delete pf;
// 	delete db;
// 	// delete s1;
// 	// delete s2;
// 	// delete s3;

// 	return 0;
// }