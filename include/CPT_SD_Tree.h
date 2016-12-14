#include "PredictionTree.h"
#include <sdsl/bit_vectors.hpp>


#ifndef CPT_SD_TREE_H
#define CPT_SD_TREE_H
class  CPT_SD_Tree : public PredictionTree{
	public:
		CPT_SD_Tree(PredictionTree*, std::map<uint64_t, uint64_t>, uint64_t);
		~CPT_SD_Tree();
		void levelOrderTraverse(PredictionTree*, std::map<uint64_t, uint64_t>);
		void addChild(uint64_t);
		bool hasChild(uint64_t);
		PredictionTree* getChild(uint64_t);
		PredictionTree* getParent();
		uint64_t getParentIndex(uint64_t);
		std::vector<uint64_t> getNodesToRoot(uint64_t);
		uint64_t getItem() const;
		float memoryInMB() const;
	private:
		sdsl::bit_vector* bitstring;
		sdsl::sd_vector<>* sd_bitstring;
		uint64_t sigma;
		uint64_t* sArray;
		uint64_t nodeNumber;
		sdsl::sd_vector<>::select_1_type* sd_bitstring_select;
};
#endif