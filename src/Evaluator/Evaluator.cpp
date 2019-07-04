#include "../../include/Evaluator.h"
#include "../../include/SD_CPTPredictor.h"
#include "../../include/CPTPredictor.h"
#include "../../include/CPTPlusPredictor.h"

using namespace std;

Evaluator::Evaluator(string datasetName, DatabaseHelper::Format fm, Predictor_Structures type, int seq__number){
	pf = new Profile();
	pf->apply();
	db = new DatabaseHelper(datasetName, fm, pf, seq__number);
	auto begin_time = clock();
	switch(type){
		case RAW:
			cout << "CPT" << endl;
			pr = new CPTPredictor(db->getDatabase(), pf);
			break;
		case SD:
			cout << "SD_CPT" << endl;
			pr = new SD_CPTPredictor(db->getDatabase(), pf);
			break;
		case PLUS:
			cout << "CPT+" << endl;
			pr = new CPTPlusPredictor(db->getDatabase(), pf);
			break;
	}
	pr->createII();
	auto end_time = clock();
	auto duration = ((double)(end_time - begin_time))/CLOCKS_PER_SEC;
	cout << "Train (s): " << duration << endl;
}
Evaluator::~Evaluator(){
	delete pr;
	delete pf;
	delete db;
}
float Evaluator::getMemoryUsageInMB(){
	return pr->memoryInMB();
}
void Evaluator::runPredictor(){
	std::vector<Sequence*> db_v = db->getDatabase();

	auto begin_time = clock();
	uint64_t counter = 0;
	for(Sequence* s :db_v){
		if (s->size() < 5) continue;
		if (counter == 1000) break; 
		vector<uint64_t> v;
		v.assign(s->getItems(), s->getItems() + 4);
		Sequence* target = new Sequence(v);
		//begin_time = clock();
		Sequence* predicted = pr->Predict(target);
		// for (int i = 0; i < predicted->size(); i++){
		// 	cout << predicted->getItems()[i] << " ";
		// }
		// cout << endl;
		//end_time = clock();
		//duration += ((double)(end_time - begin_time))/CLOCKS_PER_SEC;
		delete predicted;
		delete target;
		counter++;
	}

	auto end_time = clock();
	auto duration = ((double)(end_time - begin_time))/CLOCKS_PER_SEC;

	//cout << "Memory: " << pr->memoryInMB() << endl;
	cout << "Duration (s): " << duration << endl;
}

int main(){

	
	  
	  
	  

	cout << "QUEST 5_long" << endl;
	Evaluator* ev1 = new Evaluator("var_length/data.slen_5.ncust_1200.tlen_1.nitems_1.txt", DatabaseHelper::SPMF, Evaluator::PLUS, -1);
	ev1->getMemoryUsageInMB();
	ev1->runPredictor();

	delete ev1;

	cout << "QUEST 10_long" << endl;
	ev1 = new Evaluator("var_length/data.slen_10.ncust_600.tlen_1.nitems_1.txt", DatabaseHelper::SPMF, Evaluator::PLUS, -1);
	ev1->getMemoryUsageInMB();
	ev1->runPredictor();

	delete ev1;

	cout << "QUEST 20_long" << endl;
	ev1 = new Evaluator("var_length/data.slen_20.ncust_300.tlen_1.nitems_1.txt", DatabaseHelper::SPMF, Evaluator::PLUS, -1);
	ev1->getMemoryUsageInMB();
	ev1->runPredictor();

	delete ev1;

	cout << "QUEST 40_long" << endl;
	ev1 = new Evaluator("var_length/data.slen_40.ncust_150.tlen_1.nitems_1.txt", DatabaseHelper::SPMF, Evaluator::PLUS, -1);
	ev1->getMemoryUsageInMB();
	ev1->runPredictor();

	delete ev1;
	
}
