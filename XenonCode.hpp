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

enum class CODE_TYPE : uint8_t {
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
	
	CODE_TYPE GetType() const {
		return CODE_TYPE(type);
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
		case '.':
		case ':':
		case ',':
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

class ParseError : public runtime_error {
	using runtime_error::runtime_error;
};

#define THROW_PARSE_ERROR(pre,word,...) throw ParseError(pre " '" + string(word) + "' " __VA_ARGS__);

struct Word {
	string word = "";
	
	enum Type : int {
		// Intermediate types
		Empty = 0, // nothing or comment or \n
		Invalid,
		Tab, // \t
		Expression, // ( )
		Operator, // + - * / ++ -- ....
		
		// Final types
		Numeric = 11, // 0.0
		Text, // ""
		Varname, // $
		Funcname, // @
		Name, // any other alphanumeric word that starts with alpha
		ExpressionBegin, // (
		ExpressionEnd, // )
		// Operators
		TrailOperator = 101, // .
		CastOperator, // :
		SuffixOperatorGroup, // ++ --
		NotOperator, // !
		MulOperatorGroup, // * / ^ %
		AddOperatorGroup, // + -
		CompareOperatorGroup, // < <= > >=
		EqualityOperatorGroup, // == != <>
		AndOperator, // && and
		OrOperator, // || or
		AssignmentOperatorGroup, // = += -= *= /= ^= %=
		CommaOperator // ,
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
					} else throw ParseError("Invalid var/const name");
				}return;
				// User-defined Functions
				case '@':{
					if (isalpha_(s.peek())) {
						do {
							word += tolower(s.get());
						} while (!s.eof() && isalnum_(s.peek()));
						type = Funcname;
					} else throw ParseError("Invalid function name");
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
			THROW_PARSE_ERROR("Invalid character", word)
		} else {
			if (word == Word::Operator) {
				if (word == ".")
					word.type = Word::TrailOperator;
				else if (word == ":")
					word.type = Word::CastOperator;
				else if (word == "++" || word == "--")
					word.type = Word::SuffixOperatorGroup;
				else if (word == "!")
					word.type = Word::NotOperator;
				else if (word == "*" || word == "/" || word == "^" || word == "%")
					word.type = Word::MulOperatorGroup;
				else if (word == "+" || word == "-")
					word.type = Word::AddOperatorGroup;
				else if (word == "<" || word == ">" || word == "<=" || word == ">=")
					word.type = Word::CompareOperatorGroup;
				else if (word == "==" || word == "!=" || word == "<>")
					word.type = Word::EqualityOperatorGroup;
				else if (word == "&&")
					word.type = Word::AndOperator;
				else if (word == "||")
					word.type = Word::OrOperator;
				else if (word == "=" || word == "+=" || word == "-=" || word == "*=" || word == "/=" || word == "^=" || word == "%=")
					word.type = Word::AssignmentOperatorGroup;
				else if (word == ",")
					word.type = Word::CommaOperator;
				else THROW_PARSE_ERROR("Invalid Operator", word)
			} else if (word == Word::Name) {
				if (word == "and")
					word.type = Word::AndOperator;
				else if (word == "or")
					word.type = Word::OrOperator;
			}
			words.push_back(word);
		}
	}
}

// Returns the position to continue, or -1 if reached the end of the line. This function also ensures that the returned value is less than the word count.
int ParseExpression(vector<Word>& words, int startAt) {
	if (words.size() == startAt+1 && (words[startAt] == Word::Numeric || words[startAt] == Word::Text)) {
		// Literal expression, Nothing more to do here
		return -1;
	} else {
		//TODO Validate and Encapsulate operations within expressions, recursively
		return -1;
	}
}

// Returns the position to continue, or -1 if reached the end of the line. This function also ensures that the returned value is less than the word count.
int ParseDeclarationArgs(vector<Word>& words, int startAt) {
	// Args MUST be surrounded by parenthesis
	//TODO
	return -1;
}

// Returns the position to continue, or -1 if reached the end of the line. This function also ensures that the returned value is less than the word count.
int ParseArgs(vector<Word>& words, int startAt) {
	// Args MUST be surrounded by parenthesis
	//TODO
	return -1;
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
	
	operator bool () const {
		return words.size() > 0;
	}
	
	ParsedLine(const string& str) {
		ParseWords(str, words, scope);
		
		if (words.size() > 0) {
			
			// Check first word validity for this Scope
			if (scope == 0) { // Global scope
				if (words[0] != Word::Name || find(begin(globalScopeFirstWords), end(globalScopeFirstWords), words[0]) == end(globalScopeFirstWords)) {
					THROW_PARSE_ERROR("Invalid first word", words[0], "in the global scope")
				}
			} else { // Function scope
				if (words[0] != Word::Varname && words[0] != Word::Funcname && (words[0] != Word::Name || find(begin(functionScopeFirstWords), end(functionScopeFirstWords), words[0]) == end(functionScopeFirstWords))) {
					THROW_PARSE_ERROR("Invalid first word", words[0], "in a function scope")
				}
			}
			
			// Validate statement (and insert/modify words to standarize it)
			if (words[0] == Word::Name) {
				
				// Only in Global Scope
				
				// include
					if (words[0] == "include") {
						if (words.size() > 1) {
							if (words[1] == Word::Text) {
								// Valid
							} else if (words[1] == Word::Name) {
								words[1].type = Word::Text;
							} else {
								THROW_PARSE_ERROR("Invalid file name", words[1])
							}
							if (words.size() > 2) {
								throw ParseError("Too many words");
							}
						} else throw ParseError("Too few words");
					} else
					
				// init
					if (words[0] == "init") {
						if (words.size() != 1) throw ParseError("Too many words");
					} else
					
				// tick
					if (words[0] == "tick") {
						if (words.size() != 1) throw ParseError("Too many words");
					} else
					
				// timer
					if (words[0] == "timer") {
						if (words.size() > 1 && words[1] != "frequency" && words[1] != "interval") {
							throw ParseError("Second word must be either 'frequency' or 'interval'");
						}
						if (words.size() > 2 && words[2] != Word::Numeric && words[2] != Word::Varname) {
							throw ParseError("Third word must be either a literal number or a constant name");
						}
						if (words.size() > 3) throw ParseError("Too many words");
						if (words.size() < 3) throw ParseError("Too few words");
					} else
					
				// function
					if (words[0] == "function") {
						if (words.size() > 1 && words[1] != Word::Funcname) {
							throw ParseError("Second word must be a valid function name starting with @");
						}
						if (words.size() > 2 && words[2] != Word::ExpressionBegin) {
							throw ParseError("Function name must be followed by a set of parenthesis, optionally containing an argument list");
						}
						if (words.size() < 4) throw ParseError("Too few words");
						int next = ParseDeclarationArgs(words, 2);
						if (next != -1) {
							if (words.size() == next + 2 && words[next] == Word::CastOperator && (words[next + 1] == "number" || words[next + 1] == "text")) {
								// Valid
							} else {
								throw ParseError("The only thing that can follow a function's argument list is a colon and its return type, which must be either 'number' or 'text'");
							}
						}
					} else
					
				// input
					if (words[0] == "input") {
						if (words.size() > 1 && words[1] != Word::TrailOperator) {
							throw ParseError("The input word must be followed by a dot and the input number");
						}
						if (words.size() > 2 && words[2] != Word::Numeric && words[2] != Word::Varname) {
							throw ParseError("The input number after the dot must be either a literal number or a constant name");
						}
						if (words.size() > 3 && words[3] != Word::ExpressionBegin) {
							throw ParseError("Input number must be followed by a set of parenthesis, optionally containing an argument list");
						}
						if (words.size() < 5) throw ParseError("Too few words");
						if (ParseDeclarationArgs(words, 3) != -1) {
							throw ParseError("Too many words");
						}
					} else
					
				// const
					if (words[0] == "const") {
						if (words.size() >= 4) {
							if (words[1] != Word::Varname) throw ParseError("Invalid constant name (must start with $)");
							if (words[2] != "=") throw ParseError("Invalid const assignment");
							if (ParseExpression(words, 3) != -1) throw ParseError("Invalid expression after const assignment");
						} else throw ParseError("Too few words");
					} else
					
				// storage
					if (words[0] == "storage") {
						if (words.size() > 1 && words[1] != "var" && words[1] != "array") {
							throw ParseError("Second word must be either 'var' or 'array'");
						}
						if (words.size() > 2 && words[2] != Word::Varname
						|| words.size() > 3 && words[3] != Word::CastOperator
						) {
							throw ParseError("Third word must be a variable name (starting with $) followed by a colon and the storage type (number or text)");
						}
						if (words.size() > 4 && words[4] != "number" && words[4] != "text") {
							THROW_PARSE_ERROR("Invalid storage type", words[4], "it must be either 'number' or 'text'")
						}
						if (words.size() > 5 && words[5] == "=") {
							throw ParseError("Cannot initialize storage values here");
						}
						if (words.size() > 5) throw ParseError("Too many words");
						if (words.size() < 5) throw ParseError("Too few words");
					} else
					
				// In both Global and Function scopes
				
				// var
					if (words[0] == "var") {
						if (words.size() > 1 && words[1] != Word::Varname
						 || words.size() < 4
						 || words[2] != "=" && words[2] != Word::CastOperator
						 || words[2] == Word::CastOperator && words[3] != "number" && words[3] != "text"
						 || words[2] == Word::CastOperator && words.size() > 4
						) {
							throw ParseError("Second word must be a variable name (starting with $), and it must be followed either by a colon and its type (number or text) or an equal sign and an expression");
						}
						if (words[2] == "=") {
							if (ParseExpression(words, 3) != -1) {
								throw ParseError("Invalid expression after var assignment");
							}
						}
					} else
					
				// array
					if (words[0] == "array") {
						if (words.size() > 1 && words[1] != Word::Varname
						|| words.size() > 2 && words[2] != Word::CastOperator
						) {
							throw ParseError("Second word must be a variable name (starting with $) followed by a colon and the array type (number or text)");
						}
						if (words.size() > 3 && words[3] != "number" && words[3] != "text") {
							THROW_PARSE_ERROR("Invalid array type", words[3], "it must be either 'number' or 'text'")
						}
						if (words.size() > 4 && words[4] == "=") {
							throw ParseError("Cannot initialize array values here");
						}
						if (words.size() > 4) throw ParseError("Too many words");
						if (words.size() < 4) throw ParseError("Too few words");
					} else
					
				// Only in Function scope
				
				// output
					if (words[0] == "output") {
						if (words.size() > 1 && words[1] != Word::TrailOperator) {
							throw ParseError("Output must be followed by a dot and the output number");
						}
						if (words.size() > 2 && words[2] != Word::Numeric && words[2] != Word::Varname) {
							throw ParseError("Output number must be either a literal number or a constant name starting with $");
						}
						if (words.size() > 3 && words[3] != Word::ExpressionBegin) {
							throw ParseError("Output number must be followed by a set of parenthesis, optionally containing arguments");
						}
						if (words.size() < 5) {
							throw ParseError("Too few words");
						}
						if (ParseArgs(words, 3) != -1) {
							throw ParseError("Invalid argument list after output call");
						}
					} else
					
				// foreach
					if (words[0] == "foreach") {
						if (words.size() > 1 && words[1] != Word::Varname) {
							throw ParseError("Foreach must be followed by an array variable name starting with $");
						}
						if (words.size() > 2 && words[2] != Word::ExpressionBegin
						 || words.size() > 3 && words[3] != Word::Varname
						 || words.size() > 4 && words[4] != Word::ExpressionEnd && words[4] != Word::CommaOperator
						 || words.size() > 5 && words[5] != Word::Varname
						 || words.size() > 6 && words[6] != Word::ExpressionEnd
						) {
							throw ParseError("Foreach must be followed by an array name and a set of parenthesis containing one or two parameters (the item and optionally the index)");
						}
						if (words.size() < 5) {
							throw ParseError("Too few words");
						}
						if (words.size() > 7) {
							throw ParseError("Too many words");
						}
					} else
					
				// repeat
					if (words[0] == "repeat") {
						if (words.size() > 1 && words[1] != Word::Numeric && words[1] != Word::Varname) {
							throw ParseError("Repeat must be followed by either a literal number or a variable name starting with $");
						}
						if (words.size() > 2 && words[2] != Word::ExpressionBegin
						 || words.size() > 3 && words[3] != Word::Varname
						 || words.size() > 4 && words[4] != Word::ExpressionEnd
						) {
							throw ParseError("Repeat must be followed by a number and a set of parenthesis containing one parameter (the index)");
						}
						if (words.size() < 5) {
							throw ParseError("Too few words");
						}
						if (words.size() > 5) {
							throw ParseError("Too many words");
						}
					} else
					
				// while
					if (words[0] == "while") {
						if (words.size() == 1 || ParseExpression(words, 1) != -1) {
							throw ParseError("While must be followed by a boolean expression");
						}
					} else
					
				// break
					if (words[0] == "break") {
						if (words.size() != 1) throw ParseError("Too many words");
					} else
					
				// next
					if (words[0] == "next") {
						if (words.size() != 1) throw ParseError("Too many words");
					} else
					
				// if
					if (words[0] == "if") {
						if (words.size() == 1 || ParseExpression(words, 1) != -1) {
							throw ParseError("If must be followed by a boolean expression");
						}
					} else
					
				// elseif
					if (words[0] == "elseif") {
						if (words.size() == 1 || ParseExpression(words, 1) != -1) {
							throw ParseError("Elseif must be followed by a boolean expression");
						}
					} else
					
				// else
					if (words[0] == "else") {
						if (words.size() != 1) throw ParseError("Too many words");
					} else
					
				// return
					if (words[0] == "return") {
						if (words.size() > 1 && ParseExpression(words, 1) != -1) {
							throw ParseError("Return must be followed by nothing, an expression, a variable name or a literal value");
						}
					} else
					
				THROW_PARSE_ERROR("Invalid word", words[0])
			} else
			
			// Only in Function scope
			
			// Varname
				if (words[0] == Word::Varname) {
					if (words.size() < 2 || words[1] != Word::TrailOperator && words[1] != Word::AssignmentOperatorGroup && words[1] != Word::SuffixOperatorGroup) {
						throw ParseError("A leading variable name must be followed either by a trailing function, an assignment or a suffix operator");
					}
					if (words[1] == Word::AssignmentOperatorGroup) {
						if (words.size() < 3) {
							throw ParseError("Too few words");
						}
						if (ParseExpression(words, 2) != -1) {
							throw ParseError("Invalid expression after var assignment");
						}
					} else if (words[1] == Word::SuffixOperatorGroup) {
						if (words.size() > 2) {
							throw ParseError("Too many words");
						}
					} else if (words[1] == Word::TrailOperator) {
						if (words.size() < 5) {
							throw ParseError("Too few words");
						}
						// Array accessor
						if (words[2] == Word::Numeric || words[2] == Word::Varname) {
							if (words[3] == Word::AssignmentOperatorGroup) {
								if (ParseExpression(words, 4) != -1) {
									throw ParseError("Invalid expression after var assignment");
								}
							} else {
								throw ParseError("Invalid expression after var assignment");
							}
						} else
						// Trailing function
						if (words[2] == Word::Name || words[2] == Word::Funcname) {
							if (words[3] == Word::ExpressionBegin) {
								if (ParseArgs(words, 3) != -1) {
									throw ParseError("Invalid argument list after trailing function on var");
								}
							} else {
								throw ParseError("Invalid expression after trailing function on var");
							}
						} else {
							throw ParseError("Invalid expression after var assignment");
						}
					} else {
						throw ParseError("Invalid expression after var assignment");
					}
				} else
				
			// Funcname
				if (words[0] == Word::Funcname) {
					if (words.size() > 1 && words[1] != Word::ExpressionBegin) {
						throw ParseError("A function call must be followed by a set of parenthesis, optionally containing arguments");
					}
					if (words.size() < 3) {
						throw ParseError("Too few words");
					}
					if (ParseArgs(words, 1) != -1) {
						throw ParseError("Invalid argument list after function call");
					}
				} else
				
			THROW_PARSE_ERROR("Invalid word", words[0])
			
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
			} catch (ParseError& e) {
				cout << e.what() << " at line " << lineNumber << endl;
				return;
			}
		}
	}
	
	void DebugParsedLines() {
		int lineNumber = 1;
		for (auto& line : lines) {
			if (line) {
			
				cout << lineNumber << ": Scope{" << line.scope << "} ";
				
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
						case Word::ExpressionBegin:
							cout << "Expression{ ";
							break;
						case Word::ExpressionEnd:
							cout << "} ";
							break;
						case Word::Name:
							cout << "Name{" << word << "} ";
							break;
						case Word::TrailOperator:
							cout << "TrailOperator{" << word << "} ";
							break;
						case Word::CastOperator:
							cout << "CastOperator{" << word << "} ";
							break;
						case Word::SuffixOperatorGroup:
							cout << "SuffixOperatorGroup{" << word << "} ";
							break;
						case Word::NotOperator:
							cout << "NotOperator{" << word << "} ";
							break;
						case Word::MulOperatorGroup:
							cout << "MulOperatorGroup{" << word << "} ";
							break;
						case Word::AddOperatorGroup:
							cout << "AddOperatorGroup{" << word << "} ";
							break;
						case Word::CompareOperatorGroup:
							cout << "CompareOperatorGroup{" << word << "} ";
							break;
						case Word::EqualityOperatorGroup:
							cout << "EqualityOperatorGroup{" << word << "} ";
							break;
						case Word::AndOperator:
							cout << "AndOperator{" << word << "} ";
							break;
						case Word::OrOperator:
							cout << "OrOperator{" << word << "} ";
							break;
						case Word::AssignmentOperatorGroup:
							cout << "AssignmentOperatorGroup{" << word << "} ";
							break;
						case Word::CommaOperator:
							cout << "CommaOperator{" << word << "} ";
							break;
							
						default: assert(!"Parse Error");
					}
				}
				
				cout << '\n';
			}
			++lineNumber;
		}
	}
};

}

#undef THROW_PARSE_ERROR
