#include "Frame.h"
#include "ConfParse.h"

//conf.json /path/to/HTML/repo/ /path/to/output/site/ defaults.json (optional)
int main(int argc, char * argv[]) {
	if (argc < 4) {
		return 1;
	} else if (argc == 4) {
		ConfParse c(argv[1], argv[2], argv[3]);
	} else if (argc == 5) {
		ConfParse c(argv[1], argv[2], argv[3], argv[4]);
	}
	return 0;
}