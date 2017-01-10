#include "InvertedIndex.h"
#include <vector>
#include <unordered_map>

#include <sdsl/bit_vectors.hpp>


#ifndef II_BIT_VECTOR_H
#define II_BIT_VECTOR_H
class II_bit_vector : public InvertedIndex{
	public:
		II_bit_vector(std::vector<Sequence*>);
		virtual ~II_bit_vector();
		virtual uint64_t* query(uint64_t*, uint64_t);
		virtual std::vector<uint64_t> query_(uint64_t*, uint64_t);
		virtual uint64_t getCardinality(uint64_t);
		bool itemIsValidAlphabet(uint64_t);
	protected:
		std::unordered_map<uint64_t, sdsl::bit_vector*> bit_vectors_table;
	private:
		std::unordered_map<uint64_t, sdsl::bit_vector::rank_1_type*> ii_bv_rank;
};
#endif
