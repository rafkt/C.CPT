#include "../../include/Evaluator.h"
#include "../../include/SD_CPTPredictor.h"
#include "../../include/CPTPredictor.h"

using namespace std;

Evaluator::Evaluator(string datasetName, DatabaseHelper::Format fm, Predictor_Structures type){
	pf = new Profile();
	pf->apply();
	db = new DatabaseHelper(datasetName, fm, pf);
	switch(type){
		case RAW:
			pr = new CPTPredictor(db->getDatabase(), pf);
			break;
		case SD:
			pr = new SD_CPTPredictor(db->getDatabase(), pf);
			break;
	}
	pr->createII();
}
Evaluator::~Evaluator(){
	delete pr;
	delete pf;
	delete db;
}
float Evaluator::getMemoryUsageInMB(){
	return 0.0;
}
void Evaluator::runPredictor(){
	vector<uint64_t> v = {356, 122};
	Sequence* target = new Sequence(v);
	Sequence* predicted = pr->Predict(target);
	cout << "Prtedicted: " << endl;
	predicted->print();
	cout << endl;
	cout << pr->memoryInMB() << endl;

	 delete target;
	 delete predicted;
}

int main(){
	Evaluator* ev = new Evaluator("BIBLE.txt", DatabaseHelper::TXT, Evaluator::RAW);

	ev->runPredictor();

	delete ev;
	// cout << "Memory size of Predictor: " << cpt_pr->memoryInMB() << "MB";
	
}