#include "PredictionTree.h"

#ifndef CPT_SD_TREE_H
#define CPT_SD_TREE_H
class  CPT_SD_Tree : public PredictionTree{
	public:
		CPT_SD_Tree();
		CPT_SD_Tree(uint64_t);
		~CPT_SD_Tree();
		void addChild(uint64_t);
		bool hasChild(uint64_t);
		PredictionTree* getChild(uint64_t);
		PredictionTree* getParent();
		uint64_t getItem();
};
#endif