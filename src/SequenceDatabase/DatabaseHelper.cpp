#include "../../include/DatabaseHelper.h"
#include <stdio.h>
#include <iostream>


using namespace std;


DatabaseHelper::DatabaseHelper(string, Format){

}
DatabaseHelper::~DatabaseHelper(){

}
vector<Sequence*> DatabaseHelper::getDatabase(){
	vector<Sequence*> v;
	v.push_back(nullptr);
	return v;
}
vector<Sequence*> DatabaseHelper::loadSPMFFormat(string){
	vector<Sequence*> v;
	v.push_back(nullptr);
	return v;
}
vector<Sequence*> DatabaseHelper::loadQuestFormat(string){
	vector<Sequence*> v;
	v.push_back(nullptr);
	return v;
}
string DatabaseHelper::getFullPath(string){
	return "";
}