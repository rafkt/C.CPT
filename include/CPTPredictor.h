#include <vector>
#include "Sequence.h"
#include "CPT_Trie.h"
#include "InvertedIndex.h"
#include "Predictor.h"
#include <unordered_map>
#include <set>

#ifndef CPTPREDICTOR_H
#define CPTPREDICTOR_H
class CPTPredictor : public Predictor{
	public:
		CPTPredictor(std::vector<Sequence*>);
		~CPTPredictor();
		Sequence* Predict(Sequence*);
		uint64_t size();
	protected:
		bool Train(std::vector<Sequence*>);
	private:
		PredictionTree* root;
		InvertedIndex *II;
		CPT_Trie** LT;
		uint64_t nodeNumber;
		std::vector<uint64_t> getMatchingSequences(Sequence*);
		void UpdateCountTable(Sequence*, float, std::unordered_map<uint64_t, float>, std::set<uint64_t>);
		Sequence* getBestSequenceFromCountTable(std::unordered_map<uint64_t, float>, bool);

};
#endif