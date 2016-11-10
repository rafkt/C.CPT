#include <vector>

#ifndef PREDICTIONTREE_H
#define PREDICTIONTREE_H
class PredictionTree{
	public:
		virtual ~PredictionTree();
		uint64_t getSupport();
		virtual uint64_t getItem();
		virtual PredictionTree* getParent();
		std::vector<PredictionTree*> getChildren();
		uint64_t getChildrenCount();
		virtual void addChild(uint64_t);
		virtual bool hasChild(uint64_t);
		virtual PredictionTree* getChild(uint64_t);
	protected:
		PredictionTree();
		PredictionTree(uint64_t);
	private:
		uint64_t Support;
		uint64_t item;
		PredictionTree* parent;
		std::vector<PredictionTree*> children;
};
#endif