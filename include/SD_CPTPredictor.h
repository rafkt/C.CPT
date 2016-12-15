#include "CPTPredictor.h"
#include "CPT_SD_Tree.h"

#ifndef SD_CPTPREDICTOR_H
#define SD_CPTPREDICTOR_H
class SD_CPTPredictor : public CPTPredictor{
	public:
		SD_CPTPredictor(std::vector<Sequence*>, Profile*, std::map<uint64_t, uint64_t>);
		~SD_CPTPredictor();
	private:
		PredictionTree* sd_tree;
};
#endif