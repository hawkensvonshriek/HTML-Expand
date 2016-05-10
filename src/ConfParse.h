#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include <fstream>
#include <string>

#pragma once
typedef std::map<std::string, std::vector<std::string>> MultInstMap;
typedef std::map<std::string, std::vector<boost::property_tree::ptree>> MultInstMapTest;

class ConfParse
{
public:
	ConfParse(const char* confPath, const char* repoPath, const char* outPath, const char* dPath = "defaults.json");
	std::string ParseConf(boost::property_tree::ptree &p);
private:
	boost::property_tree::ptree pt;
	boost::property_tree::ptree defaults;
	std::ifstream conf;
	std::ifstream defs;
	std::ofstream outF;
	std::string oExt = ".html";
	std::string rPath;
	std::string oPath;

	bool IsObject(boost::property_tree::ptree p);
};