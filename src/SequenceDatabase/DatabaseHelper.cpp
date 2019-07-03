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
	//std::random_shuffle(_database.begin(), _database.end());
}
DatabaseHelper::~DatabaseHelper(){
	for (uint64_t i = 0; i < _database.size(); i++) delete _database[i];	
}
vector<Sequence*>& DatabaseHelper::getDatabase(){
	return _database;
}
void DatabaseHelper::loadSPMFFormat(string filename, uint64_t seq__length, uint64_t seq__number){
	ifstream file(getFullPath(filename));
	string line;
	_length = 0;
	while(getline(file, line)){
		stringstream linestream(line);
		vector<uint64_t> tmp_v;
		for(string word; linestream >> word;){
			uint64_t tmp = atoi(word.c_str());
			if (tmp == -1 || tmp == -2) continue;
			tmp_v.push_back(tmp);
			_length++;
		}
		Sequence* s = new Sequence(tmp_v);
		_database.push_back(s);
		seq__number--;
		if (seq__number >= 0) if (seq__number == 0) break;
	}
	cout << "Overall dataset length: " << _length << endl;
}
void DatabaseHelper::loadTXTFormat(string filename, uint64_t seq__length, uint64_t seq__number){
	ifstream file(getFullPath(filename));
	string line;
	_length = 0;
	while(getline(file, line)){
		stringstream linestream(line);
		vector<uint64_t> tmp_v;
		for(string word; linestream >> word;){
			uint64_t tmp = atoi(word.c_str());
			tmp_v.push_back(tmp);
			_length++;
		}
		Sequence* s = new Sequence(tmp_v);
		_database.push_back(s);
		if (seq__number  >= 0) if (seq__number == 0) break;
	}
	cout << "Overall dataset length: " << _length << endl;
}
string DatabaseHelper::getFullPath(string filename){
	return "./datasets/" + filename;
}

int DatabaseHelper::getLength(){return _length;}

// int main(){
// 	DatabaseHelper db("BIBLE.txt", DatabaseHelper::TXT);
// 	for(uint64_t i = 0; i < db.getDatabase().size(); i++){
// 		db.getDatabase()[i]->print();
// 	}
	
// 	return 0;
// }


