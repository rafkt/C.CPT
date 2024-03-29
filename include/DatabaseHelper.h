#include "Sequence.h"
#include "Profile.h"
#include <vector>
#include <string>
#include <map>

#ifndef DATABASEHELPER_H
#define DATABASEHELPER_H

class DatabaseHelper{
	public:
		enum Format{SPMF, TXT};
		DatabaseHelper(std::string, Format,  Profile*); //use as  DatabaseHelper db(fileName, DatabaseHelper::SPMF);
		~DatabaseHelper();
		std::vector<Sequence*>& getDatabase();
		int getLength();
	private:
		void loadSPMFFormat(std::string, uint64_t);
		void loadTXTFormat(std::string, uint64_t);
		std::string getFullPath(std::string);
		std::vector<Sequence*> _database;
		int _length;
};

#endif