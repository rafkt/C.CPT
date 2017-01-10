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


	cout << "Memory: " << pr->memoryInMB() << endl;
	cout << "Duration: " << duration << endl;
}

int main(){
	Evaluator* ev = new Evaluator("BIBLE.txt", DatabaseHelper::TXT, Evaluator::RAW);

	ev->runPredictor();

	delete ev;
	// cout << "Memory size of Predictor: " << cpt_pr->memoryInMB() << "MB";
	
}