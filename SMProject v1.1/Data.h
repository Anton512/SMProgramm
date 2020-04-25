#pragma once
#include <string>
struct Data
{
public:
	Data() {};
	Data(std::string _name, std::string _definition)
	{
		name = _name;
		definition = _definition;
	}

	std::string getName() { return name; }
	std::string getDefinition() { return definition; }
	void setName(std::string str) { name = str; }
	void setDefinition(std::string str) { definition = str; }
private:
	std::string name;
	std::string definition;
	unsigned int id = 0;
};