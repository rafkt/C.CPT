#include "../../include/DatabaseHelper.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>


using namespace std;


DatabaseHelper::DatabaseHelper(string filename, Format fm){

}
DatabaseHelper::~DatabaseHelper(){

}
vector<Sequence*> DatabaseHelper::getDatabase(){
	return _database;
}
void DatabaseHelper::loadSPMFFormat(string filename){
	ifstream file(getFullPath(filename));
	string line;
	while(getline(file, line)){
		stringstream linestream(line);
		vector<uint64_t> tmp_v;
		for(string word; linestream >> word;){
			uint64_t tmp = atoi(word.c_str());
			//reading from quest datasets
			if (tmp == -1 || tmp == -2) continue;
			tmp_v.push_back(tmp);
		}
		Sequence* s = new Sequence(tmp_v);
		_database.push_back(s);
	}
}
string DatabaseHelper::getFullPath(string filename){
	return "./Datasets/" + filename;
}