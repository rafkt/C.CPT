#include "../../../include/Encoder.h"
#include <vector>
#include <deque>
#include <iostream>

Encoder::Encoder(){
}
Encoder::~Encoder(){
}
float Encoder::sizeInMB(){
	return 0.0;
}
void Encoder::clearInvDict(){
	InvDict.clear(); //this is only needed during encoding - no need to keep if we only decode items.
	//Either call after Train is finished or after pathCollapse (if implemented) is finished.
}
// Add an itemset in the dictionary
// return the Id of the itemset
uint64_t Encoder::addEntry(std::vector<uint64_t> entry){
	bool found = false;
	uint64_t id = getId(entry, found);
	if(id == 0 && !found) {
		
		Dict.push_back(entry);
		id = Dict.size() - 1;
		InvDict.insert({entry, id});
	}
	
	return id;
}
// Return the itemset with the given id
std::vector<uint64_t> Encoder::getEntry(uint64_t id){
	return Dict[id];
}
// Return the id of the given itemset
// return the id or null if the itemset is not found
uint64_t Encoder::getId(std::vector<uint64_t> entry, bool& found){
	uint64_t id;
	try {
		id = InvDict.at(entry);      //throws an out-of-range
	}
	catch (const std::out_of_range& oor) {
		found = false;
		return 0;
	}
	found = true;
	return id;
}
// Return the id of an itemset and adds it in the dictionary if needed
uint64_t Encoder::getIdorAdd(std::vector<uint64_t> entry){
	return addEntry(entry);
}
// Encode a sequence by replacing sequential items with known itemsets.
// It always try to use the longuest itemsets possible.
// return A encoded hard copy of the original sequence
Sequence* Encoder::encode(Sequence* seq){
	if(seq == nullptr || seq->size() == 0) {
		return nullptr ;
	}
	
	std::vector<uint64_t> encoded_items;
	uint64_t seqSize = seq->size();

	std::vector<uint64_t> seq_items(seq->getItems(), seq->getItems() + seqSize);
	
	//For each items in the sequence
	for(int i = 0; i < seqSize; i++) {
		
		//Finds the longuest itemset (taking everything from the current item to the end
		//end removing one item at the time (from the end) until it finds a known itemset
		std::vector<uint64_t> candidate (seq_items.begin() + i, seq_items.end());
		bool idFound = false;
		uint64_t id = 0;
		while(!idFound && candidate.size()) {
			idFound = false;
			//if it found a known itemset
			id = getId(candidate, idFound);
			if(idFound) {		
				encoded_items.push_back(id);
				
				i += candidate.size() - 1; 
			}
			//special case when the candidate list has only a single item left
			else if(candidate.size() == 1) {
				id = addEntry(candidate);
				encoded_items.push_back(id);
				idFound = true;
			}
			//removing the last item
			else {
				candidate.pop_back();
			}

		}
	}
	Sequence* encoded = new Sequence(encoded_items);
	return encoded;
}
// Replace each itemset in the sequence with the original sequential items
// return a hard decoded copy of the encoded sequence
Sequence* Encoder::decode(Sequence* seq){
	if(seq == nullptr || seq->size() == 0) {
		return nullptr;
	}
	
	std::vector<uint64_t> decoded_items;
	std::vector<uint64_t> seq_items(seq->getItems(), seq->getItems() + seq->size());

	//for each encoded item, it decodes 
	//it and adds it in the decoded sequence
	for(uint64_t encodedItem : seq_items) {
		
		//if it founds the itemset
		std::vector<uint64_t> itemset = getEntry(encodedItem);
		if(itemset.size()) {
			
			for(uint64_t decodedItem : itemset) {
				decoded_items.push_back(decodedItem);
			}
			
		}
		else {
			std::cerr << "Could not find item: " << encodedItem << std::endl;
		}
	}
	Sequence* decoded = new Sequence(decoded_items);
	return decoded;
}


// int main() {
	
	

	
// 	//Pattern
// 	std::vector<uint64_t> p1({42, 43});
// 	std::vector<uint64_t> p2({42});
// 	std::vector<uint64_t> p3({42, 43, 44});
// 	// p1.add(new Item(42));
// 	// p1.add(new Item(43));
// 	// List<Item> p2 = new LinkedList<Item>();
// 	// p2.add(new Item(42));
// 	// List<Item> p3 = new LinkedList<Item>();
// 	// p3.add(new Item(42));
// 	// p3.add(new Item(43));
// 	// p3.add(new Item(44));
	
// 	//1 2 3 4
// 	std::vector<uint64_t> v({42, 43, 44, 45});
// 	Sequence* seq1 = new Sequence(v);
// 	// seq1.addItem(new Item(42));
// 	// seq1.addItem(new Item(43));
// 	// seq1.addItem(new Item(44));
// 	// seq1.addItem(new Item(45));

// 	Encoder en;
	
// 	en.addEntry(p1);
// 	en.addEntry(p2);
// 	en.addEntry(p3);
	
	
// 	Sequence* encoded = en.encode(seq1);
	
// 	seq1->print();
// 	encoded->print();
// 	en.decode(encoded)->print();
	
// }
