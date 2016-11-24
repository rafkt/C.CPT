#include "Sequence.h"
#include <vector>
#include <string>

#ifndef DATABASEHELPER_H
#define DATABASEHELPER_H

class DatabaseHelper{
	public:
		enum Format{SPMF, TXT};
		DatabaseHelper(std::string, Format); //use as  DatabaseHelper db(fileName, DatabaseHelper::SPMF);
		~DatabaseHelper();
		std::vector<Sequence*> getDatabase();
	private:
		void loadSPMFFormat(std::string);
		void loadTXTFormat(std::string);
		std::string getFullPath(std::string);
		std::vector<Sequence*> _database;
};

#endif