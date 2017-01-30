#include "../../include/FIFRaw.h"
#include "../../include/Sequence.h"
#include <map>
#include <iostream>

using namespace std;

FIFRaw::FIFRaw(){}

/**
 * Return all the consecutive items (length between [minLength,maxlength] ) found in the 
 * given sequences with a high enough support (support >= minSup)
 * @param minLength Minimum length for the itemsets
 * @param maxlength Maximum length for the itemsets
 */

vector<vector<uint64_t>> FIFRaw::findFrequentItemsets(vector<Sequence*> seqs, uint64_t minLength, uint64_t maxlength, uint64_t minSup){
	
	vector<vector<uint64_t>> frequents;
	map<vector<uint64_t>, uint64_t> frequencies;
	
	if(maxlength <= 1 || minLength > maxlength) {
		return frequents;
	}
	
	//Calculating frequencies by iterating through each sequence
	for(Sequence* seq : seqs) {
		
		if(seq->size() >= minLength) {
			
			for(uint64_t i = 0; i < seq->size() - 1; i++) {
				
				//Calculate the frequencies of itemsets of size in range [minLength, maxlength]
				vector<uint64_t> itemset;
				for(uint64_t offset = i; (offset - i) < maxlength && offset < seq->size(); offset++) {
					
					//adding one item at the time to the itemset
					itemset.push_back(seq->getItems()[offset]);
					
					//saving the frequency of itemset if it is long enough
					if(itemset.size() >= minLength) {
						
						//Updating the frequency of this itemset
						uint64_t support = 0;
						if(frequencies.find(itemset) != frequencies.end()) support = frequencies[itemset];
						frequencies.insert({itemset, support + 1});
						for (uint64_t i : itemset) cout << i << " ";
						cout << " : " << support + 1 << endl;
					}
				}
				
				uint64_t support = 0;
				if(itemFrequencies.find(seq->getItems()[i]) != itemFrequencies.end()) support = itemFrequencies[seq->getItems()[i]];
				support++;
				itemFrequencies.insert({seq->getItems()[i], support});
			}
		}
	}
	
	for (map<vector<uint64_t>, uint64_t>::iterator it = frequencies.begin(); it != frequencies.end(); it++)
		if (it->second >= minSup) frequents.push_back(it->first);
	
	return frequents;
}
unordered_map<uint64_t, uint64_t> FIFRaw::getItemFrequencies(vector<Sequence*> seqs){
	return itemFrequencies;
}

	
	
int main() {
		
		
	//Training sequences
	std::vector<Sequence*> training;
//		//1 2 3 4
	std::vector<uint64_t> v1({1, 2, 3, 4});
	Sequence* seq1 = new Sequence(v1);
	training.push_back(seq1);
	
	//1 2 3 4
	std::vector<uint64_t> v2({1, 2, 3, 4});
	Sequence* seq2 = new Sequence(v2);
	training.push_back(seq2);
	
// 	//1 2 3 4
// 	std::vector<uint64_t> v3({1, 2, 3, 4});
// 	Sequence* seq3 = new Sequence(v3);
// 	training.push_back(seq3);
	
// //		//0 1 2 4
// 	std::vector<uint64_t> v4({0, 1, 2, 4});
// 	Sequence* seq4 = new Sequence(v4);
// 	training.push_back(seq4);
	
	
	FIFRaw finder;
	std::vector<std::vector<uint64_t>> v = finder.findFrequentItemsets(training, 2, 4, 2);
	for(std::vector<uint64_t> l : v){
		for(uint64_t i : l) cout << i << " ";
		cout << endl;
	}
	}