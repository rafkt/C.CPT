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
	auto begin_time = clock(), end_time = clock();
	auto duration = ((double)(end_time - begin_time))/CLOCKS_PER_SEC;
	duration = 0;
	uint64_t counter = 0;
	for(Sequence* s : db->getDatabase()){
		if (s->size() < 3) continue;
		if (counter == 1000) break; 
		vector<uint64_t> v;
		v.assign(s->getItems(), s->getItems() + 2);
		Sequence* target = new Sequence(v);
		begin_time = clock();
		Sequence* predicted = pr->Predict(target);
		end_time = clock();
		duration += ((double)(end_time - begin_time))/CLOCKS_PER_SEC;
		delete predicted;
		delete target;
		counter++;
	}


	//cout << "Memory: " << pr->memoryInMB() << endl;
	cout << "Duration / query: " << duration / 1000.0 << endl;
}

int main(){

	cout << "CPT RAW" << endl;

	cout << "BMS1_spmf" << endl;
	Evaluator* ev1 = new Evaluator("BMS1_spmf.txt", DatabaseHelper::TXT, Evaluator::RAW);
	ev1->runPredictor();

	cout << "BMS2" << endl;
	Evaluator* ev2 = new Evaluator("BMS2.txt", DatabaseHelper::TXT, Evaluator::RAW);
	ev2->runPredictor();

	cout << "BMS2_spmf" << endl;
	Evaluator* ev3 = new Evaluator("Kosarak_converted.txt", DatabaseHelper::TXT, Evaluator::RAW);
	ev3->runPredictor();

	cout << "SIGN" << endl;
	Evaluator* ev4 = new Evaluator("SIGN.txt", DatabaseHelper::TXT, Evaluator::RAW);
	ev4->runPredictor();

	cout << "BIBLE" << endl;
	Evaluator* ev5 = new Evaluator("BIBLE.txt", DatabaseHelper::TXT, Evaluator::RAW);
	ev5->runPredictor();

	cout << "LEVIATHAN" << endl;
	Evaluator* ev6 = new Evaluator("LEVIATHAN.txt", DatabaseHelper::TXT, Evaluator::RAW);
	ev6->runPredictor();

	cout << "MSNBC" << endl;
	Evaluator* ev7 = new Evaluator("MSNBC.txt", DatabaseHelper::TXT, Evaluator::RAW);
	ev7->runPredictor();

	cout << "FIFA" << endl;
	Evaluator* ev8 = new Evaluator("FIFA.txt", DatabaseHelper::TXT, Evaluator::RAW);
	ev8->runPredictor();

	cout << "NASA_access_log_Aug95" << endl;
	Evaluator* ev9 = new Evaluator("NASA_access_log_Aug95.txt", DatabaseHelper::TXT, Evaluator::RAW);
	ev9->runPredictor();

	cout << "NASA_access_log_Jul95" << endl;
	Evaluator* ev10 = new Evaluator("NASA_access_log_Jul95.txt", DatabaseHelper::TXT, Evaluator::RAW);
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
	// cout << "Memory size of Predictor: " << cpt_pr->memoryInMB() << "MB";
	
}