#include "CPTPredictor.h"
#include "CPT_SD_Tree.h"

#ifndef SD_CPTPREDICTOR_H
#define SD_CPTPREDICTOR_H
class SD_CPTPredictor : public CPTPredictor{
	public:
		SD_CPTPredictor(std::vector<Sequence*>, Profile*);
		~SD_CPTPredictor();
		float memoryInMB();
	private:
		CPT_SD_Tree* sd_tree;
		uint64_t* LT_SD;
		virtual std::vector<uint64_t> getBranch(uint64_t);
};
#endif