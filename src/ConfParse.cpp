#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include "ConfParse.h"
#include "Frame.h"

ConfParse::ConfParse(const char* confPath, const char* repoPath, const char* outPath, const char* dPath) {
	std::stringstream dss;
	defs.open(dPath);

	if (defs) {
		dss << defs.rdbuf();
		defs.close();
		boost::property_tree::read_json(dss, defaults);
	}

	rPath = repoPath;
	oPath = outPath;

	rPath += '/';
	oPath += '/';
	
	std::stringstream ss;
	conf.open(confPath);
	if (conf) {
		ss << conf.rdbuf();
		conf.close();
	}

	boost::property_tree::read_json(ss, pt);

	ParseConf(pt);
}


std::string ConfParse::ParseConf(boost::property_tree::ptree &p) {
	//Add defaults
	boost::optional<std::string> addDefaults = p.get_optional<std::string>("-D");
	if (addDefaults) {
		p.erase("-D");
		boost::optional<boost::property_tree::ptree&> defDefined = defaults.get_child_optional(*addDefaults);

		if (defDefined) {
			BOOST_FOREACH(boost::property_tree::ptree::value_type &v, *defDefined) {
				boost::optional<boost::property_tree::ptree&> sourceDef = p.get_child_optional(v.first);
				if (sourceDef) {
					//Is supposed to "push_back" if is array
					if (!sourceDef->empty() && !IsObject(*sourceDef)) {
						if (!v.second.empty() && !IsObject(v.second)) {
							BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, v.second) {
								//sourceDef->push_back(std::make_pair("", v2.second));
								sourceDef->push_back(boost::property_tree::ptree::value_type("", v2.second));
							}
						} else {
							//sourceDef->push_back(std::make_pair("", v.second));
							sourceDef->push_back(boost::property_tree::ptree::value_type("", v.second));
						}
					}
				} else {
					p.put_child(v.first, v.second);
				}
			}
		}

		return ParseConf(p);
	} else {
		//If there's no define frame, the code assumes that this tier defines pages in the form "key.oExt"
		boost::optional<std::string> getFrame = p.get_optional<std::string>("-f");
		if (!getFrame) {
			std::string ret;
			BOOST_FOREACH(boost::property_tree::ptree::value_type &v, p) {
				//Parses the child tree and sends the output to file (html by default) within specified output path
				std::string outFilePath = oPath + v.first + oExt;
				if (!v.second.empty() && IsObject(v.second)) {
					outF.open(outFilePath.c_str());
					if (outF) {
						outF << ParseConf(v.second);
					}
					outF.close();
				}
			}

			return "";
		} else {
			unsigned int mInstCountTmp = 0;
			bool reloop = false, mInst = false;
			std::string fPath = rPath + *getFrame;
			Frame f(fPath.c_str());
			//MultInstMap mInstTmp;
			MultInstMapTest mInstTmp;
			std::map<std::string, std::string> vars;

			BOOST_FOREACH(boost::property_tree::ptree::value_type &v, p) {
				vars.insert(std::pair<std::string, std::string>(v.first, ""));
				if (!v.second.empty()) {
					bool isObj = IsObject(v.second);
					if (isObj) {
						//Object
						vars.at(v.first) = ParseConf(v.second);
					} else if (!isObj) {
						//Array
						std::string key = v.first;
						//mInstTmp.insert(std::pair<std::string, std::vector<std::string>> (key, std::vector<std::string>()));
						mInstTmp.insert(std::pair<std::string, std::vector<boost::property_tree::ptree>> (key, std::vector<boost::property_tree::ptree>()));
						mInst = true;

						BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, v.second) {
							mInstTmp.at(key).push_back(v2.second);
						}

						//Makes sure that once mInstCountTmp is set it doesn't ever exceed the most instances of the object possible
						if (mInstCountTmp == 0 || mInstCountTmp > mInstTmp.at(key).size())
							mInstCountTmp = mInstTmp.at(key).size();
					}
				} else if (v.second.empty()) {
					//Terminal key/value pair
					if (v.second.data() == "-d") {
						reloop = true;
						boost::optional<boost::property_tree::ptree&> defDefined = defaults.get_child_optional(v.first);
						if (!defDefined) {
							p.put(v.first, "");
						} else {
							p.put_child(v.first, *defDefined);
						}
					} else {
						vars.at(v.first) = v.second.data();
					}
				}
			}

			
			if (reloop) {
				return ParseConf(p);
			}

			if (mInst) {
				std::string ret = "";
				for (unsigned int i = 0; i < mInstCountTmp; ++i) {
					boost::property_tree::ptree tmp = p;
					for (MultInstMapTest::iterator it = mInstTmp.begin(); it != mInstTmp.end(); ++it) {
						tmp.put_child(it->first, it->second.at(i));
					}
					ret += ParseConf(tmp);
				}
				return ret;
			} else if (!mInst) {
				f << vars;
				return f.GetDocument();
			}
		}
	}
}

bool ConfParse::IsObject(boost::property_tree::ptree p) {
	BOOST_FOREACH(boost::property_tree::ptree::value_type &v, p) {
		if (v.first.empty()) {
			return false;
		}
	}
	return true;
}