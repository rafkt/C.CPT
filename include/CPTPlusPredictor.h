#include "CPTPredictor.h"
#include "FIFRaw.h"
#include "Encoder.h"

#ifndef CPTPLUSPREDICTOR_H
#define CPTPLUSPREDICTOR_H
class CPTPlusPredictor : public CPTPredictor{
	public:
		CPTPlusPredictor(std::vector<Sequence*>, Profile*);
		virtual ~CPTPlusPredictor();
		virtual float memoryInMB();
	private:
		virtual bool Train(std::vector<Sequence*>);
		virtual std::vector<uint64_t> getBranch(uint64_t);
		void pathCollapse();
		Encoder* encoder;
};
#endif