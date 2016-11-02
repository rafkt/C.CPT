#include <string>
#include <vector>
#include "Sequence.h"

#ifndef PREDICTOR_H
#define PREDICTOR_H
class Predictor{
	public:
		virtual ~Predictor();
		virtual Sequence* Predict(Sequence*) = 0;
		virtual uint64_t size() = 0;
		std::string getTAG();
	protected:
		virtual bool Train(std::vector<Sequence*>) = 0;
		std::string TAG;
		uint64_t support;
		Predictor();
};
#endif