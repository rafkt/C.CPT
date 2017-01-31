#include <unordered_map>
#include "Sequence.h"

#ifndef FIFRAW_H
#define FIFRAW_H
class FIFRaw{
	public:
		FIFRaw();
		~FIFRaw();
		float memoryInMB();
		std::vector<std::vector<uint64_t>> findFrequentItemsets(std::vector<Sequence*>, uint64_t, uint64_t, uint64_t);
		std::unordered_map<uint64_t, uint64_t> getItemFrequencies(std::vector<Sequence*>);
	private:
		//std::unordered_map<uint64_t, uint64_t> itemFrequencies; //enable this if you need it
};
#endif