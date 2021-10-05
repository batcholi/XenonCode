#include <iostream>
#include <fstream>

#include "XenonCode.hpp"

using namespace std;

int main(int argc, char** argv) {
	if (argc == 2) {
		ifstream file{argv[1]};
		XenonCode::Program program(file);
		program.Parse();
	} else {
		cerr << "You must provide a file path to a .xenon file\n";
	}
}
