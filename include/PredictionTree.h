#include <vector>

#ifndef PREDICTIONTREE_H
#define PREDICTIONTREE_H
class PredictionTree{
	public:
		virtual ~PredictionTree();
		uint64_t getSupport();
		virtual uint64_t getItem();
		PredictionTree* getParrent();
		std::vector<PredictionTree*> getChildren();
		uint64_t getChildrenCount();
	protected:
		PredictionTree();
		PredictionTree(uint64_t);
		virtual void addChild(uint64_t);
		virtual bool hasChild(uint64_t);
		virtual PredictionTree* getChild(uint64_t);
	private:
		uint64_t Support;
		uint64_t item;
		PredictionTree* parent;
		std::vector<PredictionTree*> children;
};
#endif