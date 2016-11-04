#include <unordered_map>
#include <string>

#ifndef PROFILE_H
#define PROFILE_H
class Profile{
	public:
		Profile();
		~Profile();
		double paramDouble(std::string);
		int paramInt(std::string);
		float paramFloat(std::string);
		bool paramBool(std::string);
		void apply();
	private:
		std::unordered_map<std::string, std::string> parameters;
};
#endif