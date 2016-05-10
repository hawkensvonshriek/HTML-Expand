#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include "Frame.h"

Frame::Frame(std::string filename, std::string ext) {
	std::string filePath = filename + ext;
	std::ifstream in(filePath.c_str());
	if (in) {
		std::string key;
		std::string content(	(std::istreambuf_iterator<char>(in)),
								(std::istreambuf_iterator<char>()		));

		int ind = 0;

		while (content.find("{{", ind) != std::string::npos) {
			frameParts.push_back(content.substr(ind, content.find("{{", ind) - ind));

			key = content.substr(content.find("{{", ind) + 2, content.find("}}", ind) - (content.find("{{", ind) + 2));
			frameVars.push_back(key);

			ind = content.find("}}", ind) + 2;
		}
		frameParts.push_back(content.substr(ind, content.length() - ind));
	} else {
		failSwitch = true;
	}
}

std::string Frame::GetDocument() const {
	return document;
}


void Frame::Assemble(const std::map<std::string, std::string>& vars) {
	if(failSwitch) {
		document = "";
		return;
	} else {
		if (document.length() > 0)
			document += '\n';

		if (frameParts.size() > 1) {
			for (unsigned int i = 0; i < frameParts.size() - 1; ++i)
			{
				document += frameParts.at(i) + ((vars.count(frameVars.at(i)) > 0) ? vars.at(frameVars.at(i)) : "");
			}

			document += frameParts.back();
		} else {
			document += frameParts.at(0);
		}
		return;
	}
}

Frame Frame::operator+(const std::map<std::string, std::string>& vars) {
	this->Assemble(vars);
	return *this;
}

Frame& Frame::operator<<(const std::map<std::string, std::string>& vars) {
	this->Assemble(vars);
	return *this;
}

std::ostream& operator<<(std::ostream& out, const Frame& f) {
	return out << f.GetDocument();
}