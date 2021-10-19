#pragma once

#include <iostream>
#include <sstream>
#include <regex>
#include <vector>
#include <map>
#include <unordered_map>
#include <stack>
#include <cassert>
#include <algorithm>

using namespace std;

namespace XenonCode {

const size_t MAX_TEXT_LENGTH = 256;

enum class WORD_TYPE : uint8_t {
	VOID = 0,

	FILE,
	LINE,
	GOTO,
	GOTO_IF,
	GOTO_IF_NOT,
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
	
	ROM_CONST_NUMERIC,
	ROM_CONST_TEXT,
	
	RAM_VAR_NUMERIC,
	RAM_VAR_TEXT,
	RAM_ARRAY_NUMERIC,
	RAM_ARRAY_TEXT,
	RAM_DATA,
	
};

class ByteCode {
	union {
		uint32_t rawValue;
		struct {
			uint32_t value : 24; // 16 million possible values per type
			uint32_t type : 8; // 256 possible types
		};
	};
public:
	ByteCode(uint32_t rawValue = 0) : rawValue(rawValue) {}
	ByteCode(uint8_t type, uint32_t value) : value(value), type(type) {}
	
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
	uint32_t initSize = 0; // number of byte codes in the init code (uint32_t)
	uint32_t programSize = 0; // number of byte codes in the program code (uint32_t)
	vector<string> storageRefs {}; // storage references (line)
	unordered_map<string, uint32_t> functionRefs {}; // function references (line)
	
	// ROM
	vector<ByteCode> rom_init {}; // the bytecode of this computer's init function
	vector<ByteCode> rom_program {}; // the actual bytecode of this program
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
			if (filetype != parserFiletype) throw runtime_error("Bad XenonCode assembly");
			s >> version;
			if (version > parserVersion) throw runtime_error("XenonCode file version is more recent than this parser");
			
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

bool isoperator(int c) {
	switch (c) {
		case '=':
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
		case '^':
		case '>':
		case '<':
		case '&':
		case '|':
		case '!':
		case '~':
		case '?':
			return true;
		default: return false;
	}
}
bool isalpha_(int c) {
	return isalpha(c) || c == '_';
}
bool isalnum_(int c) {
	return isalnum(c) || c == '_';
}

struct Word {
	string word = "";
	
	enum Type {
		// Intermediate types
		Empty, // nothing or comment or \n
		Tab, // \t
		Expression, // ( )
		Invalid,
		
		// Final types
		Numeric, // 0.0
		Text, // ""
		Varname, // $
		Funcname, // @
		Operator, // +
		ExpressionBegin, // (
		ExpressionEnd, // )
		Comma, // ,
		Trail, // .
		Cast, // :
		Name, // any other alphanumeric word that starts with alpha
	} type = Empty;
	
	void Clear() {
		word = "";
		type = Empty;
	}
	
	operator bool() const {
		return type != Empty && type != Invalid;
	}
	operator double() const {
		if (type == Numeric)
			return stod(word);
		return 0;
	}
	operator const string&() const {
		return word;
	}
	
	bool operator == (Type t) const {
		return type == t;
	}
	bool operator == (const string& str) const {
		return word == str;
	}
	bool operator == (const char* str) const {
		return word == str;
	}
	bool operator != (Type t) const {return !(*this == t);}
	bool operator != (const string& str) const {return !(*this == str);}
	bool operator != (const char* str) const {return !(*this == str);}
	
	Word(Type type, string word = "") : word(word), type(type) {}
	
	Word(const Word& other) = default;
	Word(Word&& other) = default;
	
	Word(istringstream& s) {
		for (int c; (c = s.get()) != -1; ) if (c != ' ') {
			switch (c) {
				case ',':{
					word = c;
					type = Comma;
				}return;
				case '.':{
					word = c;
					type = Trail;
				}return;
				case ':':{
					word = c;
					type = Cast;
				}return;
				case '\t':{
					word = c;
					type = Tab;
				}return;
				// Variables and Constants
				case '$':{
					if (isalpha_(s.peek())) {
						do {
							word += tolower(s.get());
						} while (!s.eof() && isalnum_(s.peek()));
						type = Varname;
					}
				}return;
				// User-defined Functions
				case '@':{
					if (isalpha_(s.peek())) {
						do {
							word += tolower(s.get());
						} while (!s.eof() && isalnum_(s.peek()));
						type = Funcname;
					}
				}return;
				// Text literals
				case '"':{
					while (!s.eof()) {
						if (s.peek() == '"') {
							s.get();
							break;
						}
						if (s.peek() == '\\') s.get();
						word += s.get();
					}
					type = Text;
				}return;
				// Expressions
				case '(':{
					int stack = 0;
					bool inString = false;
					while (!s.eof()) {
						if (inString) {
							if (s.peek() == '\\') {
								word += s.get();
								if (s.peek() == '"') {
									word += s.get();
								}
							} else if (s.peek() == '"') {
								inString = false;
							}
						} else {
							if (s.peek() == '(') {
								stack++;
							} else if (s.peek() == ')') {
								if (stack-- == 0) {
									s.get();
									break;
								}
							} else if (s.peek() == '"') {
								inString = true;
							}
						}
						word += s.get();
					}
					type = Expression;
				}return;
			}
			// Comments
			if (c == ';' || (c == '/' && s.peek() == '/')) {
				s = {};
				return;
			}
			// Operators
			if (isoperator(c)) {
				word += c;
				if (isoperator(s.peek())) {
					word += s.get();
				}
				type = Operator;
				return;
			}
			// Numeric
			if (isdigit(c)) {
				word += c;
				bool hasDecimal = false;
				while (!s.eof()) {
					if (s.peek() == '.') {
						if (hasDecimal) break;
						hasDecimal = true;
						word += s.get();
					}
					if (!isdigit(s.peek())) break;
					word += s.get();
				}
				type = Numeric;
				return;
			}
			// Name
			if (isalpha_(c)) {
				word += tolower(c);
				while (isalnum_(s.peek())) {
					word += tolower(s.get());
				}
				type = Name;
				return;
			}
			// Invalid
			word = c;
			type = Invalid;
			return;
		}
	}
};

ostream& operator << (ostream& s, const Word& w) {
	return s << string(w);
}

class ParseError : public runtime_error {
	using runtime_error::runtime_error;
};

void ParseWords(const string& str, vector<Word>& words, int& scope) {
	istringstream line(str);
	while (Word word {line}) {
		if (word == Word::Tab) {
			if (words.size() == 0) {
				++scope;
			}
		} else if (word == Word::Expression) {
			words.push_back(Word::ExpressionBegin);
			ParseWords(word, words, scope);
			words.push_back(Word::ExpressionEnd);
		} else if (word == Word::Invalid) {
			throw ParseError("Invalid character '" + string(word) + "'");
		} else {
			words.push_back(word);
		}
	}
}

const vector<string> globalScopeFirstWords {
	"include",
	"const",
	"var",
	"array",
	"storage",
	"init",
	"tick",
	"function",
	"timer",
	"input",
};

const vector<string> functionScopeFirstWords {
	"var",
	"array",
	"output",
	"foreach",
	"repeat",
	"while",
	"break",
	"next",
	"if",
	"elseif",
	"else",
	"return",
};

struct ParsedLine {
	int scope = 0;
	vector<Word> words {};
	
	ParsedLine(const string& str) {
		ParseWords(str, words, scope);
		if (words.size() > 0) {
			// Global Scope?
			if (scope == 0) {
				if (words[0] != Word::Name || find(begin(globalScopeFirstWords), end(globalScopeFirstWords), words[0]) == end(globalScopeFirstWords)) {
					throw ParseError("Invalid first word '" + string(words[0]) + "' in the global scope");
				}
			} else {
				if (words[0] != Word::Varname && words[0] != Word::Funcname && (words[0] != Word::Name || find(begin(functionScopeFirstWords), end(functionScopeFirstWords), words[0]) == end(functionScopeFirstWords))) {
					throw ParseError("Invalid first word '" + string(words[0]) + "' in a function scope");
				}
			}
			//TODO validate the line and add expressions around math operations
		}
	}
};

struct SourceFile {
	vector<ParsedLine> lines;

	SourceFile(istream& stream) {
		int lineNumber = 1;
		int scope = 0;
		
		// Parse all lines
		for (string lineStr; getline(stream, lineStr); lineNumber++) {
			try {
				auto& line = lines.emplace_back(lineStr);
				if (line.scope > scope + 1) {
					throw ParseError("Too many leading tabs");
				}
				scope = line.scope;
				if (line.words.size() == 0) {
					lines.pop_back();
				}
			} catch (ParseError& e) {
				cout << e.what() << " at line " << lineNumber;
				return;
			}
		}
	}
	
	void DebugParsedLines() {
		for (auto& line : lines) {
			
			cout << "Scope{" << line.scope << "} ";
			
			for (auto& word : line.words) {
				switch (word.type) {
					
					case Word::Numeric:
						cout << "Numeric{" << word << "} ";
						break;
					case Word::Text:
						cout << "Text{" << word << "} ";
						break;
					case Word::Varname:
						cout << "Varname{" << word << "} ";
						break;
					case Word::Funcname:
						cout << "Funcname{" << word << "} ";
						break;
					case Word::Operator:
						cout << "Operator{" << word << "} ";
						break;
					case Word::ExpressionBegin:
						cout << "Expression{ ";
						break;
					case Word::ExpressionEnd:
						cout << "} ";
						break;
					case Word::Comma:
						cout << "Comma{" << word << "} ";
						break;
					case Word::Trail:
						cout << "Trail{" << word << "} ";
						break;
					case Word::Cast:
						cout << "Cast{" << word << "} ";
						break;
					case Word::Name:
						cout << "Name{" << word << "} ";
						break;
						
					default: assert(!"Not supposed to happen");
				}
			}
			
			cout << '\n';
		}
	}
};

}
