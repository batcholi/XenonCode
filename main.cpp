#define XENONCODE_IMPLEMENTATION
#include "XenonCode.hpp"
#include <thread>
#include <queue>
#include <mutex>
#include <csignal>

using namespace std;

bool verbose = false; // Set using -verbose in the arguments
bool isRunning = true;
int64_t cyclesPerSecond = 0;

void Init() {

	// Define numeric global constants.
	XenonCode::DeclareGlobalConstant("pi", 3.141592653589793238462643);
	XenonCode::DeclareGlobalConstant("2pi", 2 * 3.141592653589793238462643);
	XenonCode::DeclareGlobalConstant("number_one", 1);
	XenonCode::DeclareGlobalConstant("number_two", 2);
	XenonCode::DeclareGlobalConstant("number_three", 3);

	// Define text global constants.
	XenonCode::DeclareGlobalConstant("test_str1", "This is test string 1");
	XenonCode::DeclareGlobalConstant("test_str2", "This is test string 2");

	// Define callable entrypoints.
	XenonCode::DeclareEntryPoint("shutdown");
	
	// Do NOT change the order of declarations once in production, just append new things after the last one. This is because scripts are compiled using the indices, which are based on the order they were declared.
	// Maximum of 127 object types
	auto positionType = XenonCode::DeclareObjectType("position", {
		// Those are the members, may be used either as functions (using args, modifying the object, returning void) or as properties (ignoring args, returning a value)
		{"x:number", [](XenonCode::Computer*, const XenonCode::Var& obj, const vector<XenonCode::Var>& args) -> XenonCode::Var {
			return 0.0;
		}},
		{"y:number", [](XenonCode::Computer*, const XenonCode::Var& obj, const vector<XenonCode::Var>& args) -> XenonCode::Var {
			return 0.0;
		}},
		{"z:number", [](XenonCode::Computer*, const XenonCode::Var& obj, const vector<XenonCode::Var>& args) -> XenonCode::Var {
			return 0.0;
		}},
		{"normalize()", [](XenonCode::Computer*, const XenonCode::Var& obj, const vector<XenonCode::Var>& args) -> XenonCode::Var {
			//...
			return {};
		}},
	});
	
	// Maximum of 65k global device functions
	XenonCode::DeclareDeviceFunction("delta:number", [](XenonCode::Computer*, const vector<XenonCode::Var>& args) -> XenonCode::Var {
		return 0.0;
	});
	XenonCode::DeclareDeviceFunction("print", [](XenonCode::Computer*, const vector<XenonCode::Var>& args) -> XenonCode::Var {
		for (const auto& arg : args) {
			cout << string(arg) << endl;
		}
		return {};
	});
	
	// This is an example of a constructor for the object type "position" (defined as a global device function)
	XenonCode::DeclareDeviceFunction("position:position", [=](XenonCode::Computer*, const vector<XenonCode::Var>& args) -> XenonCode::Var {
		return {positionType, 0/*This would be an implementation-defined object pointer (uint64_t) that must be mapped somewhere*/};
	});
	
	// Must set the output function here
	XenonCode::SetOutputFunction([](XenonCode::Computer*, uint32_t ioIndex, const vector<XenonCode::Var>& args){
		if (verbose) {
			cout << "output." << ioIndex << "\n";
		}
		for (const auto& arg : args) {
			if (verbose) {
				cout << '\t';
			}
			cout << string(arg) << endl;
		}
		if (verbose) cout << endl;
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
	cout << "  xenoncode [-verbose] -parse-line-generic 'a line of code'" << endl;
	cout << "    Parses a single line of code and check for syntax errors, without verifying the validity of device-specific objects/functions/entrypoints" << endl;
	cout << endl;
	cout << "  xenoncode [-verbose] -compile <sourcedir>" << endl;
	cout << "    Parse and Compile a program from a given directory" << endl;
	cout << "    There must be a 'main.xc' present" << endl;
	cout << "    It compiles into '" << XC_PROGRAM_EXECUTABLE << "' in that same given directory" << endl;
	cout << endl;
	cout << "  xenoncode [-verbose] [-hz <NCyclesPerSecond>] -run <sourcedir>" << endl;
	cout << "    Run a program from a given directory" << endl;
	cout << "    There must be a '" << XC_PROGRAM_EXECUTABLE << "' present" << endl;
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

bool ParseLine(const string& line, bool generic = false) {
	try {
		if (verbose) {
			cout << line << endl;
		}
		XenonCode::ParsedLine src(line, 0, generic);
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
		if (XenonCode::Computer::CompileAssembly(directory, mainFile.lines, verbose)) {
			if (verbose) {
				cout << "\nCompiled Successfully!\n" << endl;
			}
			return true;
		}
	} catch (XenonCode::CompileError& e) {
		cerr << e.what() << endl;
	}
	return false;
}

void InteruptSignalHandler(int signum) {
	isRunning = false;
}

bool Run(const string& directory) {
	XenonCode::Computer computer;
	computer.capability.ram = 65536;
	if (computer.LoadProgram(directory)) {
		computer.LoadStorage(directory + "/storage");
		try {
			if (computer.RunInit()) {
				computer.SaveStorage(directory + "/storage");
				if (cyclesPerSecond && computer.ShouldRunContinuously()) {
					
					{// Signal Handler
					#ifdef _WIN32
						signal(SIGINT, InteruptSignalHandler);
					#else
						struct sigaction _sa;
						_sa.sa_handler = InteruptSignalHandler;
						sigemptyset(&_sa.sa_mask);
						_sa.sa_flags = 0;
						sigaction(SIGINT, &_sa, NULL);
					#endif
					}
					
					std::mutex inputMutex;
					std::queue<std::string> inputQueue;
					std::thread inputThread;
					if (computer.HasInputs()) {
						inputThread = std::thread{[&](){
							std::string input;
							while (isRunning && std::getline(std::cin, input)) {
								if (input == "exit") break;
								if (input != "") {
									std::lock_guard lock(inputMutex);
									inputQueue.emplace(input);
								}
							}
							isRunning = false;
						}};
					}
					while (isRunning) {
						std::this_thread::sleep_for(std::chrono::milliseconds(1000 / std::min(int64_t(1000), cyclesPerSecond)));
						computer.RunCycle();
						{
							std::lock_guard lock(inputMutex);
							if (!inputQueue.empty()) {
								computer.RunInput(0, {inputQueue.front()});
								inputQueue.pop();
							}
						}
						computer.SaveStorage(directory + "/storage");
					}
					if (inputThread.joinable()) {
						inputThread.detach();
					}
					if (verbose) {
						std::cout << "Program terminated gracefully" << std::endl;
					}
				} else {
					if (verbose) {
						cout << "Program's Init function successfully executed\n" << endl;
					}
				}
				computer.RunEntryPoint("shutdown");
				return true;
			}
		} catch (XenonCode::RuntimeError& e) {
			cerr << e.what() << endl;
		}
	} else {
		cerr << "Cannot run this program" << endl;
	}
	return false;
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
				// Parse Single Line at a time, without verifying the validity of device-specific objects, functions and entry points
				else if (arg == "parse-line-generic") {
					if (!ParseLine(nextArgStr(), true)) return 1;
				}
				// Compile (using a directory)
				else if (arg == "compile") {
					string directory = nextArgStr();
					if (directory == "") {
						cerr << "You must provide a path to a directory containing the source files of the program to compile" << endl;
					}
					if (!Compile(directory)) return 1;
				}
				// HZ (Set Cycles Per Second)
				else if (arg == "hz") {
					cyclesPerSecond = nextArgInt();
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
