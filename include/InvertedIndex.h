#include "Sequence.h"
#include <vector>
#include <unordered_map>

#ifndef INVERTEDINDEX_H
#define INVERTEDINDEX_H

class InvertedIndex{
	public:
		virtual ~InvertedIndex();
		virtual uint64_t* query(uint64_t*, uint64_t) = 0;
		virtual uint64_t getCardinality(uint64_t) = 0;
	protected:
		InvertedIndex(std::vector<Sequence*>);
		Sequence** II_database;
		uint64_t _size;
		std::unordered_map<uint64_t, std::vector<uint64_t>> alphabet2sequences_table;
		uint64_t Sigma;
};

#endif