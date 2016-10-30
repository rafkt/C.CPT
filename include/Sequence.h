#include <vector>

#ifndef SEQUENCE_H
#define SEQUENCE_H
class Sequence
{
	public:
		Sequence();
		Sequence(std::vector<uint64_t>);
		~Sequence();
		void print();
		uint64_t* getItems();
		uint64_t size();
	private:
		uint64_t* sequenceItems;
		uint64_t _size;
};
#endif