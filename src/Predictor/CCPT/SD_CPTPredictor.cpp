#include "../../../include/SD_CPTPredictor.h"
#include "../../../include/CPT_SD_Tree.h"
#include "../../../include/DatabaseHelper.h"
using namespace std;

SD_CPTPredictor::SD_CPTPredictor(vector<Sequence*> trainingSequences, Profile* profile, map<uint64_t, uint64_t> sigmaIndex): CPTPredictor(trainingSequences, profile, sigmaIndex){

	sd_tree = new CPT_SD_Tree(root, sigmaIndex, nodeNumber);

	LT_SD = new uint64_t[trainingSequences.size()];
	for (uint64_t i = 0; i < trainingSequences.size(); i ++) LT_SD[i] = LT[i]->id;

	cout << "LT_SD nodes: ";
	for (uint64_t i = 0; i < trainingSequences.size(); i ++) cout << LT_SD[i] << " ";
	cout << endl;

	//after sucessfull construction, delete CPTPredictor structures
	deleteTrie(root);
	delete II;
	delete LT;
	root = nullptr; II = nullptr; LT = nullptr;
}
SD_CPTPredictor::~SD_CPTPredictor(){
	delete sd_tree;
	delete LT_SD;
}

int main(){
	Profile* pf = new Profile();
	pf->apply();
	DatabaseHelper* db = new DatabaseHelper("test.txt", DatabaseHelper::TXT, pf);
	Predictor* cpt_pr = new SD_CPTPredictor(db->getDatabase(), pf, db->getSigmaIndex());


	// vector<uint64_t> v = {356, 122};
	// Sequence* target = new Sequence(v);
	// Sequence* predicted = cpt_pr->Predict(target);
	// cout << "Prtedicted: " << endl;
	// predicted->print();
	// cout << endl;
	// cout << "Memory size of Predictor: " << cpt_pr->memoryInMB() << "MB";

	 delete cpt_pr;
	 delete pf;
	 delete db;
	// delete target;
	// delete predicted;
}