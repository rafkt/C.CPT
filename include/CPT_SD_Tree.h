#include "PredictionTree.h"
#include <sdsl/bit_vectors.hpp>

#ifndef CPT_SD_TREE_H
#define CPT_SD_TREE_H
class  CPT_SD_Tree : public PredictionTree{
	public:
		CPT_SD_Tree(PredictionTree*);
		~CPT_SD_Tree();
		void levelOrderTraverse(PredictionTree*);
		void addChild(uint64_t);
		bool hasChild(uint64_t);
		PredictionTree* getChild(uint64_t);
		PredictionTree* getParent();
		std::vector<uint64_t> getNodesToRoot();
		uint64_t getItem() const;
};
#endif