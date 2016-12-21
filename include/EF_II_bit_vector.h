#include "II_bit_vector.h"
#include <vector>
#include <sdsl/bit_vectors.hpp>
#include <unordered_map>

#ifndef EF_II_BIT_VECTOR_H
#define EF_II_BIT_VECTOR_H
class EF_II_bit_vector : public II_bit_vector{
	public:
		EF_II_bit_vector(std::vector<Sequence*>);
		~EF_II_bit_vector();
		uint64_t* query(uint64_t*, uint64_t);
		uint64_t getCardinality(uint64_t);
	private:
		std::unordered_map<uint64_t, sdsl::sd_vector<>*> EF_II_bvs;
};
#endif
