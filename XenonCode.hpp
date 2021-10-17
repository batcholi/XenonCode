#pragma once

#include <iostream>
#include <sstream>
#include <regex>
#include <vector>
#include <map>
#include <unordered_map>
#include <stack>

using namespace std;

namespace XenonCode {

const size_t MAX_TEXT_LENGTH = 256;

enum class WORD_TYPE : uint8_t {
	VOID = 0,

	LINE,
	GOTO,
	RETURN,
	NUMBER_OPERATION,
	TEXT_OPERATION,
	COMPARE,
	INPUT_FUNCTION,
	OUTPUT_FUNCTION,
	SYSTEM_FUNCTION,
	DEVICE_FUNCTION,
	MATH_FUNCTION,
	USER_FUNCTION,

	STORAGE_VAR_NUMERIC,
	STORAGE_VAR_TEXT,
	STORAGE_ARRAY_NUMERIC,
	STORAGE_ARRAY_TEXT,
	GLOBAL_CONST_NUMERIC,
	GLOBAL_CONST_TEXT,
	GLOBAL_VAR_NUMERIC,
	GLOBAL_VAR_TEXT,
	GLOBAL_ARRAY_NUMERIC,
	GLOBAL_ARRAY_TEXT,
	LOCAL_VAR_NUMERIC,
	LOCAL_VAR_TEXT,
	LOCAL_VAR_DATA,
	LOCAL_ARRAY_NUMERIC,
	LOCAL_ARRAY_TEXT,
	TMP_VAR_NUMERIC,
	TMP_VAR_TEXT,
	TMP_DATA,
	ARG_NUMERIC,
	ARG_TEXT,
	ARG_DATA,
	
};

class Word {
	union {
		uint32_t rawValue;
		struct {
			uint32_t value : 24; // 16 million possible values per type
			uint32_t type : 8; // 256 possible types
		};
	};
public:
	Word(uint32_t rawValue = 0) : rawValue(rawValue) {}
	Word(uint8_t type, uint32_t value) : value(value), type(type) {}
	
	WORD_TYPE GetType() const {
		return WORD_TYPE(type);
	}
	uint32_t GetValue() const {
		return value;
	}
};

class Assembly {
	const string parserFiletype = "XenonCode!";
	const uint32_t parserVersion = 0;
	
public:
	uint32_t initSize = 0; // number of words in the init code (uint32_t)
	uint32_t programSize = 0; // number of words in the program code (uint32_t)
	vector<string> storageRefs {}; // storage references (line)
	unordered_map<string, uint32_t> functionRefs {}; // function references (line)
	
	// ROM
	vector<Word> rom_init {}; // the bytecode of this computer's init function
	vector<Word> rom_program {}; // the actual bytecode of this program
	vector<double> rom_numericConstants {}; // the actual numeric constant values
	vector<string> rom_textConstants {}; // the actual text constant values
	
	// RAM size
	uint32_t ram_numericVariables = 0;
	uint32_t ram_textVariables = 0;
	uint32_t ram_dataReferences = 0;
	uint32_t ram_numericArrays = 0;
	uint32_t ram_textArrays = 0;

	Assembly() {}
	explicit Assembly(istream& s) {Read(s);}

	void Write(ostream& s) {
		{// Write Header
			// Write assembly file info
			s << parserFiletype << ' ' << parserVersion << '\n';
			
			// Write some sizes
			s << initSize << ' ' << programSize << ' ' << storageRefs.size() << ' ' << functionRefs.size() << '\n';
			s << rom_numericConstants.size() << ' ';
			s << rom_textConstants.size() << '\n';
			s << ram_numericVariables << ' ';
			s << ram_textVariables << ' ';
			s << ram_dataReferences << ' ';
			s << ram_numericArrays << ' ';
			s << ram_textArrays << '\n';
			
			// Write memory references
			for (auto& name : storageRefs) {
				s << name << '\n';
			}
			
			// Write function references
			for (auto&[name,addr] : functionRefs) {
				s << name << ' ' << addr << '\n';
			}
			
			// Write Rom data (constants)
			for (auto& value : rom_numericConstants) {
				s << value << '\n';
			}
			for (auto& value : rom_textConstants) {
				s << value << '\0';
			}
		}
		
		// Write init bytecode
		s.write((char*)rom_init.data(), initSize * sizeof(uint32_t));
		
		// Write program bytecode
		s.write((char*)rom_program.data(), programSize * sizeof(uint32_t));
	}

private:
	void Read(istream& s) {
		string filetype;
		uint32_t version;
		size_t storageRefsSize;
		size_t functionRefsSize;
		size_t rom_numericConstantsSize;
		size_t rom_textConstantsSize;

		{// Read Header
			// Read assembly file info
			s >> filetype;
			if (filetype != parserFiletype) throw std::runtime_error("Bad XenonCode assembly");
			s >> version;
			if (version > parserVersion) throw std::runtime_error("XenonCode file version is more recent than this parser");
			
			// Read some sizes
			s >> initSize >> programSize >> storageRefsSize >> functionRefsSize;
			s >> rom_numericConstantsSize;
			s >> rom_textConstantsSize;
			s >> ram_numericVariables;
			s >> ram_textVariables;
			s >> ram_dataReferences;
			s >> ram_numericArrays;
			s >> ram_textArrays;

			// Prepare data
			rom_init.resize(initSize);
			rom_program.resize(programSize);
			rom_numericConstants.reserve(rom_numericConstantsSize);
			rom_textConstants.reserve(rom_textConstantsSize);
			storageRefs.reserve(storageRefsSize);
			functionRefs.reserve(functionRefsSize);

			// Read memory references
			for (size_t i = 0; i < storageRefsSize; ++i) {
				string name;
				s >> name;
				storageRefs.push_back(name);
			}

			// Read function references
			for (size_t i = 0; i < functionRefsSize; ++i) {
				string name;
				uint32_t address;
				s >> name >> address;
				functionRefs.emplace(name, address);
			}
			
			// Read Rom data (constants)
			for (size_t i = 0; i < rom_numericConstantsSize; ++i) {
				double value;
				s >> value;
				rom_numericConstants.push_back(value);
			}
			for (size_t i = 0; i < rom_textConstantsSize; ++i) {
				char value[MAX_TEXT_LENGTH];
				s.getline(value, MAX_TEXT_LENGTH, '\0');
				rom_textConstants.push_back(value);
			}
		}

		// Read init bytecode
		s.read((char*)rom_init.data(), initSize * sizeof(uint32_t));

		// Read program bytecode
		s.read((char*)rom_program.data(), programSize * sizeof(uint32_t));
	}
};

class SourceFile {
	istream& stream;
public:
	explicit SourceFile(istream& stream) : stream(stream) {}
	void Parse() {
		for (string lineStr; getline(stream, lineStr);) {
			istringstream line(lineStr);
			cout << line.str() << "\n";
		}
	}
};

}
