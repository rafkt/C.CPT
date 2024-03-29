//
//  test.cpp
//  
//
//  Created by Rafael Ktistakis on 02/09/2015.
//
//

//----------------------------------------------------------------------------------------------------------
// Compile with:
// g++ -std=c++11 -O3 -g -DNDEBUG -I ~/include -L ~/lib Elias_Fano.cpp -lsdsl -ldivsufsort -ldivsufsort64
//----------------------------------------------------------------------------------------------------------

#include "../../include/Elias_Fano.h"
#include <iostream>

using namespace sdsl;
using namespace std;

Elias_Fano::Elias_Fano(uint64_t* array, uint64_t size): size(size){
	//array should be sorted

	int _l = floor(log2(array[size - 1] / size));
	_l = (_l <= 0 ? 1 : _l);
	l = _l;

	m = array[size - 1] / pow(2, l);

	bits = ceil(m + size);

	uint64_t h_part_size = ceil((m + size)/(double) 64);
	uint64_t l_part_size = size;//(int)ceil((size * l) / (double)64);

	uint64_t set_bit = 1;


	//cout << l << " " << m << endl;

	//h_part = new uint64_t[h_part_size];
	l_part = new int_vector<>(l_part_size, 0, l);



	uint64_t l_mask = 1 << l;
	l_mask--;

	uint64_t current_high = 0, previous_high = 0, h_word = 0;

	h_part_sdsl_bv = new bit_vector(ceil(m + size), 0);
	uint64_t* tmpBV_raw = h_part_sdsl_bv->data();

	int rem_bits = 64;
	uint64_t counter = 0;
	uint64_t l_word = 0, wordsPer64 = 64 / l;
	int bits_writen = 0;
	for (int i = 0; i < size; i++){
		current_high = array[i] >> l;
		current_high -= previous_high;
		counter += current_high;
		h_word = counter / 64;
		tmpBV_raw[h_word] |= set_bit << (counter % 64); 
		counter++;
		(*l_part)[i] = array[i] & l_mask;

		previous_high = array[i] >> l;
	}
	//cout << h_part[0] << endl;


	//setting up h_part in a bit vector in order to accomodate a select opration

	h_part = tmpBV_raw;

	//select_h_part = new bit_vector::select_0_type(h_part_sdsl_bv);

	rank_support_v<0> rs(h_part_sdsl_bv);
	total_zeros = rs.rank(ceil(m + size));

	//we can create dictionaries based on threashold; which was experimentally found
	// rank_support_v(5) is fast O(1) but has a ~50 bytes fixed overhead (theoretic lower bound) for short bit-vectors
	// same for select_support_mcl O(1) but overhead for short bit-vectors.
	// rank/select_support_scan, no overhead at all for any bit-vector length.
	// But makes the implementation slower.
	// Work-around, use O(1) dicrtionaries in expense for memory but for longer bit-vectors.
	// The overhead will be minimised to an upper bound of ~0.2n (look sdsl)

	#ifdef SD_SLOW
		dictionariesSpeed speed = SLOW;
		// cout << "SD_CPT constructed with slow dictionaries" << endl;
	#elif SD_FAST
		dictionariesSpeed speed = FAST;
		// cout << "SD_CPT constructed with fast dictionaries" << endl;
	#elif SD_HYBRID
		dictionariesSpeed speed = HYBRID;
		// cout << "SD_CPT constructed with hybrid dictionaries" << endl;
	#else 
		dictionariesSpeed speed = FAST;
		// cout << "SD_CPT constructed with fast dictionaries" << endl;
	#endif

	//dictionariesSpeed speed = SLOW;
	switch(speed){
		case HYBRID:
			if (h_part_sdsl_bv->size() > 2800){
				select_h_part = new select_support_mcl<0>(h_part_sdsl_bv);
			}else{
				select_h_part = new select_support_scan<0>(h_part_sdsl_bv);
			}
			if (h_part_sdsl_bv->size() > 1024){
				rank_h_part = new rank_support_v5<1>(h_part_sdsl_bv);	
			}else{
				rank_h_part = new rank_support_scan<1>(h_part_sdsl_bv);
			}
			break;
		case SLOW:
			select_h_part = new select_support_scan<0>(h_part_sdsl_bv);
			rank_h_part = new rank_support_scan<1>(h_part_sdsl_bv);
			break;
		case FAST:
			select_h_part = new select_support_mcl<0>(h_part_sdsl_bv);
			rank_h_part = new rank_support_v5<1>(h_part_sdsl_bv);
			break;
	}
}

void Elias_Fano::printArray(){
	uint64_t counter = 0;
	uint64_t current_number = 0;
	uint64_t bit_to_start = 0;
	uint64_t wordsPer64 = 64 / l;
	uint64_t word_index;
	uint8_t bit;
	for (int i = 0; i < size; i++){
		for (uint64_t j = bit_to_start; j < bits; j++){
			word_index = j / 64;
			bit = h_part[word_index] & 1;
			h_part[word_index] >>= 1;
			counter += 1 - bit;
			if (bit){
				current_number = counter << l;
				current_number |= (*l_part)[i];
				bit_to_start = j + 1;
				break;
			}
		}
		cout << current_number << " ";
	}
	cout << endl;
}

double Elias_Fano::size_in_MB(){
	return (8 + (ceil((m + size)/(double) 64)) * 8) / (double) 1048576 + size_in_mega_bytes(*l_part) + size_in_mega_bytes(*rank_h_part) + size_in_mega_bytes(*select_h_part);
}

Elias_Fano::~Elias_Fano(){
	//delete[] h_part;
	delete l_part;
	delete rank_h_part;
	delete select_h_part;
	delete h_part_sdsl_bv;
}

bit_vector Elias_Fano::get_H_part(){
	return *h_part_sdsl_bv;
}

uint64_t Elias_Fano::getBits(){
	return bits;
}
uint64_t Elias_Fano::getSize(){
	return size;
}
uint64_t Elias_Fano::getL(){
	return l;
}
int_vector<> Elias_Fano::get_L_part(){
	return *l_part;
}

bool Elias_Fano::find(uint64_t value){
	uint64_t counter = 0;
	uint64_t current_number = 0;
	uint8_t bit;
	uint64_t bit_to_find = (value) / pow(2, l);
	bool zero_found = false;
	uint64_t bit_index, f;
	if (bit_to_find > total_zeros) return false;
	if (!bit_to_find){
		bit_index = 0;
		f = 0;
		counter= 0;
	}else{
		//auto begin_clk = clock();
		bit_index = (*select_h_part)(bit_to_find) + 1;
		f = rank_h_part->rank(bit_index);
		// auto end_clk = clock();
		// rank_select_duration = ((double)(end_clk - begin_clk))/CLOCKS_PER_SEC;
		// cout << rank_select_duration << endl;
		counter = bit_to_find;
	}
	for (uint64_t b = bit_index; b < bits; b++){
		bit = (*h_part_sdsl_bv)[b];
		if (bit){
			current_number = counter << l;
			current_number |= (*l_part)[f];
			if (current_number == value) return true;
			f++;
			if (f == size) break;
		}else return false;
	}
	return false;
}


// int main(int argc, char* argv[]){

// 	int v[] = {2, 3, 5, 7, 11, 13, 24};

// 	// for (int i = 0; i < 350; i ++) v[i] = 2 * i;
// 	// v[349] = 1024;

// 	Elias_Fano f(v, 7);
// 	//f.printArray();
// 	//cout << f.size_in_MB() << endl;
// 	set<uint64_t> ef_set;
// 	ef_set.insert(1);
// 	ef_set.insert(2);
// 	ef_set.insert(4);
// 	ef_set.insert(7);
// 	ef_set.insert(8);
// 	ef_set.insert(1024);
// 	ef_set.insert(2024);
	
// 	for (set<uint64_t>::iterator it = ef_set.begin(); it != ef_set.end(); ++it){
// 		if (f.find(*it)) cout << *it << " FOUND" << endl;
// 	}

// 	return 0;	
// }


// int main(int argc, char* argv[]){

// 	cout << "Bit-length: " << argv[1] << endl;

// 	//constructing random bit-vector
// 	bit_vector bv;

//     bool default_value = 0;//ID[ID.length()-1]-'0';
//     bv = bit_vector(atoi(argv[1]), default_value);
//     std::mt19937_64 rng;
//     std::uniform_int_distribution<uint64_t> distribution(0, bv.size()-1);
//     auto dice = bind(distribution, rng);
//     // populate vectors with some other bits
//     for (uint64_t i=0; i < atoi(argv[2]); ++i) {
//         uint64_t x = dice();
//         bv[x] = !default_value;
//     }

//     sdsl::select_support_mcl<0,1>* bvs  = new select_support_mcl<0,1>(&bv);
//     sdsl::rank_support_v<1,1>* bvr  = new rank_support_v<1,1>(&bv);

//     uint64_t set_bits_number = bvr->rank(bv.size());
//     uint64_t* set_bits = new uint64_t[set_bits_number];

//     uint64_t counter = 0;
//     for (uint64_t i = 0; i < bv.size(); i++){
//     	if (bv[i] == 1) set_bits[counter++] = i;
//     }

//     //Elias_Fano* my_ef = new Elias_Fano(set_bits, set_bits_number);
//     //sdsl::sd_vector<>* sd_bitstring = new sd_vector<>(bv);
//     //sdsl::select_support_sd<1>* sd_bitstring_select = new select_support_sd<1>(sd_bitstring);


//     // cout << "My EF: " << my_ef->size_in_MB() << endl;
//     // cout << "sd_vector: " << size_in_mega_bytes(*sd_bitstring) << endl;
//     // cout << "select_support_sd: " << size_in_bytes(*sd_bitstring_select) << endl;

//     sdsl::select_support_mcl<0>* _select = new select_support_mcl<0>(&bv);
//     sdsl::rank_support_v5<1>* _rank = new rank_support_v5<1>(&bv);
//     cout << "rank_support_v5: " << size_in_bytes(*_rank) << " Expected: " << 0.0625 * atoi(argv[1]) / 8 << endl;
//     cout << "select_support_mcl: " << size_in_bytes(*_select) << " Expected: " << 0.375 * atoi(argv[1]) / 8 << endl;


    
//     // for (uint64_t i = 0; i < set_bits_number; i++){
//     // 	if (!my_ef->find(set_bits[i])) {cout << "BUG1" << endl; return 0;}
//     // }

// 	return 0;
// }