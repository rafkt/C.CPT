#include "DatabaseHelper.h"
#include "Predictor.h"
#include "Sequence.h"
#include <stdio.h>
#include <iostream>
#include <string>

#ifndef EVALUATOR_H
#define EVALUATOR_H
class Evaluator{
	public:
		enum Predictor_Structures{RAW, SD};
		Evaluator(std::string, DatabaseHelper::Format, Predictor_Structures);
		~Evaluator();
		float getMemoryUsageInMB();
		void runPredictor();
	private:
		DatabaseHelper* db;
		Predictor* pr;
		Profile* pf;
};
#endif