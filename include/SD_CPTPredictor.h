#include "CPTPredictor.h"
#include "CPT_SD_Tree.h"
#include "EF_II_bit_vector.h"

#ifndef SD_CPTPREDICTOR_H
#define SD_CPTPREDICTOR_H
class SD_CPTPredictor : public CPTPredictor{
	public:
		SD_CPTPredictor(std::vector<Sequence*>, Profile*);
		~SD_CPTPredictor();
		float memoryInMB();
		void createII();
	private:
		CPT_SD_Tree* sd_tree;
		uint64_t* LT_SD;
		std::vector<uint64_t> getBranch(uint64_t);
		std::vector<uint64_t> getMatchingSequences(Sequence*);
		bool visited(std::set<uint64_t>&, uint64_t);
		void gotVisit(std::set<uint64_t>& , uint64_t);
};
#endif