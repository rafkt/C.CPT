//
//  invertedIndexComparison.h
//  
//
//  Created by Rafael Ktistakis on 02/09/2015.
//
//

#include <sdsl/bit_vectors.hpp>

class Elias_Fano{
	
	public:
		Elias_Fano(uint64_t*, uint64_t);
		~Elias_Fano();
		void printArray();
		bool find(uint64_t);
		double size_in_MB();
		sdsl::bit_vector get_H_part();
		uint64_t getBits();
		uint64_t getSize();
		uint64_t getL();
		sdsl::int_vector<> get_L_part();
	private:
		enum dictionariesSpeed{HYBRID, SLOW, FAST}; //in expense or favour of memory overhead
		uint64_t* h_part;
		sdsl::bit_vector* h_part_sdsl_bv;
		sdsl::select_support* select_h_part;
		sdsl::rank_support* rank_h_part;
		sdsl::int_vector<>* l_part;
		uint64_t m;
		uint64_t l;
		uint64_t bits;
		uint64_t size;
		uint64_t total_zeros;
};
