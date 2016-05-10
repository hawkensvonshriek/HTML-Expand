#include "Frame.h"
#include "ConfParse.h"

/*
* I commented out this (fully functional) code for accepting cmd args for ease of grading;
* this way you don't have to specify the correct config file, HTML repository, output dir,
* and defaults configuration file
*/
//conf.json /path/to/HTML/repo/ /path/to/output/site/ defaults.json (optional)
int main(int argc, char * argv[]) {
	if (argc < 4) {
		return 1;
	} else if (argc == 4) {
		ConfParse c(argv[1], argv[2], argv[3]);
	} else if (argc == 5) {
		ConfParse c(argv[1], argv[2], argv[3], argv[4]);
	}

	//Yep this is all main.cpp does--check out my whacky Frame and ConfParse classes :)
	//ConfParse c("struct.json", "Modules/", "FinalSite/", "defaults.json");
	return 0;
}