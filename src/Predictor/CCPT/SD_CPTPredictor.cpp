#include "../../../include/SD_CPTPredictor.h"

using namespace std;

SD_CPTPredictor::SD_CPTPredictor(vector<Sequence*> trainingSequences, Profile* profile, map<uint64_t, uint64_t> sigmaIndex): CPTPredictor(trainingSequences, profile, sigmaIndex){

	PredictionTree* sd_tree = new CPT_SD_Tree(root, sigmaIndex, nodeNumber);

	//after sucessfull construction, delete CPTPredictor structures
	//deleteTrie(root);
	//delete II;
	//delete LT;
}
SD_CPTPredictor::~SD_CPTPredictor(){

}