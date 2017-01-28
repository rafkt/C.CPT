#ifndef ENCODER_H
#define ENCODER_H

#include <vector>
#include <map>
#include "Sequence.h"
class Encoder{
	public:
		Encoder();
		uint64_t addEntry(std::vector<uint64_t>);
		std::vector<uint64_t> getEntry(uint64_t);
		uint64_t getId(std::vector<uint64_t>, bool&);
		uint64_t getIdorAdd(std::vector<uint64_t>);
		Sequence* encode(Sequence*);
		Sequence* decode(Sequence*);
	private:
		std::vector<std::vector<uint64_t>> Dict;
		std::map<std::vector<uint64_t>, uint64_t> InvDict;
};

#endif