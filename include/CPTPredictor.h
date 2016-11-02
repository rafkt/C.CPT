#include <vector>
#include "Sequence.h"
#include "PredictionTree.h"
#include "InvertedIndex.h"
#include "Predictor.h"
#include <unordered_map>
#include <set>

#ifndef CPTPREDICTOR_H
#define CPTPREDICTOR_H
class CPTPredictor : public Predictor{
	public:
		CPTPredictor();
		~CPTPredictor();
		bool Train(std::vector<Sequence*>);
		Sequence* Predict(Sequence*);
		uint64_t size();
	protected:
		
	private:
		PredictionTree* root;
		InvertedIndex *II;
		std::vector<PredictionTree*> LT;
		uint64_t nodeNumber;
		std::vector<uint64_t> getMatchingSequences(Sequence*);
		void UpdateCountTable(Sequence*, float, std::unordered_map<uint64_t, float>, std::set<uint64_t>);
		Sequence* getBestSequenceFromCountTable(std::unordered_map<uint64_t, float>, bool);

};
#endif