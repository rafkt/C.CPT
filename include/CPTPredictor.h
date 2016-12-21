#include <vector>
#include "Sequence.h"
#include "CPT_Trie.h"
#include "InvertedIndex.h"
#include "Predictor.h"
#include <unordered_map>
#include <set>
#include "Profile.h"
#include <map>

#ifndef CPTPREDICTOR_H
#define CPTPREDICTOR_H
class CPTPredictor : public Predictor{
	public:
		CPTPredictor(std::vector<Sequence*>, Profile*);
		virtual ~CPTPredictor();
		Sequence* Predict(Sequence*);
		uint64_t size();
		float memoryInMB();
	protected:
		bool Train(std::vector<Sequence*>);
		PredictionTree* root;
		InvertedIndex *II;
		PredictionTree** LT;
		uint64_t nodeNumber;
		uint64_t trainingSequenceNumber;
		Profile* profile;
		std::vector<Sequence*> tmp_sequences;
		std::vector<uint64_t> getMatchingSequences(Sequence*);
		virtual void UpdateCountTable(Sequence*, float, std::unordered_map<uint64_t, float>&, std::set<uint64_t>&);
		Sequence* getBestSequenceFromCountTable(std::unordered_map<uint64_t, float>, bool);
		void deleteTrie(PredictionTree*);
		//static functions
		static void RecursiveDivider(std::vector<Sequence*>&, Sequence*, uint64_t);
		static Sequence* sliceBasic(Sequence*, uint64_t);
		static Sequence* slice(Sequence*, uint64_t);

		#ifdef SD_CONSTRACTION
		std::map<uint64_t, uint64_t> mapSigmaIndex;
		void sigmaIndex(std::vector<Sequence*>);
		#endif

		virtual std::vector<uint64_t> getBranch(uint64_t);
};
#endif