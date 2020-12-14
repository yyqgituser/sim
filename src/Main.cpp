#include <memory>
#include <iostream>

#include "Compiler.h"


int main(int argc, char *argv[]) {
	if (argc < 2) {
		cout << "usage: simc <filename>" << endl;
		return 1;
	}
	string fileName = argv[1];
	size_t dotIndex = fileName.rfind('.');
	if (dotIndex == string::npos || fileName.substr(dotIndex) != ".sim") {
		cout << "error: name of source file must be ended with '.sim'" << endl;
		return 1;
	}
	Compiler compiler;
	if (!compiler.compile(fileName)) {
		return 1;
	}
	return 0;
}

