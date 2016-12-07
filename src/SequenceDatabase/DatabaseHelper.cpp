#include "../../include/DatabaseHelper.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>


using namespace std;


DatabaseHelper::DatabaseHelper(string filename, Format fm, Profile* pf){
	switch (fm){
		case SPMF:
			loadSPMFFormat(filename, pf->paramInt("sequenceMinSize"));
			break;
		case TXT:
			loadTXTFormat(filename, pf->paramInt("sequenceMinSize"));
			break;
	}
	sigmaIndex();
}
DatabaseHelper::~DatabaseHelper(){
	for (uint64_t i = 0; i < _database.size(); i++) delete _database[i];	
}
vector<Sequence*>& DatabaseHelper::getDatabase(){
	return _database;
}
void DatabaseHelper::loadSPMFFormat(string filename, uint64_t seq_length){
	ifstream file(getFullPath(filename));
	string line;
	while(getline(file, line)){
		stringstream linestream(line);
		vector<uint64_t> tmp_v;
		for(string word; linestream >> word;){
			uint64_t tmp = atoi(word.c_str());
			if (tmp == -1 || tmp == -2) continue;
			tmp_v.push_back(tmp);
		}
		Sequence* s = new Sequence(tmp_v);
		_database.push_back(s);
	}
}
void DatabaseHelper::loadTXTFormat(string filename, uint64_t seq_length){
	ifstream file(getFullPath(filename));
	string line;
	while(getline(file, line)){
		stringstream linestream(line);
		vector<uint64_t> tmp_v;
		for(string word; linestream >> word;){
			uint64_t tmp = atoi(word.c_str());
			tmp_v.push_back(tmp);
		}
		Sequence* s = new Sequence(tmp_v);
		_database.push_back(s);
	}
}
void DatabaseHelper::sigmaIndex(){
	for (Sequence* s : _database){
		uint64_t* items = s->getItems();
		for (uint64_t i = 0; i < s->size(); i++) {
			mapSigmaIndex.insert({items[i], 0});
			if (items[i] == 0){ cerr << "Input error: 0/-1 are not a valid aplhabet items."; exit(1);}
		}
	}
	uint64_t counter = 0;
	for (map<uint64_t, uint64_t>::iterator it = mapSigmaIndex.begin(); it != mapSigmaIndex.end(); it++) it->second = counter++;
	//for(pair<uint64_t, uint64_t> i : mapSigmaIndex) cout << i.first << ": " << i.second << endl;
}
std::map<uint64_t, uint64_t>& DatabaseHelper::getSigmaIndex(){
	return mapSigmaIndex;
}
string DatabaseHelper::getFullPath(string filename){
	return "./Datasets/" + filename;
}


// int main(){
// 	DatabaseHelper db("BIBLE.txt", DatabaseHelper::TXT);
// 	for(uint64_t i = 0; i < db.getDatabase().size(); i++){
// 		db.getDatabase()[i]->print();
// 	}
	
// 	return 0;
// }


