#include "XenonCode.hpp"

using namespace std;

bool verbose = false; // Set using -verbose in the arguments

const string outputFileName = "xc_program.bin";

void Init() {
	XenonCode::DeclareDeviceFunction("delta () : number", [](const vector<XenonCode::Var>& args) -> XenonCode::Var {
		return {};
	});
}

void PrintVersion() {
	cout << "XenonCode parser/compiler tool version " << XenonCode::VERSION_MAJOR << "." << XenonCode::VERSION_MINOR << "." << XenonCode::VERSION_PATCH << endl;
}

void PrintUsage() {
	cout << "Usage:" << endl;
	cout << endl;
	cout << "  xenoncode -help" << endl;
	cout << "    Prints this usage information" << endl;
	cout << endl;
	cout << "  xenoncode -version" << endl;
	cout << "    Prints version information" << endl;
	cout << endl;
	cout << "  xenoncode [-verbose] -parse-file <sourcefilepath>" << endl;
	cout << "    Parses a file and check for syntax errors" << endl;
	cout << endl;
	cout << "  xenoncode [-verbose] -parse-line 'a line of code'" << endl;
	cout << "    Parses a single line of code and check for syntax errors" << endl;
	cout << endl;
	cout << "  xenoncode [-verbose] -compile <sourcedir>" << endl;
	cout << "    Parse and Compile a program from a given directory" << endl;
	cout << "    There must be a 'main.xc' present" << endl;
	cout << "    It compiles into '" << outputFileName << "' in that same given directory" << endl;
	cout << endl;
	cout << "  xenoncode [-verbose] -run <sourcedir>" << endl;
	cout << "    Run a program from a given directory" << endl;
	cout << "    There must be a '" << outputFileName << "' present" << endl;
	cout << "    This will only run the body of the init function" << endl;
	cout << endl;
	cout << "All commands may be used multiple times and in conjunction with one another, in the order they will be executed. The program will stop on the first error." << endl;
	cout << endl;
}

bool ParseFile(const string& filepath) {
	try {
		XenonCode::SourceFile src(filepath);
		if (verbose) {
			src.DebugParsedLines();
		}
		return true;
	} catch (XenonCode::ParseError& err) {
		cerr << err.what() << " in " << filepath << endl;
	}
	return false;
}

bool ParseLine(const string& line) {
	try {
		if (verbose) {
			cout << line << endl;
		}
		XenonCode::ParsedLine src(line);
		if (verbose) {
			XenonCode::DebugWords(src.words);
			cout << endl;
		}
		return true;
	} catch (XenonCode::ParseError& err) {
		cerr << err.what() << endl;
	}
	return false;
}

bool Compile(const string& directory) {
	try {
		auto mainFile = XenonCode::GetParsedFile(directory, "main.xc");
		if (verbose) {
			mainFile.DebugParsedLines();
			cout << "Compiling..." << endl;
		}
		XenonCode::Assembly assembly {mainFile.lines, verbose};
		ofstream output(directory + "/" + outputFileName, ios_base::out | ios_base::trunc);
		assembly.Write(output);
		if (verbose) {
			cout << "\nCompiled Successfully!\n" << endl;
		}
		return true;
	} catch (XenonCode::ParseError& e) {
		cerr << e.what() << endl;
	}
	return false;
}

bool Run(const string& directory) {
	XenonCode::Computer computer;
	computer.LoadProgram(directory);
	return computer.RunInit();
}

int main(const int argc, const char** argv) {
	Init();
	if (argc > 1) {
		for (int i = 1; i < argc; ++i) {
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
					PrintVersion();
				}
				// Help
				if (arg == "help") {
					PrintUsage();
				}
				// Verbose
				else if (arg == "verbose") {
					verbose = true;
				}
				// Parse File
				else if (arg == "parse-file") {
					string filepath = nextArgStr();
					if (filepath == "") {
						cerr << "You must provide a file path to a .xc file to parse" << endl;
						return 1;
					}
					if (!ParseFile(filepath)) return 1;
				}
				// Parse Single Line at a time
				else if (arg == "parse-line") {
					if (!ParseLine(nextArgStr())) return 1;
				}
				// Compile (using a directory)
				else if (arg == "compile") {
					string directory = nextArgStr();
					if (directory == "") {
						cerr << "You must provide a path to a directory containing the source files of the program to compile" << endl;
					}
					if (!Compile(directory)) return 1;
				}
				// Run (using a directory)
				else if (arg == "run") {
					string directory = nextArgStr();
					if (directory == "") {
						cerr << "You must provide a path to a directory containing the XenonCode program to run" << endl;
					}
					if (!Run(directory)) return 1;
				}
				// Invalid
				else {
					cerr << "Invalid option: " << string(argv[i]) << endl;
					return 1;
				}
			}
		}
	} else {
		PrintVersion();
		cout << endl;
		PrintUsage();
		return 1;
	}
}
