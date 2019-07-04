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
		if (s->size() < 6) continue;
		if (counter == 1000) break; 
		vector<uint64_t> v;
		v.assign(s->getItems(), s->getItems() + 5);
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

	cout << "QUEST 100K" << endl;
	Evaluator* ev1 = new Evaluator("var_seq/data.slen_20.ncust_2000.tlen_1.nitems_1.txt", DatabaseHelper::SPMF, Evaluator::RAW, 100000);
	ev1->getMemoryUsageInMB();
	ev1->runPredictor();

	delete ev1;

	cout << "QUEST 200K" << endl;
	ev1 = new Evaluator("var_seq/data.slen_20.ncust_2000.tlen_1.nitems_1.txt", DatabaseHelper::SPMF, Evaluator::RAW, 200000);
	ev1->getMemoryUsageInMB();
	ev1->runPredictor();

	delete ev1;

	cout << "QUEST 400K" << endl;
	ev1 = new Evaluator("var_seq/data.slen_20.ncust_2000.tlen_1.nitems_1.txt", DatabaseHelper::SPMF, Evaluator::RAW, 400000);
	ev1->getMemoryUsageInMB();
	ev1->runPredictor();

	delete ev1;

	cout << "QUEST 800K" << endl;
	ev1 = new Evaluator("var_seq/data.slen_20.ncust_2000.tlen_1.nitems_1.txt", DatabaseHelper::SPMF, Evaluator::RAW, 800000);
	ev1->getMemoryUsageInMB();
	ev1->runPredictor();

	delete ev1;
	
	cout << "QUEST 1.6KK" << endl;
	ev1 = new Evaluator("var_seq/data.slen_20.ncust_2000.tlen_1.nitems_1.txt", DatabaseHelper::SPMF, Evaluator::RAW, 1600000);
	ev1->getMemoryUsageInMB();
	ev1->runPredictor();

	delete ev1;
	
	
}
