#include "PredictionTree.h"

#ifndef CPT_TRIE_H
#define CPT_TRIE_H
class  CPT_Trie : public PredictionTree{
	public:
		CPT_Trie();
		CPT_Trie(uint64_t);
		~CPT_Trie();
		void addChild(uint64_t);
		bool hasChild(uint64_t);
		PredictionTree* getChild(uint64_t);
		PredictionTree* getParent();
		uint64_t getItem() const;
};
#endif