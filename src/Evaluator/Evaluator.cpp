#include "../../include/Evaluator.h"
#include "../../include/SD_CPTPredictor.h"
using namespace std;

Evaluator::Evaluator(){}
Evaluator::~Evaluator(){}

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
	 cout << cpt_pr->memoryInMB() << endl;
	// cout << "Memory size of Predictor: " << cpt_pr->memoryInMB() << "MB";

	 delete cpt_pr;
	 delete pf;
	 delete db;
	 delete target;
	 delete predicted;
}