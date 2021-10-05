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

enum class WORD_TYPE : uint8_t { // Maximum of 16
	STATEMENT = 0,
	OPERATOR,
	REF_CONST_NUMERIC,
	REF_CONST_TEXT,
	REF_VAR_NUMERIC,
	REF_VAR_TEXT,
	REF_ARRAY_NUMERIC,
	REF_ARRAY_TEXT,
	REF_SYSTEM_FUNCTION,
	REF_NATIVE_FUNCTION,
	REF_MATH_FUNCTION,
	REF_USER_FUNCTION,
	// 4 remaining
};

class Word {
	union {
		uint32_t rawValue = 0;
		struct {
			uint32_t value : 24; // 16 million possible words (reference index or command index)
			uint32_t type : 4; // 16 types
			uint32_t flags : 3; // 3 flags per type
			uint32_t : 1;
		};
	};
	Word(uint32_t rawValue) : rawValue(rawValue) {}
};

class Program {
	istream& stream;
public:
	Program(istream& stream) : stream(stream) {}
	void Parse() {
		for (string lineStr; getline(stream, lineStr);) {
			istringstream line(lineStr);
			cout << line.str() << "\n";
		}
	}
};

}