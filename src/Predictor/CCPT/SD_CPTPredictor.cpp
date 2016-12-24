#include "../../../include/SD_CPTPredictor.h"
#include "../../../include/DatabaseHelper.h"
using namespace std;

SD_CPTPredictor::SD_CPTPredictor(vector<Sequence*> trainingSequences, Profile* profile): CPTPredictor(trainingSequences, profile){

	sd_tree = new CPT_SD_Tree(root, mapSigmaIndex, nodeNumber);

	LT_SD = new uint64_t[trainingSequences.size()];
	for (uint64_t i = 0; i < trainingSequences.size(); i ++) LT_SD[i] = LT[i]->id;

	//cout  << "Path to root from 15 is ";
	//vector<uint64_t> v = sd_tree->getNodesToRoot(15);
	//for (uint64_t i : v) cout << i << " ";
	//cout << endl;

	//cout << "LT_SD nodes: ";
	//for (uint64_t i = 0; i < trainingSequences.size(); i ++) cout << LT_SD[i] << " ";
	//cout << endl;

	//after sucessfull construction, delete CPTPredictor structures
	deleteTrie(root);
	//delete II;
	delete LT;
	root = nullptr; 
	//II = nullptr; 
	LT = nullptr;
}
SD_CPTPredictor::~SD_CPTPredictor(){
	delete sd_tree;
	delete LT_SD;
}

float SD_CPTPredictor::memoryInMB(){
	//to implemented after II and LT finalisation
	// sd_tree->get_memory_in_mega_bytes() + II_EF
	// LT should be part of II_EF so no need to add any extra calculation

	return sd_tree->memoryInMB();
}

vector<uint64_t> SD_CPTPredictor::getBranch(uint64_t index){
	//cout << "SD get branch called" << endl;
	return sd_tree->getNodesToRoot(LT_SD[index]);
}

void SD_CPTPredictor::createII(){
	cout << "EF" << endl;
	II = new EF_II_bit_vector(newTrainingSet);
	//delete newTrainingSet since it is not longer needed
	for(uint64_t i = 0; i < newTrainingSet.size(); i++) delete newTrainingSet[i];
}


int main(){
	Profile* pf = new Profile();
	pf->apply();
	DatabaseHelper* db = new DatabaseHelper("BIBLE.txt", DatabaseHelper::TXT, pf);
	Predictor* cpt_pr = new SD_CPTPredictor(db->getDatabase(), pf);
	cpt_pr->createII();

	 vector<uint64_t> v = {356, 122};
	 Sequence* target = new Sequence(v);
	 Sequence* predicted = cpt_pr->Predict(target);
	 cout << "Prtedicted: " << endl;
	 predicted->print();
	 cout << endl;
	// cout << "Memory size of Predictor: " << cpt_pr->memoryInMB() << "MB";

	 delete cpt_pr;
	 delete pf;
	 delete db;
	 delete target;
	 delete predicted;
}