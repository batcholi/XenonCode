#include <iostream>
#include <fstream>

#include "XenonCode.hpp"

using namespace std;

int main(int argc, char** argv) {
	if (argc == 2) {
		try {
			ifstream file{argv[1]};
			XenonCode::SourceFile src(file);
			src.DebugParsedLines();
		} catch (XenonCode::ParseError& err) {
			cerr << err.what() << " in " << argv[1] << endl;
		}
	} else {
		cerr << "You must provide a file path to a .xc file\n";
	}
}
