#include "../../include/Profile.h"
#include <iostream>
using namespace std;

Profile::Profile(){
}
Profile::~Profile(){
}
double Profile::paramDouble(string name){
	return stod(parameters[name]);
}
int Profile::paramInt(string name){
	return stoi(parameters[name]);
}
float Profile::paramFloat(string name){
	return stof(parameters[name]);
}
bool Profile::paramBool(string name){
	return parameters[name] == "true";
}
void Profile::apply(){
	//Global parameters
		//Pre-processing
		parameters.insert(make_pair("sequenceMinSize", "6"));
		parameters.insert(make_pair("sequenceMaxSize", "999"));
		parameters.insert(make_pair("removeDuplicatesMethod", "1"));
		parameters.insert(make_pair("consequentSize", "1")); 
		parameters.insert(make_pair("windowSize", "5")); 
		
		parameters.insert(make_pair("predictionsNumber", "2"));//raf
		parameters.insert(make_pair("stepsToFlushCache", "10"));//raf

		///////////////
		//CPT parameters
		//Training
		parameters.insert(make_pair("splitMethod", "0")); //0 for no split", "1 for basicSplit", "2 for complexSplit
		parameters.insert(make_pair("splitLength", "999")); // max tree height

		//Prediction
		parameters.insert(make_pair("recursiveDividerMin", "4")); //should be >= 0 and < recursiveDividerMax 
		parameters.insert(make_pair("recursiveDividerMax", "99")); //should be > recusiveDividerMax and < windowSize
		parameters.insert(make_pair("minPredictionRatio", "2.0f")); //should be over 0
		parameters.insert(make_pair("noiseRatio", "1.0f")); //should be in the range ]0,1]

		//best prediction from the count table
		parameters.insert(make_pair("firstVote", "1")); //1 for confidence", "2 for lift
		parameters.insert(make_pair("secondVote", "2")); //0 for none", "1 for support", "2 for lift
		parameters.insert(make_pair("voteTreshold", "0.0")); //confidence threshold to validate firstVote", "else it uses the secondVote 

		//Countable weight system
		parameters.insert(make_pair("countTableWeightMultiplier", "2")); // 0 for no weight (1)", "1 for 1/targetSize", "2 for level/targetSize
		parameters.insert(make_pair("countTableWeightDivided", "1")); // 0 for no divider", "1 for x/(#ofBranches for this sequence)

		//Others
		parameters.insert(make_pair("useHashSidVisited", "true"));
		parameters.insert(make_pair("branchTraversalTopToBottom", "true")); //used for branches with duplicates", "set to true to allow with duplicates
		parameters.insert(make_pair("removeUnknownItemsForPrediction", "true")); //remove items that were never seen before from the Target sequence before LLCT try to make a prediction

}