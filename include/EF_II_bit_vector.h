#include "InvertedIndex.h"
#include <vector>
#include <unordered_map>
#include "Elias_Fano.h"

#ifndef EF_II_BIT_VECTOR_H
#define EF_II_BIT_VECTOR_H
class EF_II_bit_vector : public InvertedIndex{
	public:
		EF_II_bit_vector(std::vector<Sequence*>, uint64_t*, uint64_t);
		~EF_II_bit_vector();
		uint64_t* query(uint64_t*, uint64_t);
		std::vector<uint64_t> query_(uint64_t*, uint64_t);
		uint64_t getCardinality(uint64_t);
		bool itemIsValidAlphabet(uint64_t);
		float memoryInMB();
		uint64_t getSequenceNumber();

	private:
		std::unordered_map<uint64_t, Elias_Fano*> EF_II_bvs;
		uint64_t nodeNumber;
};
#endif
