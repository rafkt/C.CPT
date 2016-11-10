#include "InvertedIndex.h"
#include <vector>
#include <unordered_map>


#ifndef II_BIT_VECTOR_H
#define II_BIT_VECTOR_H
class II_bit_vector : public InvertedIndex{
	public:
		II_bit_vector(std::vector<Sequence*>);
		~II_bit_vector();
		uint64_t* query(uint64_t*, uint64_t);
		uint64_t getCardinality(uint64_t);
	private:
		std::unordered_map<uint64_t, uint64_t*> bit_vectors_table;
};
#endif
