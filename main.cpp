#include <iostream>
#include <fstream>

#include "XenonCode.hpp"

using namespace std;

int main(const int argc, const char** argv) {
	if (argc > 1) for (int i = 1; i < argc; ++i) {
		if (*argv[i] == '-') {
			string arg = *(argv[i]+1)=='-'? (argv[i] + 2) : (argv[i] + 1); // also handle --
			auto nextArgStr = [&i, &argc, &argv](const string& defaultValue = "") -> string {
				return ++i < argc? string(argv[i]) : defaultValue;
			};
			auto nextArgInt = [&i, &argc, &argv](const int& defaultValue = 0){
				return ++i < argc? atoi(argv[i]) : defaultValue;
			};
			// Version
			if (arg == "version") {
				cout << "XenonCode parser/compiler tool version " << XenonCode::VERSION_MAJOR << "." << XenonCode::VERSION_MINOR << "." << XenonCode::VERSION_PATCH << endl;
				return 0;
			}
			// Parse
			else if (arg == "parse-debug" || arg == "parse") {
				string filepath = nextArgStr();
				if (filepath == "") {
					cerr << "You must provide a file path to a .xc file to parse" << endl;
					return 0;
				}
				try {
					ifstream file{filepath};
					XenonCode::SourceFile src(file);
					if (arg == "parse-debug") {
						src.DebugParsedLines();
					}
					return 0;
				} catch (XenonCode::ParseError& err) {
					cerr << err.what() << " in " << filepath << endl;
					return 1;
				}
			}
			// Invalid
			else {
				cerr << "Invalid option: " << string(argv[i]) << endl;
				return 1;
			}
		}
	}
	
	// Usage
	cout << "Usage:" << endl;
	cout << "    xenoncode -parse <sourcefilepath>" << endl;
	cout << "    xenoncode --parse-debug <sourcefilepath>" << endl;
	return 1;
}
