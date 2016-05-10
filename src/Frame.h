#include <string>
#include <vector>
#include <map>
#include <iostream>

#pragma once
class Frame
{
public:
	Frame(std::string filename, std::string ext = ".html");
	std::string GetDocument() const;
	Frame operator+(const std::map<std::string, std::string>& vars);
	Frame& operator<<(const std::map<std::string, std::string>& vars);

private:
	std::vector<std::string> frameParts;
	std::vector<std::string> frameVars;
	std::string document = "";
	bool failSwitch = false;
	char* ext;
	void Assemble(const std::map<std::string, std::string>& vars);
};

std::ostream& operator<<(std::ostream& out, const Frame& f);