#include "../../../include/SD_CPTPredictor.h"
//#include "../../../include/DatabaseHelper.h"
using namespace std;

SD_CPTPredictor::SD_CPTPredictor(vector<Sequence*> trainingSequences, Profile* profile): CPTPredictor(trainingSequences, profile){

	sd_tree = new CPT_SD_Tree(root, mapSigmaIndex, nodeNumber);

	LT_SD = new uint64_t[trainingSequences.size()];
	for (uint64_t i = 0; i < trainingSequences.size(); i ++) LT_SD[i] = LT[i]->id;

	//after sucessfull construction, delete CPTPredictor structures
	deleteTrie(root);
	//delete II;
	delete[] LT;
	root = nullptr; 
	//II = nullptr; 
	LT = nullptr;
}
SD_CPTPredictor::~SD_CPTPredictor(){
	delete sd_tree; sd_tree = nullptr;
	delete II; II = nullptr;
}

float SD_CPTPredictor::memoryInMB(){
	//to implemented after II and LT finalisation
	// sd_tree->get_memory_in_mega_bytes() + II_EF
	// LT should be part of II_EF so no need to add any extra calculation
	return sd_tree->memoryInMB() + II->memoryInMB() + ((8 + 8 * trainingSequenceNumber) * 8 * 1.25 * pow(10, -7));
}

vector<uint64_t> SD_CPTPredictor::getBranch(uint64_t index){
	std::vector<uint64_t> branch = sd_tree->getNodesToRoot(LT_SD[index]);

	reverse(branch.begin(), branch.end());
	// for (uint64_t i : branch) cout << i << " ";
	// cout << endl;

	return branch;
}

vector<uint64_t> SD_CPTPredictor::getMatchingSequences(Sequence* target){
	//find all sequences that have all the target's items
	//for each item in the target sequence
	uint64_t* items = target->getItems();
	vector<uint64_t> intersection = II->query_(items, target->size());
	vector<uint64_t> indexes;
	return intersection;
}

void SD_CPTPredictor::createII(){
	II = new EF_II_bit_vector(newTrainingSet, LT_SD, nodeNumber);
	//delete newTrainingSet since it is not longer needed
	for(uint64_t i = 0; i < newTrainingSet.size(); i++) delete newTrainingSet[i];
	//delete[] LT_SD; LT_SD = nullptr;
	
	cout << "Total CPT SD size in megabytes: " << memoryInMB() << endl;
	cout << "-----------------------------------" << endl;
	cout << "II size in megabytes (bv length is node number): " << II->memoryInMB() << endl;;
	cout << "SD tree (bitstring) size in megabytes: " << sd_tree->memoryInMB() << endl;
	//cout << "LT size in megabytes: 0, LT is now contained in II" << endl;
	cout << "LT size in megabytes: " << ((8 + 8 * trainingSequenceNumber) * 8 * 1.25 * pow(10, -7)) <<endl;
}

bool SD_CPTPredictor::visited(set<uint64_t>& hashSidVisited, uint64_t index){
	if(hashSidVisited.find(index) != hashSidVisited.end()) return true;
	return false;
}
void SD_CPTPredictor::gotVisit(set<uint64_t>& hashSidVisited, uint64_t index){
	hashSidVisited.insert(index);
}


// int main(){
// 	Profile* pf = new Profile();
// 	pf->apply();
// 	DatabaseHelper* db = new DatabaseHelper("BIBLE.txt", DatabaseHelper::TXT, pf);
// 	Predictor* cpt_pr = new CPTPredictor(db->getDatabase(), pf);
// 	cpt_pr->createII();

// 	 vector<uint64_t> v = {356, 122};
// 	 Sequence* target = new Sequence(v);
// 	 Sequence* predicted = cpt_pr->Predict(target);
// 	 cout << "Prtedicted: " << endl;
// 	 predicted->print();
// 	 cout << endl;
// 	 cout << cpt_pr->memoryInMB() << endl;
// 	// cout << "Memory size of Predictor: " << cpt_pr->memoryInMB() << "MB";

// 	 delete cpt_pr;
// 	 delete pf;
// 	 delete db;
// 	 delete target;
// 	 delete predicted;
// }
