#include "../../include/Evaluator.h"
#include "../../include/SD_CPTPredictor.h"
#include "../../include/CPTPredictor.h"
#include "../../include/CPTPlusPredictor.h"

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
		case PLUS:
			pr = new CPTPlusPredictor(db->getDatabase(), pf);
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

	cout << "SD_CPT" << endl;

	cout << "BMS1_spmf" << endl;
	auto begin_time = clock();
	Evaluator* ev1 = new Evaluator("BMS1_spmf.txt", DatabaseHelper::TXT, Evaluator::SD);
	auto end_time = clock();
	auto duration = ((double)(end_time - begin_time))/CLOCKS_PER_SEC;
	cout << "Train (s): " << duration << endl;
	ev1->getMemoryUsageInMB();
	ev1->runPredictor();

	cout << "BMS2" << endl;
	begin_time = clock();
	Evaluator* ev2 = new Evaluator("BMS2.txt", DatabaseHelper::TXT, Evaluator::SD);
	end_time = clock();
	duration = ((double)(end_time - begin_time))/CLOCKS_PER_SEC;
	cout << "Train (s): " << duration << endl;
	ev2->getMemoryUsageInMB();
	ev2->runPredictor();

	cout << "Kosarak" << endl;
	begin_time = clock();
	Evaluator* ev3 = new Evaluator("Kosarak_converted.txt", DatabaseHelper::SPMF, Evaluator::SD);
	end_time = clock();
	duration = ((double)(end_time - begin_time))/CLOCKS_PER_SEC;
	cout << "Train (s): " << duration << endl;
	ev3->getMemoryUsageInMB();
	ev3->runPredictor();

	cout << "SIGN" << endl;
	begin_time = clock();
	Evaluator* ev4 = new Evaluator("SIGN.txt", DatabaseHelper::TXT, Evaluator::SD);
	end_time = clock();
	duration = ((double)(end_time - begin_time))/CLOCKS_PER_SEC;
	cout << "Train (s): " << duration << endl;
	ev4->getMemoryUsageInMB();
	ev4->runPredictor();

	cout << "BIBLE" << endl;
	begin_time = clock();
	Evaluator* ev5 = new Evaluator("BIBLE.txt", DatabaseHelper::TXT, Evaluator::SD);
	end_time = clock();
	duration = ((double)(end_time - begin_time))/CLOCKS_PER_SEC;
	cout << "Train (s): " << duration << endl;
	ev5->getMemoryUsageInMB();
	ev5->runPredictor();

	cout << "LEVIATHAN" << endl;
	begin_time = clock();
	Evaluator* ev6 = new Evaluator("LEVIATHAN.txt", DatabaseHelper::TXT, Evaluator::SD);
	end_time = clock();
	duration = ((double)(end_time - begin_time))/CLOCKS_PER_SEC;
	cout << "Train (s): " << duration << endl;
	ev6->getMemoryUsageInMB();
	ev6->runPredictor();

	cout << "MSNBC" << endl;
	begin_time = clock();
	Evaluator* ev7 = new Evaluator("MSNBC.txt", DatabaseHelper::TXT, Evaluator::SD);
	end_time = clock();
	duration = ((double)(end_time - begin_time))/CLOCKS_PER_SEC;
	cout << "Train (s): " << duration << endl;
	ev7->getMemoryUsageInMB();
	ev7->runPredictor();

	cout << "FIFA" << endl;
	begin_time = clock();
	Evaluator* ev8 = new Evaluator("FIFA.txt", DatabaseHelper::TXT, Evaluator::SD);
	end_time = clock();
	duration = ((double)(end_time - begin_time))/CLOCKS_PER_SEC;
	cout << "Train (s): " << duration << endl;
	ev8->getMemoryUsageInMB();
	ev8->runPredictor();

	cout << "NASA_access_log_Aug95" << endl;
	begin_time = clock();
	Evaluator* ev9 = new Evaluator("NASA_access_log_Aug95.txt", DatabaseHelper::TXT, Evaluator::SD);
	end_time = clock();
	duration = ((double)(end_time - begin_time))/CLOCKS_PER_SEC;
	cout << "Train (s): " << duration << endl;
	ev9->getMemoryUsageInMB();
	ev9->runPredictor();

	cout << "NASA_access_log_Jul95" << endl;
	begin_time = clock();
	Evaluator* ev10 = new Evaluator("NASA_access_log_Jul95.txt", DatabaseHelper::TXT, Evaluator::SD);
	end_time = clock();
	duration = ((double)(end_time - begin_time))/CLOCKS_PER_SEC;
	cout << "Train (s): " << duration << endl;
	ev10->getMemoryUsageInMB();
	ev10->runPredictor();

	delete ev1;
	delete ev2;
	delete ev3;
	delete ev4;
	delete ev5;
	delete ev6;
	delete ev7;
	delete ev8;
	delete ev9;
	delete ev10;
	//cout << "Memory size of Predictor: " << cpt_pr->memoryInMB() << "MB";
	
}