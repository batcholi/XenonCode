#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <stack>
#include <cassert>
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <functional>
#include <cstring>

#ifndef XC_NAMESPACE
	#define XC_NAMESPACE XenonCode
#endif

#pragma region UNDEFS // Microsoft's C++ not respecting the standard again...
#ifdef VOID
	#undef VOID
#endif
#ifdef DISCARD
	#undef DISCARD
#endif
#ifdef RETURN
	#undef RETURN
#endif
#ifdef OP
	#undef OP
#endif
#ifdef INTEGER
	#undef INTEGER
#endif
#ifdef ADDR
	#undef ADDR
#endif
#ifdef SET
	#undef SET
#endif
#ifdef ADD
	#undef ADD
#endif
#ifdef SUB
	#undef SUB
#endif
#ifdef MUL
	#undef MUL
#endif
#ifdef DIV
	#undef DIV
#endif
#ifdef MOD
	#undef MOD
#endif
#ifdef POW
	#undef POW
#endif
#ifdef CCT
	#undef CCT
#endif
#ifdef AND
	#undef AND
#endif
#ifdef ORR
	#undef ORR
#endif
#ifdef XOR
	#undef XOR
#endif
#ifdef EQQ
	#undef EQQ
#endif
#ifdef NEQ
	#undef NEQ
#endif
#ifdef LST
	#undef LST
#endif
#ifdef GRT
	#undef GRT
#endif
#ifdef LTE
	#undef LTE
#endif
#ifdef GTE
	#undef GTE
#endif
#ifdef INC
	#undef INC
#endif
#ifdef DEC
	#undef DEC
#endif
#ifdef NOT
	#undef NOT
#endif
#ifdef FLR
	#undef FLR
#endif
#ifdef CIL
	#undef CIL
#endif
#ifdef RND
	#undef RND
#endif
#ifdef SIN
	#undef SIN
#endif
#ifdef COS
	#undef COS
#endif
#ifdef TAN
	#undef TAN
#endif
#ifdef ASI
	#undef ASI
#endif
#ifdef ACO
	#undef ACO
#endif
#ifdef ATA
	#undef ATA
#endif
#ifdef ABS
	#undef ABS
#endif
#ifdef FRA
	#undef FRA
#endif
#ifdef SQR
	#undef SQR
#endif
#ifdef SIG
	#undef SIG
#endif
#ifdef LOG
	#undef LOG
#endif
#ifdef CLP
	#undef CLP
#endif
#ifdef STP
	#undef STP
#endif
#ifdef SMT
	#undef SMT
#endif
#ifdef LRP
	#undef LRP
#endif
#ifdef NUM
	#undef NUM
#endif
#ifdef TXT
	#undef TXT
#endif
#ifdef DEV
	#undef DEV
#endif
#ifdef OUT
	#undef OUT
#endif
#ifdef APP
	#undef APP
#endif
#ifdef CLR
	#undef CLR
#endif
#ifdef POP
	#undef POP
#endif
#ifdef ASC
	#undef ASC
#endif
#ifdef DSC
	#undef DSC
#endif
#ifdef INS
	#undef INS
#endif
#ifdef DEL
	#undef DEL
#endif
#ifdef FLL
	#undef FLL
#endif
#ifdef FRM
	#undef FRM
#endif
#ifdef SIZ
	#undef SIZ
#endif
#ifdef LAS
	#undef LAS
#endif
#ifdef MIN
	#undef MIN
#endif
#ifdef MAX
	#undef MAX
#endif
#ifdef AVG
	#undef AVG
#endif
#ifdef SUM
	#undef SUM
#endif
#ifdef MED
	#undef MED
#endif
#ifdef SBS
	#undef SBS
#endif
#ifdef IDX
	#undef IDX
#endif
#ifdef JMP
	#undef JMP
#endif
#ifdef GTO
	#undef GTO
#endif
#ifdef CND
	#undef CND
#endif
#ifdef FND
	#undef FND
#endif
#ifdef CON
	#undef CON
#endif
#ifdef KEY
	#undef KEY
#endif
#ifdef STR
	#undef STR
#endif
#ifdef RST
	#undef RST
#endif
#ifdef HSH
	#undef HSH
#endif
#ifdef UPP
	#undef UPP
#endif
#ifdef LCC
	#undef LCC
#endif
#ifdef ISN
	#undef ISN
#endif
#ifdef IFF
	#undef IFF
#endif
#pragma endregion

namespace XC_NAMESPACE {

// Version
const int VERSION_MAJOR = 0; // Requires assembly compiled with the same major version
const int VERSION_MINOR = 1; // Requires assembly compiled with a version <= than interpreter's minor version
const int VERSION_PATCH = 0;

#pragma region Limitations/Settings // these are default values, may be overridden by the implementation

	#ifndef XC_APP_NAME
		#define XC_APP_NAME "DEFAULT_XC_APP" // MUST be set by the implementation
	#endif
	#ifndef XC_APP_VERSION
		#define XC_APP_VERSION 1
	#endif
	#ifndef XC_PROGRAM_EXECUTABLE
		#define XC_PROGRAM_EXECUTABLE "xc_program.bin"
	#endif
	#ifndef XC_MAX_TEXT_LENGTH
		#define XC_MAX_TEXT_LENGTH 4096 // max number of chars in text variables (absolute maximum is 16M)
	#endif
	#ifndef XC_MAX_STORAGE_MEMORY_SIZE
		#define XC_MAX_STORAGE_MEMORY_SIZE 100'000'000u // max storage memory size in bytes
	#endif
	#ifndef XC_MAX_ARRAY_SIZE
		#define XC_MAX_ARRAY_SIZE 65535 // max number of elements in arrays (absolute maximum is 16M)
	#endif
	#ifndef XC_MAX_ROM_SIZE
		#define XC_MAX_ROM_SIZE 0xFFFFFF // number of 32-bit words (absolute maximum is 16M)
	#endif
	#ifndef XC_TEXT_MEMORY_PENALTY
		#define XC_TEXT_MEMORY_PENALTY 16 // memory usage multiplier for text variables
	#endif
	#ifndef XC_ARRAY_NUMERIC_MEMORY_PENALTY
		#define XC_ARRAY_NUMERIC_MEMORY_PENALTY 32 // memory usage multiplier for an array of numbers
	#endif
	#ifndef XC_ARRAY_TEXT_MEMORY_PENALTY
		#define XC_ARRAY_TEXT_MEMORY_PENALTY 512 // memory usage multiplier for an array of text
	#endif
	#ifndef XC_OBJECT_MEMORY_PENALTY
		#define XC_OBJECT_MEMORY_PENALTY 16 // memory usage multiplier for objects
	#endif
	#ifndef XC_MAX_CALL_DEPTH
		#define XC_MAX_CALL_DEPTH 16
	#endif
	#ifndef XC_RECURSIVE_MEMORY_PENALTY
		#define XC_RECURSIVE_MEMORY_PENALTY 16
	#endif

#pragma endregion

#pragma region Helper Functions
	
	#define EPSILON_DOUBLE 0.0000001

	inline static double step(double edge1, double edge2, double val) {
		if (edge1 > edge2) {
			if (val >= edge2 && val <= edge1) {
				return 1.0;
			}
		} else {
			if (val >= edge1 && val <= edge2) {
				return 1.0;
			}
		}
		return 0.0;
	}
	
	inline static double step(double edge, double val) {
		if (val >= edge) {
			return 1.0;
		}
		return 0.0;
	}
	
	inline static double smoothstep(double edge1, double edge2, double val) {
		if (edge1 == edge2) return 0.0;
		val = std::clamp((val - edge1) / (edge2 - edge1), 0.0, 1.0);
		return val * val * val * (val * (val * 6 - 15) + 10);
	}
	
	inline static void strtolower(std::string& str) {
		std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return std::tolower(c); });
	}
	
	inline static void strtoupper(std::string& str) {
		std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return std::toupper(c); });
	}
	
	inline static size_t utf8length(const std::string& str) {
		size_t len = 0;
		for (size_t i = 0; i < str.length(); ++i) {
			if ((str[i] & 0xC0) != 0x80) {
				++len;
			}
		}
		return len;
	}
	
	inline static std::string utf8substr(const std::string& str, size_t start, int length = -1) {
		if (length < 0) {
			length = utf8length(str) - start;
		}
		if (start == 0 && length == 0) {
			return "";
		}
		size_t len = 0;
		size_t i = 0;
		for (; i < str.length(); ++i) {
			if ((str[i] & 0xC0) != 0x80) {
				if (len == start) {
					break;
				}
				++len;
			}
		}
		size_t j = i;
		for (; j < str.length(); ++j) {
			if ((str[j] & 0xC0) != 0x80) {
				if (len == start + size_t(length)) {
					break;
				}
				++len;
			}
		}
		return str.substr(i, j-i);
	}
	
	inline static void utf8assign(std::string& str, size_t start, const std::string& substr) {
		size_t substrLen = utf8length(substr);
		size_t len = 0;
		size_t i = 0;
		for (; i < str.length(); ++i) {
			if ((str[i] & 0xC0) != 0x80) {
				if (len == start) {
					break;
				}
				++len;
			}
		}
		size_t j = i;
		for (; j < str.length(); ++j) {
			if ((str[j] & 0xC0) != 0x80) {
				if (len == start + substrLen) {
					break;
				}
				++len;
			}
		}
		str.replace(i, j-i, substr);
	}
	
#pragma endregion

#pragma region Errors

	struct ParseError : public std::runtime_error {
		using std::runtime_error::runtime_error;
		ParseError(const std::string& pre, const std::string& word) : runtime_error(pre + " '" + word + "'") {}
		ParseError(const std::string& pre, const std::string& word, const std::string& post) : runtime_error(pre + " '" + word + "' " + post) {}
	};

	struct CompileError : public std::runtime_error {
		using std::runtime_error::runtime_error;
		CompileError(const std::string& pre, const std::string& word) : runtime_error(pre + " '" + word + "'") {}
		CompileError(const std::string& pre, const std::string& word, const std::string& post) : runtime_error(pre + " '" + word + "' " + post) {}
	};

	struct RuntimeError : public std::runtime_error {
		using std::runtime_error::runtime_error;
	};

#pragma endregion

#pragma region Parser

	inline static bool isoperator(int c) {
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
	inline static bool isalpha_(int c) {
		return isalpha(c) || c == '_';
	}
	inline static bool isalnum_(int c) {
		return isalnum(c) || c == '_';
	}

	inline static std::string ToString(double value) {
		std::ostringstream oss;
		oss << std::fixed << value;
		std::string str = oss.str();
		size_t dotPos = str.find('.');
		if (dotPos != std::string::npos) {
			str.erase(str.find_last_not_of('0') + 1, std::string::npos);
			if (str.back() == '.') {
				str.pop_back();
			}
		}
		return str;
	}

	inline static const int WORD_ENUM_FINAL_TYPE_START = 11;
	inline static const int WORD_ENUM_OPERATOR_START = 101;

	struct Word {
		std::string word = "";
		
		enum Type : int {
			// Intermediate types
			Empty = 0, // nothing or comment or \n
			Invalid,
			Tab, // \t
			Expression, // ( )
			Operator, // + - * / ++ -- ....
			
			FileInfo, // will contain the name of the current file, for error handling purposes
			
			// Final types starting here
			Numeric = WORD_ENUM_FINAL_TYPE_START, // 0.0
			Text, // ""
			Varname, // $
			Funcname, // @
			Name, // any other alphanumeric word that starts with alpha
			ExpressionBegin, // (
			ExpressionEnd, // )
			HashTag, // #
			Void, // placeholder for operators with only two parameters
			
			// Operators (by order of precedence)
			TrailOperator = WORD_ENUM_OPERATOR_START, // .
			NamespaceOperator, // ::
			CastOperator, // :
			ConcatOperator, // & (text concatenation operator)
			SuffixOperatorGroup, // ++ -- !!
			NotOperator, // !
			MulOperatorGroup, // ^ % * /
			AddOperatorGroup, // + -
			CompareOperatorGroup, // < <= > >=
			EqualityOperatorGroup, // == != <>
			AndOperator, // && and
			OrOperator, // || or
			XorOperator,
			AssignmentOperatorGroup, // = += -= *= /= ^= %= &=
			CommaOperator, // ,
			
			MAX_ENUM
		} type = Empty;
		
		void Clear() {
			word = "";
			type = Empty;
		}
		
		operator bool() const {
			return type != Empty;
		}
		operator double() const {
			if (type == Numeric)
				return stod(word);
			return 0;
		}
		operator const std::string&() const {
			return word;
		}
		
		bool operator == (Type t) const {
			return type == t;
		}
		bool operator == (const std::string& str) const {
			return word == str;
		}
		bool operator == (const char* str) const {
			return word == str;
		}
		bool operator != (Type t) const {return !(*this == t);}
		bool operator != (const std::string& str) const {return !(*this == str);}
		bool operator != (const char* str) const {return !(*this == str);}
		
		Word(Type type_, std::string word_ = "") : word(word_), type(type_) {
			// Automatically assign a string word when constructed only via the type
			if (word == "") {
				switch (type) {
					case Empty: case Void:{
						// OK
						}break;
						
					case Numeric:{
						this->word = "0";
						}break;
					case Text:{
						// OK
						}break;
					case ExpressionBegin:{
						this->word = "(";
						}break;
					case ExpressionEnd:{
						this->word = ")";
						}break;
					case HashTag:{
						this->word = "#";
						}break;
					case TrailOperator:{
						this->word = ".";
						}break;
					case NamespaceOperator:{
						this->word = "::";
						}break;
					case CastOperator:{
						this->word = ":";
						}break;
					case NotOperator:{
						this->word = "!";
						}break;
					case AndOperator:{
						this->word = "&&";
						}break;
					case OrOperator:{
						this->word = "||";
						}break;
					case XorOperator:{
						this->word = "xor";
						}break;
					case ConcatOperator:{
						this->word = "&";
						}break;
					case CommaOperator:{
						this->word = ",";
						}break;
						
					// // Don't specify a word for those when constructed only via the type, we will assert instead
					// case Varname:
					// 	this->word = "Varname";
					// 	break;
					// case Funcname:
					// 	this->word = "Funcname";
					// 	break;
					// case Name:
					// 	this->word = "Name";
					// 	break;
					
					case SuffixOperatorGroup:
						this->word = "SuffixOperatorGroup";
						break;
					case MulOperatorGroup:
						this->word = "MulOperatorGroup";
						break;
					case AddOperatorGroup:
						this->word = "AddOperatorGroup";
						break;
					case CompareOperatorGroup:
						this->word = "CompareOperatorGroup";
						break;
					case EqualityOperatorGroup:
						this->word = "EqualityOperatorGroup";
						break;
					case AssignmentOperatorGroup:
						this->word = "AssignmentOperatorGroup";
						break;
						
					default: assert(!"Word not specified");
				}
			}
		}
		explicit Word(const std::string& value) : word(value), type(Text) {}
		explicit Word(double value) : word(ToString(value)), type(Numeric) {}
		Word(const Word& other) = default;
		Word(Word&& other) = default;
		Word& operator= (const Word& other) = default;
		Word& operator= (Word&& other) = default;
		Word& operator= (const std::string& value) {
			word = value;
			type = Text;
			return *this;
		}
		Word& operator= (double value) {
			word = ToString(value);
			type = Numeric;
			return *this;
		}
		
		Word(std::istringstream& s) {
			for (int c; int8_t(c = s.get()) != -1; ) if (c != ' ') {
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
								if (s.eof() || s.peek() != '"') {
									break;
								}
							}
							if (word.length() >= XC_MAX_TEXT_LENGTH) {
								throw ParseError("Text too long");
							}
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
								if (s.peek() == '"') {
									word += s.get();
									if (s.peek() != '"') {
										inString = false;
										continue;
									}
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
						if (stack < -1) {
							throw ParseError("Extra parenthesis");
						}
						if (stack > -1) {
							throw ParseError("Missing parenthesis");
						}
						type = Expression;
					}return;
					case ')':{
						throw ParseError("Extra parenthesis");
					}
				}
				// Comments
				if (c == ';' || (c == '/' && s.peek() == '/')) {
					s = {};
					return;
				}
				// HashTag
				if (c == '#') {
					word = c;
					type = HashTag;
					return;
				}
				// Operators
				if (isoperator(c)) {
					word += c;
					int c2 = s.peek();
					if (isoperator(c2) && (c2 == c || c2 == '=' || (c == '<' && c2 == '>'))) {
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
						if (!isdigit(s.peek())) {
							if (!hasDecimal && isalnum_(s.peek())) goto ContinueWithName;
							else break;
						}
						word += s.get();
					}
					type = Numeric;
					return;
				}
				// Name
				if (isalpha_(c)) {
					word += tolower(c);
					ContinueWithName:
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

	inline static std::ostream& operator << (std::ostream& s, const Word& w) {
		return s << std::string(w);
	}

	// Parses one or more words from given string (including expressions done recursively), and assign final types to words. Sets resulting words in given words ref. This will NOT add expressions according to operator precedence. 
	inline static void ParseWords(const std::string& str, std::vector<Word>& words, int& scope) {
		std::istringstream line(str);
		while (Word word {line}) {
			if (word == Word::Tab) {
				if (words.size() == 0) {
					++scope;
				}
			} else if (word == Word::Invalid) {
				throw ParseError("Invalid character", word);
			} else {
				if (word == Word::Operator) {
					if (word == ".")
						word.type = Word::TrailOperator;
					else if (word == "::")
						word.type = Word::NamespaceOperator;
					else if (word == ":")
						word.type = Word::CastOperator;
					else if (word == "++" || word == "--" || word == "!!")
						word.type = Word::SuffixOperatorGroup;
					else if (word == "&")
						word.type = Word::ConcatOperator;
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
					else if (word == "=" || word == "+=" || word == "-=" || word == "*=" || word == "/=" || word == "^=" || word == "%=" || word == "&=")
						word.type = Word::AssignmentOperatorGroup;
					else if (word == ",")
						word.type = Word::CommaOperator;
					else throw ParseError("Invalid Operator", word);
				} else if (word == Word::Name) {
					if (word == "and")
						word.type = Word::AndOperator;
					else if (word == "or")
						word.type = Word::OrOperator;
					else if (word == "xor")
						word.type = Word::XorOperator;
				}
				if (word == Word::Expression) {
					words.push_back(Word::ExpressionBegin);
					ParseWords(word, words, scope);
					words.push_back(Word::ExpressionEnd);
				} else if (word == Word::HashTag) {
					return; // ignore the remaining of the line
				} else {
					words.push_back(word);
				}
			}
		}
	}

	// From a given index of an opening parenthesis, returns the index of the corresponsing closing parenthesis, or -1 if not found. 
	inline static int GetExpressionEnd(const std::vector<Word>& words, int begin, int end = -1) {
		int stack = 0;
		if (end < 0) end += words.size();
		while(++begin <= end) {
			if (words[begin] == Word::ExpressionEnd) {
				if (stack == 0) return begin;
				--stack;
			} else if (words[begin] == Word::ExpressionBegin) {
				++stack;
			}
		}
		return -1;
	}

	// From a given index of a closing parenthesis (end), returns the index of the corresponsing opening parenthesis, or -1 if not found. This function will include a leading Name if there is one (for a function call)
	inline static int GetExpressionBegin(const std::vector<Word>& words, int begin, int end) {
		int stack = 0;
		while(--end >= 0) {
			if (words[end] == Word::ExpressionBegin) {
				if (stack == 0) {
					if (end > 0 && (words[end-1] == Word::Name || words[end-1] == Word::Funcname)) {
						if (end-1 >= begin) {
							return end-1;
						}
					}
					return end;
				}
				--stack;
			} else if (words[end] == Word::ExpressionEnd) {
				++stack;
			}
		}
		return -1;
	}

	// From a given begin index for a function argument, returns the index of the last word before either the next comma or the argument list closing parenthesis, or -1 if not found.
	inline static int GetArgEnd(const std::vector<Word>& words, int begin, int end = -1) {
		int stack = 0;
		if (end < 0) end += words.size();
		if (words[begin] == Word::ExpressionBegin) ++stack;
		else if (words[begin] == Word::ExpressionEnd) return -1; // no argument
		while(begin++ < end) {
			if (words[begin] == Word::ExpressionEnd) {
				if (stack == 0) return begin - 1;
				--stack;
			} else if (words[begin] == Word::ExpressionBegin) {
				++stack;
			} else if (words[begin] == Word::CommaOperator) {
				if (stack == 0) return begin - 1;
			}
		}
		return -1;
	}

	// Prints out the interpreted code after it's fully parsed (after ParseLine, or after a ParseExpression)
	inline static void DebugWords(std::vector<Word>& words, int startIndex = 0, bool verbose = false) {
		for (unsigned int i = startIndex; i < words.size(); ++i) {
			const auto& word = words[i];
			if (verbose) {
				switch (word.type) {
					case Word::Numeric:
						std::cout << "Numeric{" << word << "} ";
						break;
					case Word::Text:
						std::cout << "Text{" << word << "} ";
						break;
					case Word::Varname:
						std::cout << "Varname{" << word << "} ";
						break;
					case Word::Funcname:
						std::cout << "Funcname{" << word << "} ";
						break;
					case Word::ExpressionBegin:
						std::cout << "Expression{ ";
						break;
					case Word::ExpressionEnd:
						std::cout << "} ";
						break;
					case Word::Name:
						std::cout << "Name{" << word << "} ";
						break;
					case Word::TrailOperator:
						std::cout << "TrailOperator{" << word << "} ";
						break;
					case Word::NamespaceOperator:
						std::cout << "NamespaceOperator{" << word << "} ";
						break;
					case Word::CastOperator:
						std::cout << "CastOperator{" << word << "} ";
						break;
					case Word::SuffixOperatorGroup:
						std::cout << "SuffixOperatorGroup{" << word << "} ";
						break;
					case Word::ConcatOperator:
						std::cout << "ConcatOperator{" << word << "} ";
						break;
					case Word::NotOperator:
						std::cout << "NotOperator{" << word << "} ";
						break;
					case Word::MulOperatorGroup:
						std::cout << "MulOperatorGroup{" << word << "} ";
						break;
					case Word::AddOperatorGroup:
						std::cout << "AddOperatorGroup{" << word << "} ";
						break;
					case Word::CompareOperatorGroup:
						std::cout << "CompareOperatorGroup{" << word << "} ";
						break;
					case Word::EqualityOperatorGroup:
						std::cout << "EqualityOperatorGroup{" << word << "} ";
						break;
					case Word::AndOperator:
						std::cout << "AndOperator{" << word << "} ";
						break;
					case Word::OrOperator:
						std::cout << "OrOperator{" << word << "} ";
					case Word::XorOperator:
						std::cout << "XorOperator{" << word << "} ";
						break;
					case Word::AssignmentOperatorGroup:
						std::cout << "AssignmentOperatorGroup{" << word << "} ";
						break;
					case Word::CommaOperator:
						std::cout << "CommaOperator{" << word << "} ";
						break;
					case Word::Void:
						std::cout << "void ";
						break;
						
					case Word::FileInfo:
						std::cout << "FileInfo{" << word << "} ";
						break;
						
					default: assert(!"Parse Error");
				}
			} else {
				switch (word.type) {
					case Word::Numeric:
						std::cout << word << " ";
						break;
					case Word::Text:
						std::cout << "\"" << word << "\" ";
						break;
					case Word::Varname:
						std::cout << "$" << word << " ";
						break;
					case Word::Funcname:
						std::cout << "@" << word << " ";
						break;
					case Word::ExpressionBegin:
						std::cout << "( ";
						break;
					case Word::ExpressionEnd:
						std::cout << ") ";
						break;
					case Word::Name:
						std::cout << word << " ";
						break;
					case Word::TrailOperator:
					case Word::NamespaceOperator:
					case Word::CastOperator:
					case Word::SuffixOperatorGroup:
					case Word::ConcatOperator:
					case Word::NotOperator:
					case Word::MulOperatorGroup:
					case Word::AddOperatorGroup:
					case Word::CompareOperatorGroup:
					case Word::EqualityOperatorGroup:
					case Word::AssignmentOperatorGroup:
					case Word::CommaOperator:
						std::cout << word << " ";
						break;
					case Word::AndOperator:
						std::cout << "and ";
						break;
					case Word::OrOperator:
						std::cout << "or ";
						break;
					case Word::XorOperator:
						std::cout << "xor ";
						break;
					case Word::Void:
						break;
						
					case Word::FileInfo:
						std::cout << "// <" << word << "> ";
						break;
						
					default: assert(!"Parse Error");
				}
			}
		}
	}

	// Parse an expression following an assignement or function call arguments excluding surrounding parenthesis. Returns true on success, false on error. This function may add words to the given vector, mostly to add parenthesis around operations based on operator precedence. 
	inline static bool ParseExpression(std::vector<Word>& words, int startIndex, int endIndex = -1) {
		auto lastWord = [&words, &endIndex]{
			return endIndex == -1? words.size()-1 : std::min(size_t(endIndex), words.size()-1);
		};
		
		// Handle single-word literal expressions (also includes single variable name)
		if (startIndex == (int)lastWord() && (words[startIndex] == Word::Numeric || words[startIndex] == Word::Text || words[startIndex] == Word::Varname || words[startIndex] == Word::Name)) {
			return true;
		}
		
		// Any other type of single-word expression should be invalid
		if (startIndex >= (int)lastWord()) {
			return false;
		}
		
		// Encapsulate operations within expressions, based on operator precedence
		for (Word::Type op = Word::Type(WORD_ENUM_OPERATOR_START); op < Word::CommaOperator; op = Word::Type(+op+1)) {
			for (int opIndex = startIndex; opIndex <= (int)lastWord(); ++opIndex) {
				if (words[opIndex] == op) {
					auto word = words[opIndex];
					int prevPos = opIndex-1;
					int nextPos = opIndex+1;
					Word prev = prevPos >= startIndex? words[prevPos] : Word::Empty;
					Word next = nextPos <= (int)lastWord()? words[nextPos] : Word::Empty;
					
					// Suffix operators (INVALID IN EXPRESSIONS, because they modify the variable itself, and that should require a direct assignment in a separate statement, as per the language's philosophy)
					if (word == Word::SuffixOperatorGroup) {
						
						throw ParseError("Cannot use a suffix operator within an expression");
							// if (!prev || prev != Word::Varname) {
							// 	throw ParseError("Suffix operators can only be used after a variable name");
							// }
							// words.insert(words.begin() + nextPos, Word::ExpressionEnd);
							// words.insert(words.begin() + prevPos, Word::ExpressionBegin);
							// ++opIndex;
							// if (endIndex != -1) endIndex += 2;
						
					} else {
						// Find expression boundaries
						if (next == Word::AddOperatorGroup || next == Word::NotOperator) {
							++nextPos;
							next = nextPos <= (int)lastWord()? words[nextPos] : Word::Empty;
						}
						if (next == Word::ExpressionBegin) {
							nextPos = GetExpressionEnd(words, nextPos);
							if (nextPos == -1 || nextPos > (int)lastWord()) {
								return false;
							}
						}
						if (prev == Word::ExpressionEnd) {
							prevPos = GetExpressionBegin(words, startIndex, prevPos);
							if (prevPos < 0) {
								return false;
							}
						}
						// Handle function calls (and cast) within expressions
						if (next == Word::Name || next == Word::Funcname) {
							Word after = nextPos+1 <= (int)lastWord()? words[nextPos+1] : Word::Empty;
							if (word != Word::CastOperator && word != Word::TrailOperator && next == Word::Funcname && after != Word::ExpressionBegin) {
								return false;
							}
							if (after == Word::ExpressionBegin) {
								nextPos = GetExpressionEnd(words, nextPos+1);
								if (nextPos == -1 || nextPos > (int)lastWord()) {
									return false;
								}
							}
						}
						// Middle operators
						if (prev && next && prev != Word::ExpressionBegin && next != Word::ExpressionEnd && prev.type < WORD_ENUM_OPERATOR_START && next.type < WORD_ENUM_OPERATOR_START) {
							Word before = prevPos >= startIndex && prevPos > 0? words[prevPos-1] : Word::Empty;
							Word after = nextPos+1 <= (int)lastWord()? words[nextPos+1] : Word::Empty;
							// Check if not already between parenthesis
							if (before != Word::ExpressionBegin || after != Word::ExpressionEnd) {
								if (word == Word::TrailOperator) {
									if (prev != Word::Varname) {
										throw ParseError("A trail operator (.) is not allowed after", prev);
									}
									if (next != Word::Numeric && next != Word::Varname && next != Word::Name) {
										throw ParseError("A trail operator (.) cannot be followed by", next, "within an expression");
									}
								} else if (word == Word::CastOperator) {
									if (prev != Word::Varname && prev != Word::ExpressionEnd) {
										throw ParseError("A cast operator (:) is not allowed after", prev);
									}
									if (next != Word::Name) {
										throw ParseError("A cast operator (:) cannot be followed by", next);
									}
								}
								words.insert(words.begin() + nextPos + 1, Word::ExpressionEnd);
								words.insert(words.begin() + prevPos, Word::ExpressionBegin);
								++opIndex;
								if (endIndex != -1) endIndex += 2;
							}
						} else {
							// Prefix operators
							if (next && next != Word::ExpressionEnd && (word == Word::AddOperatorGroup || word == Word::NotOperator)) {
								if (word == "-") {
									if (next == Word::Numeric) {
										words[nextPos].word = "-" + next.word;
										words.erase(words.begin() + opIndex);
										--opIndex;
										if (endIndex != -1) --endIndex;
									} else {
										words.insert(words.begin() + nextPos + 1, Word::ExpressionEnd);
										words.insert(words.begin() + opIndex, Word::Numeric);
										words.insert(words.begin() + opIndex, Word::ExpressionBegin);
										opIndex += 2;
										if (endIndex != -1) endIndex += 3;
									}
								} else if (word == "+") {
									words.erase(words.begin() + opIndex);
									--opIndex;
									if (endIndex != -1) --endIndex;
								} else if (word == "!") {
									words.insert(words.begin() + nextPos + 1, Word::ExpressionEnd);
									words.insert(words.begin() + opIndex, Word::Void);
									words.insert(words.begin() + opIndex, Word::ExpressionBegin);
									opIndex += 2;
									if (endIndex != -1) endIndex += 3;
								} else {
									assert(!"Operator not implement"); // Not supposed to happen
								}
							} else if (!next) {
								return false;
							}
						}
					}
				}
			}
		}
		
		// Key can be followed by any from set
		const std::map<Word::Type, std::set<Word::Type>> authorizedSemantics {
			{Word::Numeric, {
				Word::ExpressionEnd,
				Word::CastOperator,
				Word::MulOperatorGroup,
				Word::AddOperatorGroup,
				Word::CompareOperatorGroup,
				Word::EqualityOperatorGroup,
				Word::AndOperator,
				Word::OrOperator,
				Word::XorOperator,
				Word::CommaOperator,
			}},
			{Word::Text, {
				Word::Text,
				Word::ExpressionEnd,
				Word::ConcatOperator,
				Word::CommaOperator,
			}},
			{Word::Varname, {
				Word::ExpressionEnd,
				Word::TrailOperator,
				Word::CastOperator,
				Word::MulOperatorGroup,
				Word::AddOperatorGroup,
				Word::CompareOperatorGroup,
				Word::EqualityOperatorGroup,
				Word::AndOperator,
				Word::OrOperator,
				Word::XorOperator,
				Word::ConcatOperator,
				Word::CommaOperator,
			}},
			{Word::Funcname, {
				Word::ExpressionBegin,
			}},
			{Word::Name, {
				Word::ExpressionBegin,
				Word::ExpressionEnd,
				Word::ConcatOperator,
				Word::CastOperator,
				Word::MulOperatorGroup,
				Word::AddOperatorGroup,
				Word::CompareOperatorGroup,
				Word::EqualityOperatorGroup,
				Word::AndOperator,
				Word::OrOperator,
				Word::XorOperator,
				Word::CommaOperator,
			}},
			{Word::ExpressionBegin, {
				Word::Numeric,
				Word::Text,
				Word::Varname,
				Word::Funcname,
				Word::Name,
				Word::ExpressionBegin,
				Word::Void,
			}},
			{Word::ExpressionEnd, {
				Word::ConcatOperator,
				Word::ExpressionEnd,
				Word::CastOperator,
				Word::MulOperatorGroup,
				Word::AddOperatorGroup,
				Word::CompareOperatorGroup,
				Word::EqualityOperatorGroup,
				Word::AndOperator,
				Word::OrOperator,
				Word::XorOperator,
				Word::CommaOperator,
			}},
			{Word::TrailOperator, {
				Word::Numeric,
				Word::Varname,
				Word::Funcname,
				Word::Name,
			}},
			{Word::CastOperator, {
				Word::Name,
			}},
			// {Word::SuffixOperatorGroup, {}}, // Not allowed within expressions
			{Word::NotOperator, {
				Word::Numeric,
				Word::Varname,
				Word::Funcname,
				Word::Name,
				Word::ExpressionBegin,
			}},
			{Word::MulOperatorGroup, {
				Word::Numeric,
				Word::Varname,
				Word::Funcname,
				Word::Name,
				Word::ExpressionBegin,
			}},
			{Word::AddOperatorGroup, {
				Word::Numeric,
				Word::Varname,
				Word::Funcname,
				Word::Name,
				Word::ExpressionBegin,
			}},
			{Word::CompareOperatorGroup, {
				Word::Numeric,
				Word::Varname,
				Word::Funcname,
				Word::Name,
				Word::ExpressionBegin,
			}},
			{Word::EqualityOperatorGroup, {
				Word::Numeric,
				Word::Text,
				Word::Varname,
				Word::Funcname,
				Word::Name,
				Word::ExpressionBegin,
			}},
			{Word::AndOperator, {
				Word::Numeric,
				Word::Varname,
				Word::Funcname,
				Word::Name,
				Word::ExpressionBegin,
			}},
			{Word::OrOperator, {
				Word::Numeric,
				Word::Varname,
				Word::Funcname,
				Word::Name,
				Word::ExpressionBegin,
			}},
			{Word::XorOperator, {
				Word::Numeric,
				Word::Varname,
				Word::Funcname,
				Word::Name,
				Word::ExpressionBegin,
			}},
			// {Word::AssignmentOperatorGroup, {}}, // Not allowed within expressions
			{Word::CommaOperator, {
				Word::Numeric,
				Word::Text,
				Word::Varname,
				Word::Funcname,
				Word::Name,
				Word::ExpressionBegin,
			}},
			{Word::Void, {
				Word::NotOperator,
			}},
			{Word::ConcatOperator, {
				Word::Text,
				Word::Varname,
				Word::Funcname,
				Word::Name,
				Word::ExpressionBegin,
			}},
		};
		
		// Left Key can be followed by any from set if preceeded by Right key (may have multiple sets per left key with different right keys)
		const std::map<Word::Type, std::map<Word::Type, std::set<Word::Type>>> authorizedSemanticsWhenPreceeded {
			{Word::Name, {{Word::CastOperator, {
				Word::ConcatOperator,
				Word::ExpressionEnd,
				Word::MulOperatorGroup,
				Word::AddOperatorGroup,
				Word::CompareOperatorGroup,
				Word::EqualityOperatorGroup,
				Word::AndOperator,
				Word::OrOperator,
				Word::XorOperator,
				Word::CommaOperator,
			}}}},
			{Word::ExpressionBegin, {
				{Word::Funcname, {
					Word::ExpressionEnd,
				}},
				{Word::Name, {
					Word::ExpressionEnd,
				}},
			}},
		};
		
		// Validate expression semantics using the above rules
		for (int index = startIndex; index < (int)lastWord(); ++index) {
			Word::Type a = words[index].type;
			Word::Type b = words[index+1].type;
			if (!authorizedSemantics.contains(a) || !authorizedSemantics.at(a).contains(b)) {
				if (index > startIndex) {
					Word::Type prev = words[index-1].type;
					if (authorizedSemanticsWhenPreceeded.contains(a)
					&& authorizedSemanticsWhenPreceeded.at(a).contains(prev)
					&& authorizedSemanticsWhenPreceeded.at(a).at(prev).contains(b)
					) continue;
				}
				return false;
			}
		}
		
		return true;
	}

	// This is for function and input declarations. Returns the position to continue, or -1 if reached the end of the line. This function also ensures that the returned value is less than the word count.
	inline static int ParseDeclarationArgs(std::vector<Word>& words, int index) {
		// Args MUST be surrounded by parenthesis already, thus the initial index MUST be an opening parenthesis
		if (words[index] != Word::ExpressionBegin) {
			throw ParseError("Arguments must be between parenthesis");
		}
		// Find the corresponding closing parenthesis
		int end = GetExpressionEnd(words, index);
		if (end == -1) {
			throw ParseError("Missing closing parenthesis");
		}
		// Set end to the index before the closing parenthesis
		--end;
		
		// Zero arguments is a valid syntex, return to the caller without additional parsing.
		if (index == end) return -1;
		
		// Parse each argument definition one by one
		while (++index <= end) {
			if (words[index] != Word::Varname) {
				goto Error;
			}
			++index;
			if (index > end) {
				goto Error;
			}
			if (words[index] != Word::CastOperator) {
				goto Error;
			}
			++index;
			if (index > end) {
				goto Error;
			}
			if (words[index] != Word::Name) {
				goto Error;
			}
			if (index+4 <= end && words[index+1] == Word::CommaOperator) {
				++index;
			}
		}
		
		// Go to the end and return the next thing to parse or -1
		++end;
		if (end+1 >= (int)words.size()) return -1;
		return end + 1;
		Error: throw ParseError("Invalid arguments in function declaration");
	}

	// Returns the position at which to continue parsing after the closing parenthesis, or -1 if reached the end of the line. This function also ensures that the returned value is less than the word count.
	inline static int ParseArgs(std::vector<Word>& words, int index) {
		// Args MUST be surrounded by parenthesis already, thus the initial index MUST be an opening parenthesis
		if (words[index] != Word::ExpressionBegin) {
			throw ParseError("Arguments must be between parenthesis");
		}
		// Find the corresponding closing parenthesis
		int end = GetExpressionEnd(words, index);
		if (end == -1) {
			throw ParseError("Missing closing parenthesis");
		}
		// Set end to the index before the closing parenthesis
		--end;

		// Zero arguments is a valid syntex, return to the caller without additional parsing.
		if (index == end) return -1;
		
		// Parse the arguments as an expression
		if (ParseExpression(words, index+1, end)) {
			end = GetExpressionEnd(words, index);
			if (end == -1) {
				throw ParseError("Invalid expression within function arguments or missing closing parenthesis");
			}
			if (end == int(words.size())-1) return -1;
			return end + 1;
		} else {
			throw ParseError("Invalid expression within function arguments");
		}
		return -1;
	}

	// Valid first words for the possible statements in the global scope
	static inline const std::vector<std::string> globalScopeFirstWords {
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
		"recursive",
	};
	
	struct Implementation {
		static std::vector<std::string> entryPoints;
	};

	// Valid first words for the possible statements in a function scope
	static const std::vector<std::string> functionScopeFirstWords {
		"var",
		"array",
		"output",
		"foreach",
		"repeat",
		"while",
		"for",
		"break",
		"continue",
		"if",
		"elseif",
		"else",
		"return",
	};

	// Upon construction, it will parse the entire line from the given string, including expressions, and may throw ParseError
	struct ParsedLine {
		int scope = 0;
		int line = 0;
		std::vector<Word> words {};
		
		operator bool () const {
			return words.size() > 0;
		}
		
		ParsedLine(const std::string& str, int line_ = 0, bool generic = false) : line(line_) {
			ParseWords(str, words, scope);
			
			if (words.size() > 0) {
				
				// Check first word validity for this Scope
				if (scope == 0) { // Global scope
					if (words[0] != Word::Name || (!generic && find(begin(globalScopeFirstWords), end(globalScopeFirstWords), words[0].word) == end(globalScopeFirstWords) && find(begin(Implementation::entryPoints), end(Implementation::entryPoints), words[0].word) == end(Implementation::entryPoints))) {
						throw ParseError("Invalid first word", words[0], "in the global scope");
					}
				} else { // Function scope
					if (words[0] != Word::Varname && words[0] != Word::Funcname && words[0] != Word::Name) {
						throw ParseError("Invalid first word", words[0], "in a function scope");
					}
				}
				
				// Validate statement (and insert/modify words to standarize it)
				if (words[0] == Word::Name) {
					
					// In both Global and Function scopes
					
					// var
						if (words[0] == "var") {
							if ((words.size() > 1 && words[1] != Word::Varname)
							 || (words.size() < 4)
							 || (words[2] != "=" && words[2] != Word::CastOperator)
							 || (words[2] == Word::CastOperator && words[3] != "number" && words[3] != "text")
							 || (words[2] == Word::CastOperator && words.size() > 4)
							) {
								throw ParseError("Second word must be a variable name (starting with $), and it must be followed either by a colon and its type (number or text) or an equal sign and an expression");
							}
							if (words[2] == "=") {
								if (!ParseExpression(words, 3)) {
									throw ParseError("Invalid expression after var assignment");
								}
							}
						} else
						
					// array
						if (words[0] == "array") {
							if ((words.size() > 1 && words[1] != Word::Varname)
							 || (words.size() > 2 && words[2] != Word::CastOperator)
							) {
								throw ParseError("Second word must be a variable name (starting with $) followed by a colon and the array type (number or text)");
							}
							if (words.size() > 3 && words[3] != "number" && words[3] != "text") {
								throw ParseError("Invalid array type", words[3], "it must be either 'number' or 'text'");
							}
							if (words.size() > 4 && words[4] == "=") {
								throw ParseError("Cannot initialize array with a value");
							}
							if (words.size() > 4) throw ParseError("Too many words");
							if (words.size() < 4) throw ParseError("Too few words");
						} else
						
					// Only in Global Scope
					if (scope == 0) {
					// include
						if (words[0] == "include") {
							if (words.size() > 1) {
								if (words[1] == Word::Text) {
									// Valid
								} else if (words[1] == Word::Name) {
									words[1].type = Word::Text;
								} else {
									throw ParseError("Invalid file name", words[1]);
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
						if (words[0] == "function" || words[0] == "recursive") {
							int offset = 0;
							if (words[0] == "recursive") {
								if (words.size() < 2) {
									throw ParseError("Too few words");
								}
								if (words[1] != "function") {
									throw ParseError("Recursive is only valid as a modifier of a function");
								}
								offset++;
							}
							if (int(words.size()) > 1 + offset && words[1 + offset] != Word::Funcname) {
								throw ParseError("Second word must be a valid function name starting with @");
							}
							if (int(words.size()) > 2 + offset && words[2 + offset] != Word::ExpressionBegin) {
								throw ParseError("Function name must be followed by a set of parenthesis, optionally containing an argument list");
							}
							if (int(words.size()) < 4 + offset) throw ParseError("Too few words");
							int next = ParseDeclarationArgs(words, 2 + offset);
							if (next != -1) {
								if ((int)words.size() == next + 2 && words[next] == Word::CastOperator && (words[next + 1] == "number" || words[next + 1] == "text")) {
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
								if (!ParseExpression(words, 3)) throw ParseError("Invalid expression after const assignment");
							} else throw ParseError("Too few words");
						} else
						
					// storage
						if (words[0] == "storage") {
							if (words.size() > 1 && words[1] != "var" && words[1] != "array") {
								throw ParseError("Second word must be either 'var' or 'array'");
							}
							if ((words.size() > 2 && words[2] != Word::Varname)
							 || (words.size() > 3 && words[3] != Word::CastOperator)
							) {
								throw ParseError("Third word must be a variable name (starting with $) followed by a colon and the storage type (number or text)");
							}
							if (words.size() > 4 && words[4] != "number" && words[4] != "text") {
								throw ParseError("Invalid storage type", words[4], "it must be either 'number' or 'text'");
							}
							if (words.size() > 5 && words[5] == "=") {
								throw ParseError("Cannot initialize storage values here");
							}
							if (words.size() > 5) throw ParseError("Too many words");
							if (words.size() < 5) throw ParseError("Too few words");
						} else
						
					// entry points
						if (generic || find(begin(Implementation::entryPoints), end(Implementation::entryPoints), words[0].word) != end(Implementation::entryPoints)) {
							if (words.size() > 1) {
								if (words[1] == Word::TrailOperator) {
									if (words.size() > 2 && words[2] != Word::Numeric && words[2] != Word::Text && words[2] != Word::Varname) {
										throw ParseError("The reference after the dot must be either a literal value, a variable name or a constant name");
									}
									if (words.size() > 3) {
										if (words[3] != Word::ExpressionBegin) {
											throw ParseError("Reference may only be followed by a set of parenthesis, optionally containing an argument list");
										}
										if (words.size() < 5) throw ParseError("Too few words");
										if (ParseDeclarationArgs(words, 3) != -1) {
											throw ParseError("Too many words");
										}
									}
								} else {
									// throw ParseError("A device entry point may only be followed by a dot");
									if (words[1] != Word::ExpressionBegin) {
										throw ParseError("A device entry point may only be followed by a dot and a reference, then/or a set of parenthesis optionally containing an argument list");
									}
									if (words.size() < 3) throw ParseError("Too few words");
									if (ParseDeclarationArgs(words, 1) != -1) {
										throw ParseError("Too many words");
									}
								}
							}
						} else {
							throw ParseError("Invalid first word", words[0], "in the global scope");
						}
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
							if ((words.size() > 2 && words[2] != Word::ExpressionBegin)
							 || (words.size() > 3 && words[3] != Word::Varname)
							 || (words.size() > 4 && words[4] != Word::ExpressionEnd && words[4] != Word::CommaOperator)
							 || (words.size() > 5 && words[5] != Word::Varname)
							 || (words.size() > 6 && words[6] != Word::ExpressionEnd)
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
							if ((words.size() > 2 && words[2] != Word::ExpressionBegin)
							 || (words.size() > 3 && words[3] != Word::Varname)
							 || (words.size() > 4 && words[4] != Word::ExpressionEnd)
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
							if (words.size() == 1 || !ParseExpression(words, 1)) {
								throw ParseError("While must be followed by a boolean expression");
							}
						} else
						
					// for
						if (words[0] == "for") {
							if (words.size() < 7) {
								throw ParseError("FOR must be followed by start and end values separated by a comma, and a set of parenthesis containing the loop variable");
							}
							if (words[1] == Word::AddOperatorGroup && words[2] == Word::Numeric && words[3] == Word::CommaOperator) {
								words[2].word = words[1].word + words[2].word;
								words.erase(words.begin() + 1);
							}
							if (words[3] == Word::AddOperatorGroup && words[4] == Word::Numeric && words[5] == Word::ExpressionBegin) {
								words[4].word = words[3].word + words[4].word;
								words.erase(words.begin() + 3);
							}
							// loop variable
							if (words.size() > 6) {
								if (words[4] != Word::ExpressionBegin || words.size() < 6 || words[5] != Word::Varname || words[6] != Word::ExpressionEnd) {
									throw ParseError("For loop variable must be within parentheses");
								}
							} else {
								throw ParseError("Too few words");
							}
						} else
						
					// break
						if (words[0] == "break") {
							if (words.size() != 1) throw ParseError("Too many words");
						} else
						
					// continue
						if (words[0] == "continue") {
							if (words.size() != 1) throw ParseError("Too many words");
						} else
						
					// if
						if (words[0] == "if") {
							if (words.size() == 1 || !ParseExpression(words, 1)) {
								throw ParseError("If must be followed by a boolean expression");
							}
						} else
						
					// elseif
						if (words[0] == "elseif") {
							if (words.size() == 1 || !ParseExpression(words, 1)) {
								throw ParseError("Elseif must be followed by a boolean expression");
							}
						} else
						
					// else
						if (words[0] == "else") {
							if (words.size() != 1) throw ParseError("Too many words");
						} else
						
					// return
						if (words[0] == "return") {
							if (words.size() > 1 && !ParseExpression(words, 1)) {
								throw ParseError("Return must be followed by nothing, an expression, a variable name or a literal value");
							}
						} else
						
					// device function
						if (scope > 0) {
							if (words.size() == 1 || words[1] != Word::ExpressionBegin) {
								throw ParseError("A device function call must be followed by a set of parenthesis, optionally containing arguments");
							}
							if (ParseArgs(words, 1) != -1) {
								throw ParseError("Invalid argument list after device function call");
							}
						}
					
				} else
				
				// Only in Function scope
				
				// Varname
					if (words[0] == Word::Varname) {
						if (words.size() < 2 || (words[1] != Word::TrailOperator && words[1] != Word::AssignmentOperatorGroup && words[1] != Word::SuffixOperatorGroup)) {
							throw ParseError("A leading variable name must be followed either by a trailing function, an assignment or a suffix operator");
						}
						if (words[1] == Word::AssignmentOperatorGroup) {
							if (words.size() < 3) {
								throw ParseError("Too few words");
							}
							if (!ParseExpression(words, 2)) {
								throw ParseError("Invalid expression after var assignment");
							}
						} else if (words[1] == Word::SuffixOperatorGroup) {
							if (words.size() > 2) {
								throw ParseError("Too many words");
							}
						} else if (words[1] == Word::TrailOperator) {
							if (words.size() < 4) {
								throw ParseError("Too few words");
							}
							if (words[3] != Word::SuffixOperatorGroup && words.size() < 5) {
								throw ParseError("Too few words");
							}
							// Array/Object assignment
							if (words[2] == Word::Numeric || words[2] == Word::Varname || (words[2] == Word::Name && (words[3] == Word::AssignmentOperatorGroup || words[3] == Word::SuffixOperatorGroup))) {
								if (words[3] == Word::AssignmentOperatorGroup) {
									if (!ParseExpression(words, 4)) {
										throw ParseError("Invalid expression after var assignment");
									}
								} else if (words[3] == Word::SuffixOperatorGroup) {
									if (words.size() > 4) {
										throw ParseError("Too many words");
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
									if (words[2] == Word::Funcname || words.size() < 4 || (words[3] != Word::AssignmentOperatorGroup && words[3] != Word::SuffixOperatorGroup)) {
										throw ParseError("Invalid expression after trailing function on var");
									}
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
					
				throw ParseError("Invalid word", words[0]);
				
			}
		}
		
		ParsedLine(const std::vector<Word>& words_) : words(words_) {}
		ParsedLine(const Word& word) {
			words.emplace_back(word);
		}
	};

	// Upon construction, it will parse all lines from the given stream, and may throw ParseError
	struct SourceFile {
		std::string filepath;
		std::vector<ParsedLine> lines;

		SourceFile(const std::string& filepath_) : filepath(filepath_) {
			std::ifstream stream{filepath};
			
			if (stream.fail()) {
				throw ParseError("File not found '" + filepath + "'");
			}
			
			lines.emplace_back(Word{Word::FileInfo, filepath});
			
			int lineNumber = 1;
			//int scope = 0;
			
			// Parse all lines
			for (std::string lineStr; getline(stream, lineStr); lineNumber++) {
				try {
					//auto& line = lines.emplace_back(lineStr, lineNumber);
					//scope = line.scope;
				} catch (ParseError& e) {
					std::stringstream err {};
					err << e.what() << " in " << filepath << ":" << lineNumber;
					throw ParseError(err.str());
				}
			}
		}
		
		static std::string GetExistingFilePath(const std::string& filedir, std::string/*copy*/filename) {
			std::string filepath = filedir + "/" + filename;
			if (!std::filesystem::exists(filepath)) {
				std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);
				for (const auto& f : std::filesystem::directory_iterator(filedir)) {
					std::string fLC = f.path().filename().string();
					std::transform(fLC.begin(), fLC.end(), fLC.begin(), ::tolower);
					if (fLC == filename) {
						return f.path().string();
					}
				}
				throw ParseError("File not found '" + filepath + "'");
			}
			return filepath;
		}
		
		SourceFile(const std::string& filedir, const std::string& filename) : SourceFile(GetExistingFilePath(filedir, filename)) {}
		
		void DebugParsedLines() {
			for (auto& line : lines) {
				if (line) {
					std::cout << std::setw(8) << line.line << ": " << std::string(line.scope,'\t');
					DebugWords(line.words);
					std::cout << std::endl;
				}
			}
			std::cout << "\nParsing Success!\n" << std::endl;
		}
	};

	// This function recursively parses the given file and all included files and returns all lines joined
	inline static SourceFile GetParsedFile(const std::string& filedir, const std::string& filename, std::set<std::string>& parsedFiles) {
		std::string filenameLC = filename;
		std::transform(filenameLC.begin(), filenameLC.end(), filenameLC.begin(), ::tolower);
		if (parsedFiles.contains(filenameLC)) {
			throw ParseError("Circular dependency detected with file '" + filename + "'");
		}
		parsedFiles.insert(filenameLC);
		SourceFile src(filedir, filename);
		// Include other files (and replace the include statement by the lines of the other file, recursively)
		for (unsigned int i = 0; i < src.lines.size(); ++i) {
			if (auto& line = src.lines[i]; line) {
				if (line.scope == 0 && line.words[0] == "include") {
					std::string includeFilename = line.words[1];
					line.words.clear();
					auto includeSrc = GetParsedFile(filedir, includeFilename, parsedFiles);
					src.lines.insert(src.lines.begin()+i, includeSrc.lines.begin(), includeSrc.lines.end());
					i += includeSrc.lines.size();
					src.lines.insert(src.lines.begin()+i, Word{Word::FileInfo, filedir + "/" + filename});
					++i;
				}
			}
		}
		return src;
	}
	inline static SourceFile GetParsedFile(const std::string& filedir, const std::string& filename) {
		std::set<std::string> parsedFiles;
		return GetParsedFile(filedir, filename, parsedFiles);
	}

#pragma endregion

#pragma region Assembly Definition

	enum CODE_TYPE : uint8_t {
		// Statements
		RETURN = 0, // Must add this at the end of each function block
		VOID = 10, // Must add this after each OP statement, helps validate validity and also makes the bytecode kind of human readable
		DISCARD = 11, // Discard the return value of this statement
		OP = 32, // OP [...] VOID
		
		// Comments/Info
		SOURCEFILE = 33,
		LINENUMBER = 34,

		// Reference Values
		ROM_CONST_NUMERIC = 40,
		ROM_CONST_TEXT = 41,
		STORAGE_VAR_NUMERIC = 50,
		STORAGE_VAR_TEXT = 51,
		STORAGE_ARRAY_NUMERIC = 60,
		STORAGE_ARRAY_TEXT = 61,
		RAM_VAR_NUMERIC = 70,
		RAM_VAR_TEXT = 71,
		RAM_ARRAY_NUMERIC = 80,
		RAM_ARRAY_TEXT = 81,
		
		// Extra references
		ARRAY_INDEX = 101,
		OBJ_KEY = 102,
		DEVICE_FUNCTION_INDEX = 110,
		INTEGER = 115,
		ADDR = 120,
		
		// Objects
		RAM_OBJECT = 128, // implementation-defined objects
		// Everything above 128 is reserved for implementation-specific data
	};

	#define ARRAY_INDEX_NONE uint32_t(0xFFFFFF)

	inline static constexpr uint32_t Interpret3CharsAsInt(const char* str) {
		return uint32_t(str[0]) | (uint32_t(str[1]) << 8) | (uint32_t(str[2]) << 16) | (OP << 24);
	}
	#define DEF_OP(op) inline static constexpr uint32_t op = Interpret3CharsAsInt(#op);

	DEF_OP( SET /* [(ARRAY_INDEX ifindexnone[REF_NUM]) | (OBJ_KEY REF_KEY)] REF_DST [REF_VALUE]orZero */ ) // Assign a value
	DEF_OP( ADD /* REF_DST REF_A REF_B */ ) // +
	DEF_OP( SUB /* REF_DST REF_A REF_B */ ) // -
	DEF_OP( MUL /* REF_DST REF_A REF_B */ ) // *
	DEF_OP( DIV /* REF_DST REF_A REF_B */ ) // /
	DEF_OP( MOD /* REF_DST REF_A REF_B */ ) // %
	DEF_OP( POW /* REF_DST REF_A REF_B */ ) // ^
	DEF_OP( CCT /* REF_DST REF_A REF_B */ ) // & (concat)
	DEF_OP( AND /* REF_DST REF_A REF_B */ ) // &&
	DEF_OP( ORR /* REF_DST REF_A REF_B */ ) // ||
	DEF_OP( XOR /* REF_DST REF_A REF_B */ ) // xor
	DEF_OP( EQQ /* REF_DST REF_A REF_B */ ) // ==
	DEF_OP( NEQ /* REF_DST REF_A REF_B */ ) // !=
	DEF_OP( LST /* REF_DST REF_A REF_B */ ) // <
	DEF_OP( GRT /* REF_DST REF_A REF_B */ ) // >
	DEF_OP( LTE /* REF_DST REF_A REF_B */ ) // <=
	DEF_OP( GTE /* REF_DST REF_A REF_B */ ) // >=
	DEF_OP( INC /* REF_NUM */ ) // ++
	DEF_OP( DEC /* REF_NUM */ ) // --
	DEF_OP( NOT /* REF_DST REF_VAL */ ) // !
	DEF_OP( FLR /* REF_DST REF_NUM */ ) // floor(number)
	DEF_OP( CIL /* REF_DST REF_NUM */ ) // ceil(number)
	DEF_OP( RND /* REF_DST REF_NUM */ ) // round(number)
	DEF_OP( SIN /* REF_DST REF_NUM */ ) // sin(number)
	DEF_OP( COS /* REF_DST REF_NUM */ ) // cos(number)
	DEF_OP( TAN /* REF_DST REF_NUM */ ) // tan(number)
	DEF_OP( ASI /* REF_DST REF_NUM */ ) // asin(number)
	DEF_OP( ACO /* REF_DST REF_NUM */ ) // acos(number)
	DEF_OP( ATA /* REF_DST REF_NUM */ ) // atan(number)
	DEF_OP( ABS /* REF_DST REF_NUM */ ) // abs(number)
	DEF_OP( FRA /* REF_DST REF_NUM */ ) // fract(number)
	DEF_OP( SQR /* REF_DST REF_NUM */ ) // sqrt(number)
	DEF_OP( SIG /* REF_DST REF_NUM */ ) // sign(number [, default])
	DEF_OP( LOG /* REF_DST REF_NUM REF_BASE */ ) // log(number, base)
	DEF_OP( CLP /* REF_DST REF_NUM REF_MIN REF_MAX */ ) // clamp(number, min, max)
	DEF_OP( STP /* REF_DST REF_T1 REF_T2 REF_NUM */ ) // step(t1, t2, number)
	DEF_OP( SMT /* REF_DST REF_T1 REF_T2 REF_NUM */ ) // smoothstep(t1, t2, number)
	DEF_OP( LRP /* REF_DST REF_NUM REF_NUM REF_T */ ) // lerp(number, number, t)
	DEF_OP( NUM /* REF_DST REF_SRC */ ) // cast numeric
	DEF_OP( TXT /* REF_DST REF_SRC [REPLACEMENT_VARS ...] */ ) // cast text
	DEF_OP( DEV /* DEVICE_FUNCTION_INDEX RET_DST [REF_ARG ...] */ ) // device function
	DEF_OP( OUT /* REF_NUM [REF_ARG ...] */ ) // output
	DEF_OP( APP /* REF_ARR REF_VALUE [REF_VALUE ...] */ ) // array.append(value, value...)
	DEF_OP( CLR /* REF_ARR */ ) // array.clear()
	DEF_OP( POP /* REF_ARR */ ) // array.pop()
	DEF_OP( ASC /* REF_ARR */ ) // array.sort()
	DEF_OP( DSC /* REF_ARR */ ) // array.sortd()
	DEF_OP( INS /* REF_ARR REF_INDEX REF_VALUE [REF_VALUE ...] */ ) // array.insert(index, value, value...)
	DEF_OP( DEL /* REF_ARR REF_INDEX [REF_INDEX_END] */ ) // array.erase(index [, end])
	DEF_OP( FLL /* REF_ARR REF_QTY REF_VAL */ ) // array.fill(qty, value)
	DEF_OP( FRM /* (REF_ARR | REF_TXT) (REF_ARR | REF_VAL) [REF_SEPARATOR] */ ) // array.from(value [, separator])
	DEF_OP( SIZ /* REF_DST (REF_ARR | REF_TXT) */ ) // array.size  text.size
	DEF_OP( LAS /* REF_DST (REF_ARR | REF_TXT) */ ) // array.last  text.last
	DEF_OP( FND /* REF_DST (REF_ARR | REF_TXT) REF_VALUE */ ) // array.find(value)  text.find(value)
	DEF_OP( CON /* REF_DST (REF_ARR | REF_TXT) REF_VALUE */ ) // array.contains(value)  text.contains(value)
	DEF_OP( MIN /* REF_DST (REF_ARR | (REF_NUM [REF_NUM ...])) */ ) // array.min  min(number, number...)
	DEF_OP( MAX /* REF_DST (REF_ARR | (REF_NUM [REF_NUM ...])) */ ) // array.max  max(number, number...)
	DEF_OP( AVG /* REF_DST (REF_ARR | (REF_NUM [REF_NUM ...])) */ ) // array.avg  avg(number, number...)
	DEF_OP( SUM /* REF_DST (REF_ARR | (REF_NUM [REF_NUM ...])) */ ) // array.sum
	DEF_OP( MED /* REF_DST REF_ARR */ ) // array.med
	DEF_OP( SBS /* REF_DST REF_SRC REF_START REF_LENGTH */ ) // substring(text, start, length)
	DEF_OP( IDX /* REF_DST ((REF_ARR ARRAY_INDEX ifindexnone[REF_NUM]) | (REF_TXT ((OBJ_KEY REF_KEY) | (ARRAY_INDEX ifindexnone[REF_NUM])))) */ ) // get an indexed value from an array or text, or a StringObject given a key
	DEF_OP( JMP /* ADDR */ ) // jump to addr while pushing the stack so that we return here after
	DEF_OP( GTO /* ADDR */ ) // goto addr without pushing the stack
	DEF_OP( CND /* ADDR_TRUE ADDR_FALSE REF_BOOL */ ) // conditional goto (gotoAddrIfTrue, gotoAddrIfFalse, boolExpression)
	DEF_OP( KEY /* REF_DST REF_TXT REF_OFFSET */) // returns the next key in a text object, and moves the offset as well
	DEF_OP( STR /* ADDR LEN TYPE */) // stores away local variables at from ADDR offset to (exclusive) ADDR + LEN of type TYPE
	DEF_OP( RST /* ADDR LEN TYPE */) // restores local variables at from ADDR offset to (exclusive) ADDR + LEN of type TYPE
	DEF_OP( HSH /* REF_DST REF_VAL */ ) // hash(text)
	DEF_OP( UPP /* REF_DST REF_TXT */ ) // upper(text)
	DEF_OP( LCC /* REF_DST REF_TXT */ ) // lower(text)
	DEF_OP( ISN /* REF_DST REF_TXT */ ) // isnumeric(text)
	DEF_OP( IFF /* REF_DST REF_TXT */ ) // if(cond, valTrue, valFalse)

#pragma endregion

#pragma region Compiler

	struct Var {
		enum Type : uint8_t {
			Void = 0,
			Numeric = 1,
			Text = 2,
			Object = 128
		} type;
		
		std::string textValue;
		union {
			double numericValue;
			uint64_t addrValue;
		};
		
		Var() : type(Void), textValue(""), numericValue(0.0) {}
		Var(bool value) : type(Numeric), textValue(""), numericValue(value) {}
		Var(int32_t value) : type(Numeric), textValue(""), numericValue(value) {}
		Var(int64_t value) : type(Numeric), textValue(""), numericValue(value) {}
		Var(uint32_t value) : type(Numeric), textValue(""), numericValue(value) {}
		Var(uint64_t value) : type(Numeric), textValue(""), numericValue(value) {}
		Var(double value) : type(Numeric), textValue(""), numericValue(value) {}
		Var(float value) : type(Numeric), textValue(""), numericValue(value) {}
		Var(const char* value) : type(Text), textValue(value), numericValue(0) {}
		Var(const std::string& value) : type(Text), textValue(value), numericValue(0) {}
		Var(const Var& other) : type(other.type), textValue(other.textValue), numericValue(other.numericValue) {}
		Var(Type type_, uint64_t objAddr) : type(type_), textValue(""), addrValue(objAddr) {}
		
		operator bool() const {
			if (type == Numeric) return numericValue;
			if (type == Text) return textValue != "";
			if (type >= Object) return addrValue;
			return false;
		}
		operator double() const {
			if (type == Numeric) return numericValue;
			else if (type == Text) return textValue==""? 0.0 : stod(textValue);
			return 0.0;
		}
		operator float() const {
			if (type == Numeric) return float(numericValue);
			else if (type == Text) return textValue==""? 0.0f : stof(textValue);
			return 0.0f;
		}
		operator int64_t() const {
			if (type == Numeric) return (int64_t)std::round(numericValue);
			else if (type == Text) return stoll(textValue);
			return 0;
		}
		operator uint64_t() const {
			if (type == Numeric) return (uint64_t)std::round(numericValue);
			else if (type == Text) return stoull(textValue);
			else if (type >= Object) return addrValue;
			return 0;
		}
		operator int32_t() const {
			if (type == Numeric) return (int32_t)std::round(numericValue);
			else if (type == Text) return stol(textValue);
			return 0;
		}
		operator uint32_t() const {
			if (type == Numeric) return (uint32_t)std::round(numericValue);
			else if (type == Text) return stoul(textValue);
			return 0;
		}
		operator int16_t() const {
			if (type == Numeric) return (int16_t)std::round(numericValue);
			else if (type == Text) return stol(textValue);
			return 0;
		}
		operator uint16_t() const {
			if (type == Numeric) return (uint16_t)std::round(numericValue);
			else if (type == Text) return stoul(textValue);
			return 0;
		}
		operator int8_t() const {
			if (type == Numeric) return (int8_t)std::round(numericValue);
			else if (type == Text) return stol(textValue);
			return 0;
		}
		operator uint8_t() const {
			if (type == Numeric) return (uint8_t)std::round(numericValue);
			else if (type == Text) return stoul(textValue);
			return 0;
		}
		operator std::string() const {
			if (type == Numeric) return ToString(numericValue);
			else if (type == Text) return textValue;
			return "";
		}
	};

	class Computer;
	using DeviceFunction = std::function<Var(Computer*, const std::vector<Var>& args)>;
	using DeviceObjectMember = std::function<Var(Computer*, const Var& obj, const std::vector<Var>& args)>;
	using OutputFunction = std::function<void(Computer*, uint32_t ioNumber, const std::vector<Var>& args)>;

	struct ObjectType {
		uint8_t id;
		std::string name;
	};

	struct Device {
		static std::unordered_map<std::string, ObjectType> objectTypesByName;
		static std::unordered_map<uint8_t, std::string> objectNamesById;
		static std::vector<std::string> objectTypesList;

		struct FunctionInfo {
			struct Arg {
				std::string name;
				std::string type;
			};
			
			uint32_t id; // may be set after construction
			std::string name = "";
			std::vector<Arg> args {};
			std::string returnType = "";
			std::string key = "";
			
			FunctionInfo(uint32_t id_, const std::string& line) : id(id_) {
				std::vector<Word> words;
				
				int nextWordIndex = 0;
				auto readWord = [&] (Word::Type type = Word::Empty) -> Word {
					if (nextWordIndex < (int)words.size()) {
						assert(type == Word::Empty || type == words[nextWordIndex].type);
						return words[nextWordIndex++];
					}
					return Word::Empty;
				};
				
				int scope = 0;
				ParseWords(line, words, scope);
				
				assert(words.size() > 0);
				assert(scope == 0);
				
				key = name = readWord(Word::Name).word;
				
				if (words.size() == 1) return;
				
				auto nextWord = readWord();
				if (nextWord == Word::NamespaceOperator) {
					key = readWord().word;
					name += "::" + key;
					nextWord = readWord();
				}
				
				// Arguments
				if (nextWord == Word::ExpressionBegin) {
					int argN = 0;
					for (;;) {
						Word word = readWord();
						if (!word || word == Word::ExpressionEnd) break;
						if (word == Word::CommaOperator) {
							assert(argN > 0);
							word = readWord();
						}
						++argN;
						assert(word == Word::Varname);
						readWord(Word::CastOperator);
						std::string type = readWord(Word::Name);
						if (type != "number" && type != "text" && !objectTypesByName.contains(type)) {
							assert(!"Invalid argument type in device function prototype");
						}
						args.emplace_back(std::string(word), type);
					}
					nextWord = readWord();
				}
				
				// Return type
				if (nextWord == Word::CastOperator) {
					Word type = readWord(Word::Name);
					if (type != "number" && type != "text" && !objectTypesByName.contains(type)) {
						assert(!"Invalid return type in device function prototype");
					}
					returnType = std::string(type);
				}
			}
		};

		static std::unordered_map<std::string, Device::FunctionInfo> deviceFunctionsByName;
		static std::unordered_map<uint32_t/*24 least significant bits only*/, std::string> deviceFunctionNamesById;
		static std::unordered_map<uint32_t/*24 least significant bits only*/, DeviceFunction> deviceFunctionsById;
		static std::unordered_map<uint8_t/*objectId*/, std::vector<std::string>> deviceFunctionsList;
		
		static OutputFunction outputFunction;
	};
	
	// Implementation SHOULD declare entry points
	inline static void DeclareEntryPoint(std::string entryName) {
		strtolower(entryName);
		assert(find(globalScopeFirstWords.begin(), globalScopeFirstWords.end(), entryName) == globalScopeFirstWords.end());
		Implementation::entryPoints.emplace_back(entryName);
	}

	// Implementation SHOULD declare (or override) device functions
	inline static Device::FunctionInfo& DeclareDeviceFunction(const std::string& prototype, DeviceFunction&& func, uint8_t base = 0) {
		Device::FunctionInfo f {0, prototype};
		if (Device::deviceFunctionsByName.contains(f.name)) {
			if (base != 0) throw std::runtime_error("Cannot override a device object member");
			auto& existingFuncRef = Device::deviceFunctionsByName.at(f.name);
			existingFuncRef.args = f.args;
			existingFuncRef.returnType = f.returnType;
			Device::deviceFunctionsById[existingFuncRef.id] = forward<DeviceFunction>(func);
			return existingFuncRef;
		}
		static std::map<uint8_t, uint32_t> nextID {};
		assert(nextID[base] < 65535);
		uint32_t id = (++nextID[base]) | (uint32_t(base) << 16);
		f.id = id;
		Device::deviceFunctionsByName.emplace(f.name, f);
		Device::deviceFunctionNamesById.emplace(id, f.name);
		Device::deviceFunctionsById.emplace(id, forward<DeviceFunction>(func));
		Device::deviceFunctionsList[base].emplace_back(f.key);
		assert(Device::deviceFunctionsList[base].size() == size_t(nextID[base]));
		return Device::deviceFunctionsByName.at(f.name);
	}

	// Implementation SHOULD declare object types
	inline static Var::Type DeclareObjectType(const std::string& name, const std::map<std::string, DeviceObjectMember>& members = {}) {
		static uint8_t nextID = 0;
		assert(nextID < 127);
		uint8_t id = ++nextID;
		Device::objectTypesByName.emplace(name, ObjectType{id, name});
		Device::objectNamesById.emplace(id, name);
		Device::objectTypesList.emplace_back(name);
		assert(Device::objectTypesList.size() == size_t(id));
		for (auto&[prototype, method] : members) {
			auto& func = DeclareDeviceFunction(name + "::" + prototype, [method](Computer* computer, const std::vector<Var>& args) -> Var {
				if (args.size() > 0) {
					return method(computer, args[0], std::vector<Var>(args.begin()+1, args.end()));
				} else {
					throw RuntimeError("Invalid object member arguments");
				}
			}, id);
			func.args.insert(func.args.begin(), Device::FunctionInfo::Arg{"_this", name});
		}
		return Var::Type(Var::Object | id);
	}

	// Implementation SHOULD set this function
	inline static void SetOutputFunction(OutputFunction&& func) {
		Device::outputFunction = forward<OutputFunction>(func);
	}

	struct Stack {
		int id; // a unique id, at least per function
		std::string type; // function, if, elseif, else, foreach, while, repeat, for
		std::unordered_map<std::string, uint32_t> pointers;
		Stack(int id_, std::string type_) : id(id_), type(type_), pointers() {}
	};

	struct ByteCode {
		union {
			uint32_t rawValue;
			struct {
				uint32_t value : 24; // 16 million possible values per type
				uint32_t type : 8; // 256 possible types
			};
		};
		ByteCode(uint32_t rawValue_ = 0) : rawValue(rawValue_) {}
		ByteCode(CODE_TYPE type_) : value(0), type(type_) {}
		ByteCode(uint8_t type_, uint32_t value_) : value(value_), type(type_) {}
		bool operator == (uint32_t other) const {
			return rawValue == other;
		}
		bool operator != (uint32_t other) const {
			return rawValue != other;
		}
		bool operator == (CODE_TYPE other) const {
			return type == other;
		}
		bool operator != (CODE_TYPE other) const {
			return type != other;
		}
		operator bool() const {
			if (type == RETURN) {
				return value != 0;
			} else if (type == VOID) {
				return value != 0;
			}
			return true;
		}
	};

	inline static CODE_TYPE GetRamVarType(uint32_t t) {
		switch (t) {
			case STORAGE_VAR_NUMERIC: return RAM_VAR_NUMERIC;
			case STORAGE_VAR_TEXT: return RAM_VAR_TEXT;
			case STORAGE_ARRAY_NUMERIC: return RAM_VAR_NUMERIC;
			case STORAGE_ARRAY_TEXT: return RAM_VAR_TEXT;
			case RAM_ARRAY_NUMERIC: return RAM_VAR_NUMERIC;
			case RAM_ARRAY_TEXT: return RAM_VAR_TEXT;
			case ROM_CONST_NUMERIC: return RAM_VAR_NUMERIC;
			case ROM_CONST_TEXT: return RAM_VAR_TEXT;
			default: return CODE_TYPE(t);
		}
	}

	inline static ByteCode GetOperator(std::string op) {
		if (op == "=") {
			return SET;
		} else if (op == "+" || op == "+=") {
			return ADD;
		} else if (op == "-" || op == "-=") {
			return SUB;
		} else if (op == "*" || op == "*=") {
			return MUL;
		} else if (op == "/" || op == "/=") {
			return DIV;
		} else if (op == "^" || op == "^=") {
			return POW;
		} else if (op == "%" || op == "%=") {
			return MOD;
		} else if (op == "&" || op == "&=") {
			return CCT;
		} else if (op == "++") {
			return INC;
		} else if (op == "--") {
			return DEC;
		} else if (op == "!!") {
			return NOT;
		} else {
			throw CompileError("Invalid Operator", op);
		}
	}

	inline static ByteCode GetBuiltInFunctionOp(const std::string& func, CODE_TYPE& returnType) {
		if (func == "floor") {returnType = RAM_VAR_NUMERIC; return FLR;}
		if (func == "ceil") {returnType = RAM_VAR_NUMERIC; return CIL;}
		if (func == "round") {returnType = RAM_VAR_NUMERIC; return RND;}
		if (func == "sin") {returnType = RAM_VAR_NUMERIC; return SIN;}
		if (func == "cos") {returnType = RAM_VAR_NUMERIC; return COS;}
		if (func == "tan") {returnType = RAM_VAR_NUMERIC; return TAN;}
		if (func == "asin") {returnType = RAM_VAR_NUMERIC; return ASI;}
		if (func == "acos") {returnType = RAM_VAR_NUMERIC; return ACO;}
		if (func == "atan") {returnType = RAM_VAR_NUMERIC; return ATA;}
		if (func == "abs") {returnType = RAM_VAR_NUMERIC; return ABS;}
		if (func == "fract") {returnType = RAM_VAR_NUMERIC; return FRA;}
		if (func == "sqrt") {returnType = RAM_VAR_NUMERIC; return SQR;}
		if (func == "sign") {returnType = RAM_VAR_NUMERIC; return SIG;}
		if (func == "pow") {returnType = RAM_VAR_NUMERIC; return POW;}
		if (func == "log") {returnType = RAM_VAR_NUMERIC; return LOG;}
		if (func == "clamp") {returnType = RAM_VAR_NUMERIC; return CLP;}
		if (func == "step") {returnType = RAM_VAR_NUMERIC; return STP;}
		if (func == "smoothstep") {returnType = RAM_VAR_NUMERIC; return SMT;}
		if (func == "lerp") {returnType = RAM_VAR_NUMERIC; return LRP;}
		if (func == "size") {returnType = RAM_VAR_NUMERIC; return SIZ;}
		if (func == "last") {returnType = VOID/*NUMERIC|TEXT*/; return LAS;}
		if (func == "find") {returnType = RAM_VAR_NUMERIC; return FND;}
		if (func == "contains") {returnType = RAM_VAR_NUMERIC; return CON;}
		if (func == "mod") {returnType = RAM_VAR_NUMERIC; return MOD;}
		if (func == "min") {returnType = RAM_VAR_NUMERIC; return MIN;}
		if (func == "max") {returnType = RAM_VAR_NUMERIC; return MAX;}
		if (func == "avg") {returnType = RAM_VAR_NUMERIC; return AVG;}
		if (func == "med") {returnType = RAM_VAR_NUMERIC; return MED;}
		if (func == "sum") {returnType = RAM_VAR_NUMERIC; return SUM;}
		if (func == "add") {returnType = RAM_VAR_NUMERIC; return ADD;}
		if (func == "sub") {returnType = RAM_VAR_NUMERIC; return SUB;}
		if (func == "mul") {returnType = RAM_VAR_NUMERIC; return MUL;}
		if (func == "div") {returnType = RAM_VAR_NUMERIC; return DIV;}
		if (func == "clear") {returnType = VOID; return CLR;}
		if (func == "append") {returnType = VOID; return APP;}
		if (func == "pop") {returnType = VOID; return POP;}
		if (func == "insert") {returnType = VOID; return INS;}
		if (func == "erase") {returnType = VOID; return DEL;}
		if (func == "fill") {returnType = VOID; return FLL;}
		if (func == "from") {returnType = VOID; return FRM;}
		if (func == "substring") {returnType = RAM_VAR_TEXT; return SBS;}
		if (func == "text") {returnType = RAM_VAR_TEXT; return TXT;}
		if (func == "sort") {returnType = VOID; return ASC;}
		if (func == "sortd") {returnType = VOID; return DSC;}
		if (func == "system.output") {returnType = VOID; return OUT;}
		if (func == "hash") {returnType = RAM_VAR_NUMERIC; return HSH;}
		if (func == "upper") {returnType = RAM_VAR_TEXT; return UPP;}
		if (func == "lower") {returnType = RAM_VAR_TEXT; return LCC;}
		if (func == "isnumeric") {returnType = RAM_VAR_NUMERIC; return ISN;}
		if (func == "if") {returnType = VOID /*NUMERIC|TEXT*/; return IFF;}
		returnType = VOID;
		return DEV;
	}

	// Is non-const non-array assignable var
	inline static bool IsVar(ByteCode v) {
		switch (v.type) {
			case STORAGE_VAR_NUMERIC:
			case STORAGE_VAR_TEXT:
			case RAM_VAR_NUMERIC:
			case RAM_VAR_TEXT:
			return true;
		}
		return v.type >= RAM_OBJECT;
	}

	inline static bool IsArray(ByteCode v) {
		switch (v.type) {
			case STORAGE_ARRAY_NUMERIC:
			case STORAGE_ARRAY_TEXT:
			case RAM_ARRAY_NUMERIC:
			case RAM_ARRAY_TEXT:
			return true;
		}
		return false;
	}

	inline static bool IsStorage(ByteCode v) {
		switch (v.type) {
			case STORAGE_ARRAY_NUMERIC:
			case STORAGE_ARRAY_TEXT:
			case STORAGE_VAR_NUMERIC:
			case STORAGE_VAR_TEXT:
			return true;
		}
		return false;
	}
	
	// Does not include arrays
	inline static bool IsNumeric(ByteCode v) {
		switch (v.type) {
			case ROM_CONST_NUMERIC:
			case STORAGE_VAR_NUMERIC:
			case RAM_VAR_NUMERIC:
			return true;
		}
		return false;
	}
	
	// Does not include arrays
	inline static bool IsText(ByteCode v) {
		switch (v.type) {
			case ROM_CONST_TEXT:
			case STORAGE_VAR_TEXT:
			case RAM_VAR_TEXT:
			return true;
		}
		return false;
	}

	inline static bool IsObject(ByteCode v) {
		return v.type >= RAM_OBJECT;
	}

	struct TimerFunction {
		double interval;
		uint32_t addr;
	};

	struct InputFunction {
		uint32_t addr = 0;
		std::vector<uint32_t> args {};
	};
	
	struct EntryPoint {
		std::string name;
		uint32_t addr = 0;
		ByteCode ref = VOID;
		std::vector<uint32_t> args {};
	};

	class Assembly {
		static inline const std::string parserFiletype = "XenonCode!";
		static inline const uint32_t parserVersionMajor = VERSION_MAJOR;
		static inline const uint32_t parserVersionMinor = VERSION_MINOR;
		
		static inline std::string GetDeviceObjectsList() {
			std::string str{"OBJ"};
			for (const auto& name : Device::objectTypesList) {
				str += ' ' + name;
			}
			return str;
		}
		
		static inline std::string GetDeviceFunctionsList(uint8_t base) {
			std::string str{"FN"};
			for (const auto& name : Device::deviceFunctionsList[base]) {
				str += ' ' + name;
			}
			return str;
		}
		
	public:
		uint32_t varsInitSize = 0; // number of byte codes in the vars_init code (uint32_t)
		uint32_t programSize = 0; // number of byte codes in the program code (uint32_t)
		std::vector<std::string> storageRefs {}; // storage references (addr)
		std::unordered_map<std::string, uint32_t> functionRefs {}; // function references (addr)
		std::vector<TimerFunction> timers {};
		std::map<uint32_t, InputFunction> inputs {};
		std::vector<EntryPoint> entryPoints {};
		std::vector<std::string> sourceFiles {};
		/* Function names:
			system.init
			system.tick
			system.input (not present in functionRefs)
			system.timer (not present in functionRefs)
		Varnames related to functions:
			@funcname.1 		argument
			@funcname:			return
		*/
		
		// ROM
		std::vector<ByteCode> rom_vars_init {}; // the bytecode of this computer's vars_init function
		std::vector<ByteCode> rom_program {}; // the actual bytecode of this program
		std::vector<double> rom_numericConstants {}; // the actual numeric constant values
		std::vector<std::string> rom_textConstants {}; // the actual text constant values
		
		// RAM size
		uint32_t ram_numericVariables = 0;
		uint32_t ram_textVariables = 0;
		uint32_t ram_objectReferences = 0;
		uint32_t ram_numericArrays = 0;
		uint32_t ram_textArrays = 0;
		
		// Helpers
		Word GetConstValue(ByteCode ref) const {
			if (ref.type == ROM_CONST_NUMERIC) {
				return {Word::Numeric, ToString(rom_numericConstants[ref.value])};
			} else if (ref.type == ROM_CONST_TEXT) {
				return {Word::Text, rom_textConstants[ref.value]};
			} else {
				throw CompileError("Not const");
			}
		}
		
		// From Parsed lines of code
		explicit Assembly(const std::vector<ParsedLine>& lines, bool verbose) {
			// Current context
			std::string currentFile = "";
			uint32_t currentLine = 0;
			int currentScope = 0;
			std::string currentFunctionName = "";
			double currentTimerInterval = 0;
			uint32_t currentInputPort = 0;
			uint32_t currentFunctionAddr = 0;
			int currentStackId = 0;
			std::vector<Stack> stack {};
			
			bool currentFunctionRecursive = false;

			// offset at start of current function
			uint32_t ram_numericVariables_offset = 0;
			uint32_t ram_textVariables_offset = 0;
			uint32_t ram_objectReferences_offset = 0;
			uint32_t ram_numericArrays_offset = 0;
			uint32_t ram_textArrays_offset = 0;
			
			// Temporary user-defined symbol maps
			std::unordered_map<std::string/*functionName*/, std::map<int/*stackId*/, std::unordered_map<std::string/*name*/, ByteCode>>> userVars {};
			
			// Validation helper
			auto validate = [](bool condition){
				if (!condition) {
					throw CompileError("Invalid statement");
				}
			};
			
			// Lambda functions to Get/Add user-defined symbols
			auto getVar = [&](const std::string& name) -> ByteCode {
				for (int s = stack.size()-1; s >= -1; --s) {
					if (userVars[currentFunctionName][s<0?0:stack[s].id].contains(name)) {
						return userVars.at(currentFunctionName).at(s<0?0:stack[s].id).at(name);
					}
				}
				if (currentFunctionName != "") {
					if (userVars[""][0].contains(name)) {
						return userVars.at("").at(0).at(name);
					}
				}
				throw CompileError("$" + name + " is undefined");
			};
			auto declareVar = [&](const std::string& name, CODE_TYPE type, Word initialValue/*Only for Const*/ = Word::Empty) -> ByteCode {
				if (name != "") {
					for (int s = stack.size()-1; s >= -1; --s) {
						if (userVars[currentFunctionName][s<0?0:stack[s].id].contains(name)) {
							throw CompileError("$" + name + " is already defined");
						}
					}
					if (currentFunctionName != "") {
						if (userVars[""][0].contains(name)) {
							throw CompileError("$" + name + " is already defined");
						}
					}
				}
				
				uint32_t index;
				switch (type) {
					case STORAGE_VAR_NUMERIC:
						if (name == "") {
							index = ram_numericVariables++;
							type = RAM_VAR_NUMERIC;
						} else {
							index = storageRefs.size();
							storageRefs.emplace_back(name);
						}
						break;
					case STORAGE_VAR_TEXT:
						if (name == "") {
							index = ram_textVariables++;
							type = RAM_VAR_TEXT;
						} else {
							index = storageRefs.size();
							storageRefs.emplace_back(name);
						}
						break;
					case STORAGE_ARRAY_NUMERIC:
						if (name == "") {
							index = ram_numericArrays++;
							type = RAM_ARRAY_NUMERIC;
						} else {
							index = storageRefs.size();
							storageRefs.emplace_back(name);
						}
						break;
					case STORAGE_ARRAY_TEXT:
						if (name == "") {
							index = ram_textArrays++;
							type = RAM_ARRAY_TEXT;
						} else {
							index = storageRefs.size();
							storageRefs.emplace_back(name);
						}
						break;
						
					case ROM_CONST_NUMERIC:
						if (!initialValue) throw CompileError("Const value not provided");
						index = rom_numericConstants.size();
						rom_numericConstants.emplace_back(initialValue);
						break;
					case ROM_CONST_TEXT:
						if (!initialValue) throw CompileError("Const value not provided");
						index = rom_textConstants.size();
						rom_textConstants.emplace_back(initialValue);
						break;
						
					case RAM_VAR_NUMERIC:
						index = ram_numericVariables++;
						break;
					case RAM_VAR_TEXT:
						index = ram_textVariables++;
						break;
					case RAM_ARRAY_NUMERIC:
						index = ram_numericArrays++;
						break;
					case RAM_ARRAY_TEXT:
						index = ram_textArrays++;
						break;
					
					case VOID:
						assert(!"Invalid var type VOID");
						break;
						
					default:
						if (type >= RAM_OBJECT) {
							index = ram_objectReferences++;
						} else {
							assert(!"Invalid CODE_TYPE");
						}
				}
				
				ByteCode byteCode{uint8_t(type), index};
				if (name != "") {
					userVars[currentFunctionName][currentStackId].emplace(name, byteCode);
				}
				return byteCode;
			};
			auto declareTmpNumeric = [&] {return declareVar("", RAM_VAR_NUMERIC);};
			auto declareTmpText = [&] {return declareVar("", RAM_VAR_TEXT);};
			auto getReturnVar = [&](const std::string& funcName) -> ByteCode {
				std::string retVarName = "@"+funcName+":";
				if (userVars.contains(funcName) && userVars.at(funcName).contains(0) && userVars.at(funcName).at(0).contains(retVarName)) {
					return userVars.at(funcName).at(0).at(retVarName);
				} else {
					throw CompileError("Function", funcName, "does not have a return type");
				}
			};
			auto getVarName = [&](ByteCode code) -> std::string {
				for (const auto& [fun,stacks] : userVars) {
					for (const auto& [s,vars] : stacks) {
						for (const auto& [name,byteCode] : vars) {
							if (byteCode.type == code.type && byteCode.value == code.value) {
								return name;
							}
						}
					}
				}
				return std::to_string(code.value);
			};
			auto getFunctionName = [&](ByteCode code) -> std::string {
				if (code.type == ADDR) {
					for (const auto& [func,address] : functionRefs) {
						if (code.value == address) {
							return func;
						}
					}
				}
				return std::to_string(code.value);
			};
			
			// Compile helpers
			auto addr = [&]{return rom_program.size();};
			auto write = [&](ByteCode c) -> uint32_t /*ThisAddr*/ {
				uint32_t address = addr();
				rom_program.emplace_back(c);
				return address;
			};
			auto jump = [&](uint32_t jumpToAddress) -> uint32_t /*AddrOfAddrToJumpTo*/ {
				rom_program.emplace_back(JMP);
				uint32_t address = addr();
				rom_program.emplace_back(ADDR, jumpToAddress);
				rom_program.emplace_back(VOID);
				return address;
			};
			auto gotoAddr = [&](uint32_t gotoToAddress) -> uint32_t /*AddrOfAddrToGoTo*/ {
				rom_program.emplace_back(GTO);
				uint32_t address = addr();
				rom_program.emplace_back(ADDR, gotoToAddress);
				rom_program.emplace_back(VOID);
				return address;
			};
			
			// Function helpers
			auto openFunction = [&](const std::string& name){
				assert(currentScope == 0);
				assert(currentFunctionName == "");
				ram_numericVariables_offset = ram_numericVariables;
				ram_textVariables_offset = ram_textVariables;
				ram_objectReferences_offset = ram_objectReferences;
				ram_numericArrays_offset = ram_numericArrays;
				ram_textArrays_offset = ram_textArrays;
		
				if (name != "system.timer" && name != "system.input" && !name.starts_with("entrypoint.") && functionRefs.contains(name)) {
					throw CompileError("Function " + name + " is already defined");
				}
				currentFunctionName = name;
				currentFunctionAddr = addr();
				if (currentFunctionName == "system.timer") {
					assert(currentTimerInterval);
					timers.emplace_back(currentTimerInterval, currentFunctionAddr);
					currentTimerInterval = 0;
					userVars["system.timer"].clear();
				} else if (currentFunctionName == "system.input") {
					inputs[currentInputPort].addr = currentFunctionAddr;
					currentInputPort = 0;
					userVars["system.input"].clear();
				} else if (currentFunctionName.starts_with("entrypoint.")) {
					assert(entryPoints.size() > 0);
					entryPoints.back().addr = currentFunctionAddr;
					userVars[currentFunctionName].clear();
				} else if (currentFunctionName != "") {
					functionRefs.emplace(currentFunctionName, currentFunctionAddr);
				}
			};
			auto closeCurrentFunction = [&](){
				if (currentFunctionName != "") {
					write(RETURN);
					currentFunctionName = "";
				}
				currentFunctionAddr = 0;
				currentFunctionRecursive = false;
				currentStackId = 0;
			};
			// If it's a user-declared function and it has a return type defined, returns the return var ref of that function, otherwise returns VOID
			auto compileFunctionCall = [&](Word func, const std::vector<ByteCode>& args, bool getReturn, bool isTrailingFunction = false, bool recursive = false) -> ByteCode {
				std::string funcName = func;
				if (func == Word::Funcname) {
					if (!functionRefs.contains(funcName)) {
						throw CompileError("Function", func, "is not defined");
					}

					// recursive calls only allowed with self
					if (funcName == currentFunctionName && !recursive) {
						throw CompileError("You can only recurse in functions marked as recursive through calling recurse");
					}
					
					// Set arguments
					int i = 0;
					for (auto arg : args) {
						std::string argVarName = "@"+funcName+"."+std::to_string(++i);
						if (userVars.contains(funcName) && userVars.at(funcName).contains(0) && userVars.at(funcName).at(0).contains(argVarName)) {
							ByteCode param = userVars.at(funcName).at(0).at(argVarName);
							if (IsArray(arg)) { // Don't allow arrays to be passed as arguments
								throw CompileError("Cannot pass an array to function", func, "in arg " + std::to_string(i));
							}
							write(SET);
							write(param);
							write(arg);
							write(VOID);
						} else break;
					}
					
					// Call the function
					jump(functionRefs.at(funcName));
					
					// Get the Return value
					if (getReturn) {
						ByteCode ret = getReturnVar(funcName);
						if (ret.type != VOID) {
							ByteCode tmp = declareVar("", GetRamVarType(ret.type));
							write(SET);
							write(tmp);
							write(ret);
							write(VOID);
							return tmp;
						} else {
							throw CompileError("A function call here should return a value, but", funcName, "does not");
						}
					} else if (isTrailingFunction) {
						validate(args.size() > 0);
						if (IsArray(args[0])) { // Don't allow arrays to be passed as arguments
							throw CompileError("Cannot pass an array to function", func, "in arg 1");
						}
						if (args[0].type < RAM_OBJECT) { // Objects don't get returned from trailing functions
							write(SET);
							write(args[0]);
							write(getReturnVar(funcName));
							write(VOID);
						}
					}
					return VOID;
				} else if (func == Word::Name) {
					if (getReturn && isTrailingFunction && args.size() == 1 && (args[0].type == ROM_CONST_TEXT || args[0].type == RAM_VAR_TEXT || args[0].type == STORAGE_VAR_TEXT)) {
						ByteCode ret = declareTmpText();
						write(IDX);
						write(ret);
						write(args[0]);
						write(OBJ_KEY);
						write(declareVar("", ROM_CONST_TEXT, func));
						write(VOID);
						return ret;
					}
					CODE_TYPE retType = VOID;
					ByteCode ret = VOID;
					ByteCode f = VOID;
					if (isTrailingFunction && args.size() > 0 && args[0].type >= RAM_OBJECT) {
						std::string objType = Device::objectNamesById[args[0].type & (RAM_OBJECT-1)];
						if (Device::deviceFunctionsByName.contains(objType+"::"+funcName)) {
							funcName = objType+"::"+funcName;
							f = DEV;
						}
					}
					if (f.type == VOID) {
						f = GetBuiltInFunctionOp(funcName, retType);
					}
					write(f);
					if (f == DEV) {
						if (!Device::deviceFunctionsByName.contains(funcName)) {
							throw CompileError("Function", func, "does not exist");
						}
						auto& function = Device::deviceFunctionsByName.at(funcName);
						write({DEVICE_FUNCTION_INDEX, function.id});
						if (getReturn) {
							if (function.returnType == "number") {
								retType = RAM_VAR_NUMERIC;
							} else if (function.returnType == "text") {
								retType = RAM_VAR_TEXT;
							} else if (function.returnType == "") {
								throw CompileError("A function call here should return a value, but", funcName, "does not");
							} else {
								validate(Device::objectTypesByName.contains(function.returnType));
								retType = CODE_TYPE(RAM_OBJECT | Device::objectTypesByName.at(function.returnType).id);
							}
						} else if (function.returnType != "" && !isTrailingFunction) {
							write(DISCARD); // We may discard the return value of a device function
						}
					} else if (f == LAS) {
						if (getReturn) {
							if (args.size() == 0) {
								throw CompileError("Invalid arguments");
							}
							switch (args[0].type) {
								case STORAGE_ARRAY_NUMERIC:
								case RAM_ARRAY_NUMERIC:
									retType = RAM_VAR_NUMERIC;
								break;
								case ROM_CONST_TEXT:
								case STORAGE_VAR_TEXT:
								case STORAGE_ARRAY_TEXT:
								case RAM_VAR_TEXT:
								case RAM_ARRAY_TEXT:
									retType = RAM_VAR_TEXT;
								break;
								default: throw CompileError("Invalid argument type");
							}
						} else {
							throw CompileError("Cannot call", funcName, "here");
						}
					} else if (f == IFF) {
						if (getReturn) {
							if (args.size() != 3) {
								throw CompileError("Invalid arguments");
							}
							switch (args[1].type) {
								case RAM_VAR_NUMERIC:
								case ROM_CONST_NUMERIC:
								case STORAGE_VAR_NUMERIC:
									retType = RAM_VAR_NUMERIC;
								break;
								case RAM_VAR_TEXT:
								case ROM_CONST_TEXT:
								case STORAGE_VAR_TEXT:
									retType = RAM_VAR_TEXT;
								break;
								default: throw CompileError("Invalid argument type");
							}
						} else {
							throw CompileError("Cannot call", funcName, "here");
						}
					}
					
					if (getReturn) {
						if (retType != VOID) {
							ret = declareVar("", retType);
							write(ret);
						} else {
							throw CompileError("A function call here should return a value, but", funcName, "does not");
						}
					} else if (isTrailingFunction) {
						if (retType != VOID) {
							validate(args.size() > 0);
							write(args[0]);
						}
					} else if (retType != VOID) {
						throw CompileError("A function call here should NOT return a value, but", funcName, "does");
					}
					
					for (auto arg : args) {
						write(arg);
					}
					
					write(VOID);
					return ret;
				} else {
					throw CompileError("Invalid function name");
				}
			};

			auto compileSelfCall = [&](const std::vector<ByteCode>& args, bool getReturn) -> ByteCode {
				if (!currentFunctionRecursive) {
					throw CompileError("Cannot call recurse in non-recursive function or global scope");
				}
				uint32_t ram_numericVariables_d = ram_numericVariables - ram_numericVariables_offset;
				uint32_t ram_textVariables_d = ram_textVariables - ram_textVariables_offset;
				uint32_t ram_objectReferences_d = ram_objectReferences - ram_objectReferences_offset;
				uint32_t ram_numericArrays_d = ram_numericArrays - ram_numericArrays_offset;
				uint32_t ram_textArrays_d = ram_textArrays - ram_textArrays_offset;
				auto writeRecurse = [&] (uint32_t op) {
					if (ram_numericVariables_d > 0) {
						write(op);
						write(ram_numericVariables_offset);
						write(ram_numericVariables_d);
						write(ByteCode(RAM_VAR_NUMERIC));
					}
					if (ram_textVariables_d > 0) {
						write(op);
						write(ram_textVariables_offset);
						write(ram_textVariables_d);
						write(ByteCode(RAM_VAR_TEXT));
					}
					if (ram_objectReferences_d > 0) {
						write(op);
						write(ram_objectReferences_offset);
						write(ram_objectReferences_d);
						write(ByteCode(RAM_OBJECT));
					}
					if (ram_numericArrays_d > 0) {
						write(op);
						write(ram_numericArrays_offset);
						write(ram_numericArrays_d);
						write(ByteCode(RAM_ARRAY_NUMERIC));
					}
					if (ram_textArrays_d > 0) {
						write(op);
						write(ram_textArrays_offset);
						write(ram_textArrays_d);
						write(ByteCode(RAM_ARRAY_TEXT));
					}
				};

				writeRecurse(STR);
				auto ref = compileFunctionCall(Word(Word::Type::Funcname, currentFunctionName), args, getReturn, false, true);
				writeRecurse(RST);
				return ref;
			};
			// Stack helpers
			auto addPointer = [&](std::string reg) -> uint32_t& {
				if (reg == "") reg = std::to_string(stack.back().pointers.size()+1);
				return stack.back().pointers[reg];
			};
			//auto getPointer = [&](const std::string& reg) -> ByteCode {
			//	assert(stack.size() > 0);
			//	if (stack.back().pointers.contains(reg)) {
			//		return rom_program[stack.back().pointers.at(reg)];
			//	}
			//};
			auto applyPointerAddr = [&](const std::string& reg){
				assert(stack.size() > 0);
				if (stack.back().pointers.contains(reg)) {
					rom_program[stack.back().pointers.at(reg)].value = addr();
				}
			};
			auto applyPointersAddresses = [&]{
				assert(stack.size() > 0);
				for (auto& [name, r] : stack.back().pointers) {
					assert(r < rom_program.size());
					if (rom_program[r].type == ADDR && rom_program[r].value == 0) {
						rom_program[r].value = addr();
					}
				}
				stack.back().pointers.clear();
			};
			auto pushStack = [&](const std::string& type) {
				++currentScope;
				stack.emplace_back(++currentStackId, type);
				assert(currentScope == (int)stack.size());
			};
			auto popStack = [&] {
				assert(currentScope >= 0);
				assert(stack.size() > 0);
				if (stack.back().type == "loop") {
					gotoAddr(stack.back().pointers["LoopBegin"]);
				}
				applyPointersAddresses();
				--currentScope;
				stack.pop_back();
				userVars[currentFunctionName][currentStackId].clear();
				currentStackId = stack.size() > 0 ? stack.back().id : 0;
				assert(currentScope == (int)stack.size());
			};
			
			// Expression helpers
			/*computeConstExpression*/ std::function<Word(const std::vector<Word>&, int, int)> computeConstExpression = [&](const std::vector<Word>& words, int startIndex, int endIndex = -1) -> Word {
				if (endIndex < 0) endIndex += words.size();
				validate(startIndex+1 < endIndex); // Need at least 3 words here
				int opIndex = startIndex + 1;
				
				// Get Word 1
				Word word1 = words[startIndex];
				if (word1 == Word::ExpressionBegin) {
					int closing = GetExpressionEnd(words, startIndex, endIndex);
					validate(closing != -1);
					word1 = computeConstExpression(words, startIndex+1, closing-1);
					validate(closing + 1 < endIndex);
					opIndex = closing + 1;
				}
				if (word1 == Word::Varname) {
					word1 = GetConstValue(getVar(word1));
				}
				validate(word1 == Word::Numeric || word1 == Word::Text || word1 == Word::Void);
				
				// Get Operator
				Word op = words[opIndex];
				validate(op.type >= WORD_ENUM_OPERATOR_START);
				
				// Get Word 2
				int word2Index = opIndex + 1;
				Word word2 = words[word2Index];
				if (word2 == Word::ExpressionBegin) {
					int closing = GetExpressionEnd(words, word2Index, endIndex);
					validate(closing != -1);
					word2 = computeConstExpression(words, word2Index+1, closing-1);
					validate(closing == endIndex); // Is there no more words remaining?
					word2Index = closing;
				}
				if (word2 == Word::Varname) {
					word2 = GetConstValue(getVar(word2));
				}
				validate(word2 == Word::Numeric || word2 == Word::Text || word2 == Word::Void);
				
				// Validate that there are no more words remaining
				validate(word2Index == endIndex);
				
				// Compute result
				if (op == Word::CastOperator) {
					if (word2 == "number") {
						word1.type = Word::Numeric;
						return word1;
					} else if (word2 == "text") {
						word1.type = Word::Text;
						return word1;
					} else {
						throw CompileError("Const expression may only be cast to either 'number' or 'text'");
					}
				} else if (op == Word::NotOperator) {
					if (word2 == Word::Numeric) {
						double value = word2;
						word2 = double(value == 0.0);
						return word2;
					} else if (word2 == Word::Text) {
						std::string value = word2;
						word2 = double(value == "");
						return word2;
					} else {
						validate(false);
					}
				} else if (op == Word::ConcatOperator) {
					validate(word1 == Word::Text && word2 == Word::Text);
					word1.word += word2.word;
					return word1;
				} else if (op == Word::MulOperatorGroup) {
					validate(word1 == Word::Numeric && word2 == Word::Numeric);
					if (op == "*") {
						return Word{double(word1) * double(word2)};
					} else if (op == "/") {
						if (double(word2) == 0.0) {
							throw CompileError("Division by zero in const expression");
						}
						return Word{double(word1) / double(word2)};
					} else if (op == "^") {
						return Word{std::pow(double(word1), double(word2))};
					} else if (op == "%") {
						if (double(word2) == 0.0) {
							throw CompileError("Division by zero in const expression");
						}
						return Word{double(int64_t(std::round(double(word1))) % int64_t(std::round(double(word2))))};
					} else {
						validate(false);
					}
				} else if (op == Word::AddOperatorGroup) {
					validate(word1 == Word::Numeric && word2 == Word::Numeric);
					if (op == "+") {
						return Word{double(word1) + double(word2)};
					} else if (op == "-") {
						return Word{double(word1) - double(word2)};
					} else {
						validate(false);
					}
				} else if (op == Word::CompareOperatorGroup) {
					validate(word1 == Word::Numeric && word2 == Word::Numeric);
					if (op == "<") {
						return Word{double(double(word1) < double(word2))};
					} else if (op == "<=") {
						return Word{double(double(word1) <= double(word2))};
					} else if (op == ">") {
						return Word{double(double(word1) > double(word2))};
					} else if (op == ">=") {
						return Word{double(double(word1) >= double(word2))};
					} else {
						validate(false);
					}
				} else if (op == Word::EqualityOperatorGroup) {
					if (word1 == Word::Numeric && word2 == Word::Numeric) {
						if (op == "==") {
							return Word{double(double(word1) == double(word2))};
						} else if (op == "!=" || op == "<>") {
							return Word{double(double(word1) != double(word2))};
						} else {
							validate(false);
						}
					} else if (word1 == Word::Text && word2 == Word::Text) {
						if (op == "==") {
							return Word{double(std::string(word1) == std::string(word2))};
						} else if (op == "!=" || op == "<>") {
							return Word{double(std::string(word1) != std::string(word2))};
						} else {
							validate(false);
						}
					} else {
						validate(false);
					}
				} else if (op == Word::AndOperator) {
					validate(word1 == Word::Numeric && word2 == Word::Numeric);
					return Word{double(double(word1) && double(word2))};
				} else if (op == Word::OrOperator) {
					validate(word1 == Word::Numeric && word2 == Word::Numeric);
					return Word{double(double(word1) || double(word2))};
				} else if (op == Word::XorOperator) {
					validate(word1 == Word::Numeric && word2 == Word::Numeric);
					return Word{double(!!(double(word1)) != !!(double(word2)))};
				}
				throw CompileError("Invalid operator", op, "in const expression");
			};
			/*compileExpression*/ std::function<ByteCode(const std::vector<Word>&, int, int)> compileExpression = [&](const std::vector<Word>& words, int startIndex, int endIndex = -1) -> ByteCode {
				if (endIndex < 0) endIndex += words.size();
				validate(startIndex <= endIndex);
				
				int opIndex = startIndex + 1;
				
				// Word 1
				Word word1 = words[startIndex];
				ByteCode ref1;
				switch (word1.type) {
					case Word::ExpressionBegin:{
						int closing = GetExpressionEnd(words, startIndex, endIndex);
						validate(closing != -1);
						ref1 = compileExpression(words, startIndex+1, closing-1);
						if (closing == endIndex) return ref1;
						opIndex = closing + 1;
						validate(opIndex < endIndex);
					}break;
					case Word::Varname:{
						ref1 = getVar(word1);
						if (startIndex == endIndex) return ref1;
					}break;
					case Word::Numeric:{
						ref1 = declareVar("", ROM_CONST_NUMERIC, word1);
						if (startIndex == endIndex) return ref1;
					}break;
					case Word::Text:{
						ref1 = declareVar("", ROM_CONST_TEXT, word1);
						if (startIndex == endIndex) return ref1;
					}break;
					case Word::Funcname:
					case Word::Name:{
						std::vector<ByteCode> args {};
						if (size_t(startIndex+1) < words.size() && words[startIndex+1] == Word::ExpressionBegin) {
							int argIndex = startIndex+2;
							while (argIndex <= endIndex) {
								int argEnd = GetArgEnd(words, argIndex, endIndex);
								if (argEnd == -1) {
									++argIndex;
									break;
								}
								args.push_back(compileExpression(words, argIndex, argEnd));
								argIndex = argEnd + 2;
								if (argEnd+1 > endIndex || words[argEnd+1] != Word::CommaOperator) {
									break;
								}
							}
							opIndex = argIndex;
						} else {
							opIndex = startIndex + 1;
						}
						
						if (word1 == "recurse") {
							ref1 = compileSelfCall(args, true);
						} else {
							ref1 = compileFunctionCall(word1, args, true);
						}
						if (opIndex > endIndex) return ref1;
						validate(opIndex != endIndex);
					}break;
					case Word::Void:
						ref1 = VOID;
						break;
					default: validate(false);
				}
				
				// Get Operator
				Word op = words[opIndex];
				validate(op.type >= WORD_ENUM_OPERATOR_START);
				validate(opIndex < endIndex);
				
				// Get Word 2 and compile operation
				if (op == Word::CastOperator) {
					Word t = words[opIndex+1];
					if (t == "number") {
						ByteCode tmp = declareTmpNumeric();
						write(NUM);
						write(tmp);
						write(ref1);
						write(VOID);
						return tmp;
					} else if (t == "text") {
						ByteCode tmp = declareTmpText();
						write(TXT);
						write(tmp);
						write(ref1);
						write(VOID);
						return tmp;
					} else {
						throw CompileError("Invalid cast", t);
					}
				} else if (op == Word::TrailOperator) {
					Word operand = words[opIndex+1];
					if (operand == Word::Name || operand == Word::Funcname) {
						if (opIndex+1 == endIndex) { // trailing member with no parenthesis, in an expression
							validate(IsArray(ref1) || IsText(ref1) || IsObject(ref1));
							return compileFunctionCall(operand, {ref1}, true, true);
						} else if (IsObject(ref1)) { // trailing function call, in an expression, on an object
							validate(words[opIndex+2] == Word::ExpressionBegin);
							std::vector<ByteCode> args {};
							args.push_back(ref1);
							int argBegin = opIndex+3;
							while (argBegin <= endIndex) {
								int argEnd = GetArgEnd(words, argBegin, endIndex);
								if (argEnd == -1) {
									++argBegin;
									break;
								}
								args.push_back(compileExpression(words, argBegin, argEnd));
								argBegin = argEnd + 2;
								if (argEnd+1 > endIndex || words[argEnd+1] != Word::CommaOperator) {
									break;
								}
							}
							return compileFunctionCall(operand, args, true, true);
						} else {
							throw CompileError("A non-object-member trailing function call is invalid within an expression"); // because as per the spec, a trailing function call WILL modify the value of the variable that it's called on, defined by what it returns
						}
					} else if (operand == Word::Numeric || operand == Word::Varname) {
						validate(IsArray(ref1) || IsText(ref1));
						ByteCode tmp = declareVar("", GetRamVarType(ref1.type));
						write(IDX);
						write(tmp);
						write(ref1);
						if (operand == Word::Varname) {
							ByteCode ref2 = getVar(operand);
							if (IsNumeric(ref2)) {
								write({ARRAY_INDEX, ARRAY_INDEX_NONE});
							} else {
								validate(IsText(ref2));
								write(OBJ_KEY);
							}
							write(ref2);
						} else /*numeric*/{
							write({ARRAY_INDEX, uint32_t(std::round(double(operand)))});
						}
						write(VOID);
						return tmp;
					} else {
						validate(false);
						return VOID;
					}
				} else {
					ByteCode ref2 = compileExpression(words, opIndex+1, endIndex);
					
					// Compile operation
					if (op == Word::NotOperator) {
						validate(ref1 == VOID);
						write(NOT);
						ByteCode tmp = declareTmpNumeric();
						write(tmp);
						write(ref2);
						write(VOID);
						return tmp;
					} else if (op == Word::ConcatOperator) {
						ByteCode tmp = declareTmpText();
						write(CCT);
						write(tmp);
						write(ref1);
						write(ref2);
						write(VOID);
						return tmp;
					} else if (op == Word::MulOperatorGroup) {
						if (op == "*") {
							write(MUL);
						} else if (op == "/") {
							write(DIV);
						} else if (op == "^") {
							write(POW);
						} else if (op == "%") {
							write(MOD);
						} else {
							validate(false);
							return VOID;
						}
						ByteCode tmp = declareTmpNumeric();
						write(tmp);
						write(ref1);
						write(ref2);
						write(VOID);
						return tmp;
					} else if (op == Word::AddOperatorGroup) {
						if (op == "+") {
							write(ADD);
						} else if (op == "-") {
							write(SUB);
						} else {
							validate(false);
							return VOID;
						}
						ByteCode tmp = declareTmpNumeric();
						write(tmp);
						write(ref1);
						write(ref2);
						write(VOID);
						return tmp;
					} else if (op == Word::CompareOperatorGroup) {
						if (op == "<") {
							write(LST);
						} else if (op == ">") {
							write(GRT);
						} else if (op == "<=") {
							write(LTE);
						} else if (op == ">=") {
							write(GTE);
						} else {
							validate(false);
							return VOID;
						}
						ByteCode tmp = declareTmpNumeric();
						write(tmp);
						write(ref1);
						write(ref2);
						write(VOID);
						return tmp;
					} else if (op == Word::EqualityOperatorGroup) {
						if (op == "==") {
							write(EQQ);
						} else if (op == "!=" || op == "<>") {
							write(NEQ);
						} else {
							validate(false);
							return VOID;
						}
						ByteCode tmp = declareTmpNumeric();
						write(tmp);
						write(ref1);
						write(ref2);
						write(VOID);
						return tmp;
					} else if (op == Word::AndOperator) {
						ByteCode tmp = declareTmpNumeric();
						write(AND);
						write(tmp);
						write(ref1);
						write(ref2);
						write(VOID);
						return tmp;
					} else if (op == Word::OrOperator) {
						ByteCode tmp = declareTmpNumeric();
						write(ORR);
						write(tmp);
						write(ref1);
						write(ref2);
						write(VOID);
						return tmp;
					} else if (op == Word::XorOperator) {
						ByteCode tmp = declareTmpNumeric();
						write(XOR);
						write(tmp);
						write(ref1);
						write(ref2);
						write(VOID);
						return tmp;
					}
					
					throw CompileError("Invalid operator", op);
				}
			};
			
			// Add a Return+VOID at addr 0
			write(RETURN);
			write(VOID);
			
			// Start parsing
			try {
				for (const auto& line : lines) if (line) {
					currentLine = line.line;
					int nextWordIndex = 0;
					auto readWord = [&] (Word::Type type = Word::Empty) -> Word {
						if (nextWordIndex < (int)line.words.size()) {
							validate(type == Word::Empty || type == line.words[nextWordIndex].type);
							return line.words[nextWordIndex++];
						}
						return Word::Empty;
					};
					auto firstWord = readWord();
					if (line.scope == 0) {
						// Global Scope
						switch (firstWord.type) {
							case Word::FileInfo:{
								currentFile = firstWord.word;
								write({SOURCEFILE, uint32_t(sourceFiles.size())});
								sourceFiles.emplace_back(currentFile);
							}break;
							case Word::Name: {
								
								// Adjust scope
								while (line.scope < currentScope) {
									popStack();
								}
								
								// Close current function, if any
								closeCurrentFunction();
								
								// Parse declarations from global scope
								
								// const
								if (firstWord == "const") {
									std::string name = readWord(Word::Varname);
									validate(readWord() == "=");
									Word value = readWord();
									validate(value);
									// Number literal
									if (value == Word::Numeric) {
										declareVar(name, ROM_CONST_NUMERIC, value);
										validate(!readWord());
									}
									// Text literal
									else if (value == Word::Text) {
										declareVar(name, ROM_CONST_TEXT, value);
										validate(!readWord());
									}
									// Varname
									else if (value == Word::Varname) {
										ByteCode ref = getVar(value);
										declareVar(name, CODE_TYPE(ref.type), GetConstValue(ref));
										validate(!readWord());
									}
									// Expression
									else if (value == Word::ExpressionBegin) {
										int closingIndex = GetExpressionEnd(line.words, nextWordIndex-1);
										validate(closingIndex != -1);
										value = computeConstExpression(line.words, nextWordIndex, closingIndex-1);
										if (value == Word::Numeric) {
											declareVar(name, ROM_CONST_NUMERIC, value);
										} else if (value == Word::Text) {
											declareVar(name, ROM_CONST_TEXT, value);
										} else {
											validate(false);
										}
									}
									// Error
									else {
										throw CompileError("Cannot assign non-const expression to const value");
									}
								}
								// var
								else if (firstWord == "var") {
									std::string name = readWord(Word::Varname);
									Word op = readWord();
									if (op == "=") {
										Word value = readWord();
										validate(value);
										// Determine Type
										if (value == Word::Numeric) {
											ByteCode var = declareVar(name, RAM_VAR_NUMERIC);
											if (double(value) != 0.0) {
												ByteCode con = declareVar(name+".init", ROM_CONST_NUMERIC, value);
												rom_vars_init.emplace_back(SET);
												rom_vars_init.emplace_back(var);
												rom_vars_init.emplace_back(con);
												rom_vars_init.emplace_back(VOID);
											}
										} else if (value == Word::Text) {
											ByteCode var = declareVar(name, RAM_VAR_TEXT);
											if (std::string(value) != "") {
												ByteCode con = declareVar(name+".init", ROM_CONST_TEXT, value);
												rom_vars_init.emplace_back(SET);
												rom_vars_init.emplace_back(var);
												rom_vars_init.emplace_back(con);
												rom_vars_init.emplace_back(VOID);
											}
										} else if (value == Word::Varname) {
											
											ByteCode var;
											ByteCode ref = getVar(value);
											
											switch (ref.type) {
												case RAM_VAR_NUMERIC:
												case ROM_CONST_NUMERIC:
												case STORAGE_VAR_NUMERIC:
													var = declareVar(name, RAM_VAR_NUMERIC);
													break;
												case RAM_VAR_TEXT:
												case ROM_CONST_TEXT:
												case STORAGE_VAR_TEXT:
													var = declareVar(name, RAM_VAR_TEXT);
													break;
												default: throw CompileError("Invalid assignment");
											}
											
											rom_vars_init.emplace_back(SET);
											rom_vars_init.emplace_back(var);
											rom_vars_init.emplace_back(ref);
											rom_vars_init.emplace_back(VOID);
											
										} else if (value == Word::ExpressionBegin) {
											
											int closingIndex = GetExpressionEnd(line.words, nextWordIndex-1);
											validate(closingIndex != -1);
											value = computeConstExpression(line.words, nextWordIndex, closingIndex-1);
											
											ByteCode var;
											ByteCode con;
											
											if (value == Word::Numeric) {
												var = declareVar(name, RAM_VAR_NUMERIC);
												con = declareVar(name+".init", ROM_CONST_NUMERIC, value);
											} else if (value == Word::Text) {
												var = declareVar(name, RAM_VAR_TEXT);
												con = declareVar(name+".init", ROM_CONST_TEXT, value);
											} else {
												validate(false);
											}
											
											rom_vars_init.emplace_back(SET);
											rom_vars_init.emplace_back(var);
											rom_vars_init.emplace_back(con);
											rom_vars_init.emplace_back(VOID);
											
										} else if (value == Word::Name) {
											
											std::function<ByteCode(const std::string&)> compileConstFunctionCall = [&](const std::string& funcName) -> ByteCode {
											
												CODE_TYPE retType;
												
												if (!Device::deviceFunctionsByName.contains(funcName)) {
													throw CompileError("Function", funcName, "does not exist");
												}
												auto& function = Device::deviceFunctionsByName.at(funcName);
												if (function.returnType == "number") {
													retType = RAM_VAR_NUMERIC;
												} else if (function.returnType == "text") {
													retType = RAM_VAR_TEXT;
												} else if (function.returnType == "") {
													throw CompileError("A function call here should return a value, but", funcName, "does not");
												} else {
													validate(Device::objectTypesByName.contains(function.returnType));
													retType = CODE_TYPE(RAM_OBJECT | Device::objectTypesByName.at(function.returnType).id);
												}
												
												ByteCode var = declareVar(name, retType);
												name = ""; // Do not declare the variable again for recursive calls
												std::vector<ByteCode> args {};
												
												if (Word nextWord = readWord(); nextWord && nextWord == Word::ExpressionBegin) {
													for (int argIndex = 0; nextWordIndex < (int)line.words.size(); ++argIndex) {
														int argEnd = GetArgEnd(line.words, nextWordIndex);
														if (argEnd == -1) {
															break;
														}
														
														Word arg = readWord();
														if (arg == Word::CommaOperator) {
															continue;
														}
														if (arg == Word::Varname) {
															ByteCode ref = getVar(arg);
															arg = GetConstValue(ref);
														} else if (arg == Word::ExpressionBegin) {
															arg = computeConstExpression(line.words, nextWordIndex, argEnd-1);
														}
														if (arg == Word::Numeric) {
															args.emplace_back(declareVar("", ROM_CONST_NUMERIC, arg));
														} else if (arg == Word::Text) {
															args.emplace_back(declareVar("", ROM_CONST_TEXT, arg));
														} else if (arg == Word::Name) {
															args.emplace_back(compileConstFunctionCall(arg.word));
														} else {
															validate(false);
														}
														nextWordIndex = argEnd+1;
													}
												}
												
												rom_vars_init.emplace_back(DEV);
												rom_vars_init.emplace_back(DEVICE_FUNCTION_INDEX, function.id);
												rom_vars_init.emplace_back(var);
												for (auto arg : args) {
													rom_vars_init.emplace_back(arg);
												}
												rom_vars_init.emplace_back(VOID);
											
												return var;
											};
											
											compileConstFunctionCall(value.word);
										} else {
											validate(false);
										}
									} else if (op == ":") {
										Word type = readWord();
										if (type == "number") {
											declareVar(name, RAM_VAR_NUMERIC);
										} else if (type == "text") {
											declareVar(name, RAM_VAR_TEXT);
										} else {
											throw CompileError("Var declaration in global scope can only be of type 'number' or 'text'");
										}
										validate(!readWord());
									}
								}
								// array
								else if (firstWord == "array") {
									std::string name = readWord();
									validate(name != "");
									validate(readWord() == ":");
									Word type = readWord();
									if (type == "number") {
										declareVar(name, RAM_ARRAY_NUMERIC);
									} else if (type == "text") {
										declareVar(name, RAM_ARRAY_TEXT);
									} else {
										throw CompileError("Array declaration in global scope can only be of type 'number' or 'text'");
									}
									validate(!readWord());
								}
								// storage
								else if (firstWord == "storage") {
									std::string what = readWord();
									validate(what == "var" || what == "array");
									std::string name = readWord();
									validate(name != "");
									validate(readWord() == ":");
									Word type = readWord();
									if (type == "number") {
										if (what == "var") {
											declareVar(name, STORAGE_VAR_NUMERIC);
										} else if (what == "array") {
											declareVar(name, STORAGE_ARRAY_NUMERIC);
										} else {
											validate(false);
										}
									} else if (type == "text") {
										if (what == "var") {
											declareVar(name, STORAGE_VAR_TEXT);
										} else if (what == "array") {
											declareVar(name, STORAGE_ARRAY_TEXT);
										} else {
											validate(false);
										}
									} else {
										throw CompileError("Storage declaration in global scope can only be of type 'number' or 'text'");
									}
									validate(!readWord());
								}
								// init
								else if (firstWord == "init") {
									openFunction("system.init");
									pushStack("function");
								}
								// tick
								else if (firstWord == "tick") {
									openFunction("system.tick");
									pushStack("function");
								}
								// function
								else if (firstWord == "function" || firstWord == "recursive") {
									if (firstWord == "recursive") {
										currentFunctionRecursive = true;
										firstWord = readWord(Word::Name);
									}
									std::string name = readWord(Word::Funcname);
									openFunction(name);
									readWord(Word::ExpressionBegin);
									// Arguments
									int argN = 0;
									for (;;) {
										Word word = readWord();
										if (!word || word == Word::ExpressionEnd) break;
										if (word == Word::CommaOperator) {
											validate(argN > 0);
											word = readWord();
										}
										++argN;
										validate(word == Word::Varname);
										readWord(Word::CastOperator);
										std::string type = readWord(Word::Name);
										ByteCode arg;
										if (type == "number") {
											arg = declareVar(word, RAM_VAR_NUMERIC);
										} else if (type == "text") {
											arg = declareVar(word, RAM_VAR_TEXT);
										} else {
											if (Device::objectTypesByName.contains(type)) {
												arg = declareVar(word, CODE_TYPE(RAM_OBJECT | Device::objectTypesByName.at(type).id));
											} else {
												throw CompileError("Invalid argument type in function declaration");
											}
										}
										userVars[currentFunctionName][0].emplace("@"+name+"."+std::to_string(argN), arg);
									}
									// Return type
									if (readWord() == Word::CastOperator) {
										Word type = readWord(Word::Name);
										if (type == "number") {
											declareVar("@"+name+":", RAM_VAR_NUMERIC);
										} else if (type == "text") {
											declareVar("@"+name+":", RAM_VAR_TEXT);
										} else {
											if (Device::objectTypesByName.contains(type)) {
												declareVar("@"+name+":", CODE_TYPE(RAM_OBJECT | Device::objectTypesByName.at(type).id));
											} else {
												throw CompileError("Invalid return type in function declaration");
											}
										}
									}
									pushStack("function");
								}
								// timer
								else if (firstWord == "timer") {
									std::string timerType = readWord(Word::Name);
									Word timerValue = readWord();
									if (timerValue == Word::Varname) {
										timerValue = GetConstValue(getVar(timerValue));
									}
									validate(timerValue == Word::Numeric);
									currentTimerInterval = double(timerValue);
									if (timerType == "frequency") {
										currentTimerInterval = 1.0 / currentTimerInterval;
									}
									openFunction("system.timer");
									pushStack("function");
								}
								// input
								else if (firstWord == "input") {
									readWord(Word::TrailOperator);
									Word inputPort = readWord();
									if (inputPort == Word::Varname) {
										inputPort = GetConstValue(getVar(inputPort));
									}
									validate(inputPort == Word::Numeric);
									currentInputPort = uint32_t(double(inputPort));
									if (inputs.contains(currentInputPort)) {
										throw CompileError("Input " + std::to_string(currentInputPort) + " is already defined");
									}
									auto& args = inputs[currentInputPort].args;
									openFunction("system.input");
									readWord(Word::ExpressionBegin);
									// Arguments
									int argN = 0;
									for (;;) {
										Word word = readWord();
										if (!word || word == Word::ExpressionEnd) break;
										++argN;
										validate(word == Word::Varname);
										readWord(Word::CastOperator);
										std::string type = readWord(Word::Name);
										ByteCode arg;
										if (type == "number") {
											arg = declareVar(word, RAM_VAR_NUMERIC);
										} else if (type == "text") {
											arg = declareVar(word, RAM_VAR_TEXT);
										} else {
											if (Device::objectTypesByName.contains(type)) {
												arg = declareVar(word, CODE_TYPE(RAM_OBJECT | Device::objectTypesByName.at(type).id));
											} else {
												throw CompileError("Invalid argument type in function declaration");
											}
										}
										args.emplace_back(arg.rawValue);
										Word next = readWord();
										if (next == Word::CommaOperator) continue;
										else if (next == Word::ExpressionEnd) break;
										else validate(false);
									}
									validate(!readWord());
									pushStack("function");
								}
								// entry points
								else if (find(begin(Implementation::entryPoints), end(Implementation::entryPoints), firstWord.word) != end(Implementation::entryPoints)) {
									size_t entryPointIndex = entryPoints.size();
									auto& entryPoint = entryPoints.emplace_back();
									entryPoint.name = firstWord.word;
									openFunction("entrypoint." + std::to_string(entryPointIndex));
									Word dotOrBeginParen = readWord();
									if (dotOrBeginParen == Word::TrailOperator) {
										Word refNameOrLiteral = readWord();
										if (refNameOrLiteral == Word::Varname) {
											entryPoint.ref = getVar(refNameOrLiteral);
										} else if (refNameOrLiteral == Word::Numeric) {
											entryPoint.ref = declareVar("", ROM_CONST_NUMERIC, refNameOrLiteral);
										} else if (refNameOrLiteral == Word::Text) {
											entryPoint.ref = declareVar("", ROM_CONST_TEXT, refNameOrLiteral);
										} else {
											validate(false);
										}
										dotOrBeginParen = readWord();
									}
									if (dotOrBeginParen == Word::ExpressionBegin) {
										// Arguments
										int argN = 0;
										for (;;) {
											Word word = readWord();
											if (!word || word == Word::ExpressionEnd) break;
											++argN;
											validate(word == Word::Varname);
											readWord(Word::CastOperator);
											std::string type = readWord(Word::Name);
											ByteCode arg;
											if (type == "number") {
												arg = declareVar(word, RAM_VAR_NUMERIC);
											} else if (type == "text") {
												arg = declareVar(word, RAM_VAR_TEXT);
											} else {
												if (Device::objectTypesByName.contains(type)) {
													arg = declareVar(word, CODE_TYPE(RAM_OBJECT | Device::objectTypesByName.at(type).id));
												} else {
													throw CompileError("Invalid argument type in function declaration");
												}
											}
											entryPoint.args.emplace_back(arg.rawValue);
											Word next = readWord();
											if (next == Word::CommaOperator) continue;
											else if (next == Word::ExpressionEnd) break;
											else validate(false);
										}
									} else {
										validate(!dotOrBeginParen);
									}
									pushStack("function");
								}
								// ERROR
								else {
									throw CompileError("Invalid statement");
								}
							}break;
							default: throw CompileError("Invalid statement");
						}
					} else {
						// Function Scope
						if (currentLine) {
							write({LINENUMBER, currentLine});
						}
						if (line.scope > currentScope) {
							throw CompileError("Invalid scope");
						}
						while (line.scope < currentScope) {
							if (line.scope == currentScope - 1) {
								if (firstWord == "elseif" || firstWord == "else") {
									validate(stack.back().type == "if");
									userVars[currentFunctionName][currentStackId].clear(); // popStack+pushStack would cause our pointers to be lost
									break;
								}
							}
							popStack();
						}
						switch (firstWord.type) {
							case Word::Varname: {
								// Variable assignment
								ByteCode dst = getVar(firstWord);
								Word operation = readWord();
								switch (operation.type) {
									case Word::AssignmentOperatorGroup:{
										if (IsArray(dst)) {
											// For now, the only possible assignment for an array is = another array. This will resize the array and copy all elements.
											validate(operation == "=");
											ByteCode ref = getVar(readWord(Word::Varname));
											validate(IsArray(ref));
											write(SET);
											write(dst);
											write(ref);
											write(VOID);
										} else {
											validate(IsVar(dst));
											ByteCode op = GetOperator(operation);
											ByteCode ref = compileExpression(line.words, nextWordIndex, -1);
											write(op);
											write(dst);
											if (op != SET) write(dst);
											write(ref);
											write(VOID);
										}
									}break;
									case Word::TrailOperator:{
										Word operand = readWord();
										if (operand == Word::Varname || operand == Word::Numeric) {
											validate(IsArray(dst) || (IsText(dst) && IsVar(dst))); // make sure dst is not const
											operation = readWord();
											if (operation == Word::AssignmentOperatorGroup) {
												ByteCode op = GetOperator(operation);
												ByteCode ref = compileExpression(line.words, nextWordIndex, -1);
												if (op != SET) {
													ByteCode tmp;
													switch (dst.type) {
														case STORAGE_ARRAY_NUMERIC:
														case RAM_ARRAY_NUMERIC:
															tmp = declareTmpNumeric();
															break;
														case STORAGE_VAR_TEXT:
														case STORAGE_ARRAY_TEXT:
														case RAM_VAR_TEXT:
														case RAM_ARRAY_TEXT:
															tmp = declareTmpText();
															break;
														default: validate(false);
													}
													// Assign the current indexed value to tmp
													write(IDX);
													write(tmp);
													write(dst);
													if (operand == Word::Varname) {
														ByteCode idx = getVar(operand);
														if (IsNumeric(idx)) {
															write({ARRAY_INDEX, ARRAY_INDEX_NONE});
														} else {
															validate(IsText(idx));
															write(OBJ_KEY);
														}
														write(idx);
													} else /*numeric*/ {
														write({ARRAY_INDEX, uint32_t(std::round(double(operand)))});
													}
													write(VOID);
													// Set tmp to the new value
													write(op);
													write(tmp);
													write(tmp);
													write(ref);
													write(VOID);
													ref = tmp;
												}
												write(SET);
												if (operand == Word::Varname) {
													ByteCode idx = getVar(operand);
													if (IsNumeric(idx)) {
														write({ARRAY_INDEX, ARRAY_INDEX_NONE});
													} else {
														validate(IsText(idx));
														write(OBJ_KEY);
													}
													write(idx);
												} else /*numeric*/ {
													write({ARRAY_INDEX, uint32_t(std::round(double(operand)))});
												}
												write(dst);
												write(ref);
												write(VOID);
											} else if (operation == Word::SuffixOperatorGroup) {
												ByteCode op = GetOperator(operation);
												ByteCode tmp;
												switch (dst.type) {
													case STORAGE_ARRAY_NUMERIC:
													case RAM_ARRAY_NUMERIC:
														tmp = declareTmpNumeric();
														break;
													case STORAGE_VAR_TEXT:
													case STORAGE_ARRAY_TEXT:
													case RAM_VAR_TEXT:
													case RAM_ARRAY_TEXT:
														tmp = declareTmpText();
														break;
													default: validate(false);
												}
												// Assign the current indexed value to tmp
												write(IDX);
												write(tmp);
												write(dst);
												if (operand == Word::Varname) {
													ByteCode idx = getVar(operand);
													if (IsNumeric(idx)) {
														write({ARRAY_INDEX, ARRAY_INDEX_NONE});
													} else {
														validate(IsText(idx));
														write(OBJ_KEY);
													}
													write(idx);
												} else /*numeric*/ {
													write({ARRAY_INDEX, uint32_t(std::round(double(operand)))});
												}
												write(VOID);
												// Set tmp to the new value
												write(op);
												if (operation == "!!") write(tmp);
												write(tmp);
												write(VOID);
												// Assign dst to tmp
												write(SET);
												if (operand == Word::Varname) {
													ByteCode idx = getVar(operand);
													if (IsNumeric(idx)) {
														write({ARRAY_INDEX, ARRAY_INDEX_NONE});
													} else {
														validate(IsText(idx));
														write(OBJ_KEY);
													}
													write(idx);
												} else /*numeric*/ {
													write({ARRAY_INDEX, uint32_t(std::round(double(operand)))});
												}
												write(dst);
												write(tmp);
												write(VOID);
											} else {
												validate(false);
											}
										} else if (operand == Word::Funcname || operand == Word::Name) {
											// Trailing function call statement or StringObject member assignment
											Word next = readWord();
											validate(next);
											if (next == Word::ExpressionBegin) {
												// Trailing function call
												std::vector<ByteCode> args {};
												args.push_back(dst);
												while (nextWordIndex < (int)line.words.size()) {
													int argEnd = GetArgEnd(line.words, nextWordIndex);
													if (argEnd == -1) {
														break;
													}
													args.push_back(compileExpression(line.words, nextWordIndex, argEnd));
													nextWordIndex = argEnd+1;
													if (readWord() != Word::CommaOperator) {
														break;
													}
												}
												compileFunctionCall(operand, args, false, true);
											} else if (operand == Word::Name && next == Word::AssignmentOperatorGroup) {
												// StringObject member assignment
												validate(IsVar(dst) && IsText(dst));
												ByteCode op = GetOperator(next);
												ByteCode ref = compileExpression(line.words, nextWordIndex, -1);
												ByteCode key = declareVar("", ROM_CONST_TEXT, operand);
												if (op != SET) {
													ByteCode tmp = declareTmpText();
													// Assign the current indexed value to tmp
													write(IDX);
													write(tmp);
													write(dst);
													write(OBJ_KEY);
													write(key);
													write(VOID);
													// Set tmp to the new value
													write(op);
													write(tmp);
													write(tmp);
													write(ref);
													write(VOID);
													ref = tmp;
												}
												write(SET);
												write(OBJ_KEY);
												write(key);
												write(dst);
												write(ref);
												write(VOID);
											} else if (operand == Word::Name && next == Word::SuffixOperatorGroup) {
												// StringObject member with suffix operation
												validate(IsVar(dst) && IsText(dst));
												ByteCode op = GetOperator(next);
												ByteCode key = declareVar("", ROM_CONST_TEXT, operand);
												ByteCode tmp = declareTmpText();
												// Assign the current indexed value to tmp
												write(IDX);
												write(tmp);
												write(dst);
												write(OBJ_KEY);
												write(key);
												write(VOID);
												// Set tmp to the new value
												write(op);
												if (next == "!!") write(tmp);
												write(tmp);
												write(VOID);
												// Assign dst to tmp
												write(SET);
												write(OBJ_KEY);
												write(key);
												write(dst);
												write(tmp);
												write(VOID);
											} else validate(false);
										} else validate(false);
									}break;
									case Word::SuffixOperatorGroup:{
										validate(IsNumeric(dst));
										write(GetOperator(operation));
										if (operation == "!!") write(dst);
										write(dst);
										write(VOID);
									}break;
									default: throw CompileError("Invalid operator", operation);
								}
							}break;
							case Word::Funcname: {
								// Function call
								std::vector<ByteCode> args {};
								validate(readWord() == Word::ExpressionBegin);
								while (nextWordIndex < (int)line.words.size()) {
									int argEnd = GetArgEnd(line.words, nextWordIndex);
									if (argEnd == -1) {
										break;
									}
									args.push_back(compileExpression(line.words, nextWordIndex, argEnd));
									nextWordIndex = argEnd+1;
									if (readWord() != Word::CommaOperator) {
										break;
									}
								}
								compileFunctionCall(firstWord, args, false);
							}break;
							case Word::Name: {
								// var
								if (firstWord == "var") {
									std::string name = readWord(Word::Varname);
									Word op = readWord(Word::AssignmentOperatorGroup);
									validate(op == "=");
									ByteCode ref = compileExpression(line.words, nextWordIndex, -1);
									if (ref.type == VOID) {
										throw CompileError("Cannot assign a var to VOID");
									}
									ByteCode var = declareVar(name, GetRamVarType(ref.type));
									write(SET);
									write(var);
									write(ref);
									write(VOID);
								}
								// array
								else if (firstWord == "array") {
									// For now, the only possible assignment for an array is = another array. This will resize the array and copy all elements.
									std::string name = readWord(Word::Varname);
									Word op = readWord();
									if (op == "=") {
										ByteCode ref = getVar(readWord(Word::Varname));
										validate(nextWordIndex == (int)line.words.size());
										ByteCode var = declareVar(name, GetRamVarType(ref.type));
										validate(IsArray(ref));
										write(SET);
										write(var);
										write(ref);
										write(VOID);
									} else {
										validate(op == Word::CastOperator);
										std::string type = readWord(Word::Name);
										validate(nextWordIndex == (int)line.words.size());
										if (type == "number") {
											declareVar(name, RAM_ARRAY_NUMERIC);
										} else if (type == "text") {
											declareVar(name, RAM_ARRAY_TEXT);
										} else {
											validate(false);
										}
									}
								}
								// output
								else if (firstWord == "output") {
									std::vector<ByteCode> args {};
									validate(readWord() == Word::TrailOperator);
									Word idx = readWord();
									if (idx == Word::Numeric) {
										args.push_back(declareVar("", ROM_CONST_NUMERIC, idx));
									} else if (idx == Word::Varname) {
										args.push_back(getVar(idx));
									} else validate(false);
									validate(readWord() == Word::ExpressionBegin);
									while (nextWordIndex < (int)line.words.size()) {
										int argEnd = GetArgEnd(line.words, nextWordIndex);
										if (argEnd == -1) {
											break;
										}
										args.push_back(compileExpression(line.words, nextWordIndex, argEnd));
										nextWordIndex = argEnd+1;
										if (readWord() != Word::CommaOperator) {
											break;
										}
									}
									firstWord.word = "system.output";
									compileFunctionCall(firstWord, args, false);
								}
								// foreach
								else if (firstWord == "foreach") {
									pushStack("loop");
									ByteCode arr = getVar(readWord(Word::Varname));
									validate(IsArray(arr) || IsText(arr));
									readWord(Word::ExpressionBegin);
									Word index = readWord(Word::Varname);
									readWord(Word::CommaOperator);
									Word item = readWord(Word::Varname);
									validate(readWord() == Word::ExpressionEnd);
									validate(nextWordIndex == (int)line.words.size());
									ByteCode itemRef, indexRef;
									switch (arr.type) {
										case STORAGE_ARRAY_NUMERIC:
										case RAM_ARRAY_NUMERIC:
											indexRef = declareVar(index, RAM_VAR_NUMERIC);
											itemRef = declareVar(item, RAM_VAR_NUMERIC);
											break;
										case STORAGE_ARRAY_TEXT:
										case RAM_ARRAY_TEXT:
											indexRef = declareVar(index, RAM_VAR_NUMERIC);
											itemRef = declareVar(item, RAM_VAR_TEXT);
											break;
										case STORAGE_VAR_TEXT:
										case RAM_VAR_TEXT:
										case ROM_CONST_TEXT:
											indexRef = declareVar(index, RAM_VAR_TEXT);
											itemRef = declareVar(item, RAM_VAR_TEXT);
											break;
										default: validate(false);
									}
									
									if (indexRef.type == RAM_VAR_TEXT) {
										// StringObject
										
										// Set offset to 0
										ByteCode offset = declareTmpNumeric();
										write(SET);
										write(offset);
										write(VOID);
										
										addPointer("LoopBegin") = addr();
										
										// Get key and move offset to next key
										write(KEY);
										write(indexRef);
										write(arr);
										write(offset);
										write(VOID);
										
										// Check condition (key != "")
										//ByteCode condition = declareTmpNumeric();
										write(CND);
										addPointer("LoopContinue") = write(ADDR);
										addPointer("LoopBreak") = write(ADDR);
										write(indexRef);
										write(VOID);
										
										applyPointerAddr("LoopContinue");
										
										// Set current item
										write(IDX);
										write(itemRef);
										write(arr);
										write(OBJ_KEY);
										write(indexRef);
										write(VOID);
										
									} else {
									
										// Set index to -1
										write(SET);
										write(indexRef);
										write(VOID);
										write(DEC);
										write(indexRef);
										write(VOID);
										
										// Get Array Size
										ByteCode arrSize = declareTmpNumeric();
										write(SIZ);
										write(arrSize);
										write(arr);
										write(VOID);
										
										addPointer("LoopBegin") = addr();
										
										// Increment index
										write(INC);
										write(indexRef);
										write(VOID);
										
										// Assign condition
										ByteCode condition = declareTmpNumeric();
										write(LST);
										write(condition);
										write(indexRef);
										write(arrSize);
										write(VOID);
										
										// Check condition to continue or break
										write(CND);
										addPointer("LoopContinue") = write(ADDR);
										addPointer("LoopBreak") = write(ADDR);
										write(condition);
										write(VOID);
										
										applyPointerAddr("LoopContinue");
										
										// Set current item
										write(IDX);
										write(itemRef);
										write(arr);
										write({ARRAY_INDEX, ARRAY_INDEX_NONE});
										write(indexRef);
										write(VOID);
										
									}
								}
								// repeat
								else if (firstWord == "repeat") {
									pushStack("loop");
									Word n = readWord();
									ByteCode nRef;
									if (n == Word::Varname) {
										nRef = getVar(n);
									} else if (n == Word::Numeric) {
										nRef = declareVar("", ROM_CONST_NUMERIC, n);
									}
									validate(IsNumeric(nRef));
									readWord(Word::ExpressionBegin);
									Word index = readWord(Word::Varname);
									validate(readWord() == Word::ExpressionEnd);
									validate(nextWordIndex == (int)line.words.size());
									ByteCode indexRef = declareVar(index, RAM_VAR_NUMERIC);
									
									// Set index to -1
									write(SET);
									write(indexRef);
									write(VOID);
									write(DEC);
									write(indexRef);
									write(VOID);
									
									addPointer("LoopBegin") = addr();
									
									// Increment index
									write(INC);
									write(indexRef);
									write(VOID);
									
									// Assign condition
									ByteCode condition = declareTmpNumeric();
									write(LST);
									write(condition);
									write(indexRef);
									write(nRef);
									write(VOID);
									
									// Check condition to continue or break
									write(CND);
									addPointer("LoopContinue") = write(ADDR);
									addPointer("LoopBreak") = write(ADDR);
									write(condition);
									write(VOID);
									
									applyPointerAddr("LoopContinue");
								}
								// for
								else if (firstWord == "for") {
									pushStack("loop");
									
									Word start = readWord();
									readWord(Word::CommaOperator);
									Word end = readWord();
									
									ByteCode startRef, endRef;
									if (start == Word::Varname) {
										startRef = getVar(start);
									} else if (start == Word::Numeric) {
										startRef = declareVar("", ROM_CONST_NUMERIC, start);
									}
									if (end == Word::Varname) {
										endRef = getVar(end);
									} else if (end == Word::Numeric) {
										endRef = declareVar("", ROM_CONST_NUMERIC, end);
									}
									validate(IsNumeric(startRef) && IsNumeric(endRef));
									
									readWord(Word::ExpressionBegin);
									Word index = readWord(Word::Varname);
									validate(readWord() == Word::ExpressionEnd);
									validate(nextWordIndex == (int)line.words.size());
									ByteCode indexRef = declareVar(index, RAM_VAR_NUMERIC);
									
									ByteCode diffres = declareTmpNumeric();
									write(SUB);
									write(diffres);
									write(endRef);
									write(startRef);
									write(VOID);
									
									ByteCode step = declareTmpNumeric();
									write(SIG);
									write(step);
									write(diffres);
									write(declareVar("", ROM_CONST_NUMERIC, Word{1}));
									write(VOID);
									
									// Set index to round(start - step)
									write(SET);
									write(indexRef);
									write(startRef);
									write(VOID);
									write(SUB);
									write(indexRef);
									write(indexRef);
									write(step);
									write(VOID);
									write(RND);
									write(indexRef);
									write(indexRef);
									write(VOID);
									
									// Set lastRef to round(end + step)
									ByteCode lastRef = declareTmpNumeric();
									write(ADD);
									write(lastRef);
									write(endRef);
									write(step);
									write(VOID);
									write(RND);
									write(lastRef);
									write(lastRef);
									write(VOID);
									
									addPointer("LoopBegin") = addr();
									
									// Increment index
									write(ADD);
									write(indexRef);
									write(indexRef);
									write(step);
									write(VOID);
									
									// Assign condition
									ByteCode condition = declareTmpNumeric();
									write(NEQ);
									write(condition);
									write(indexRef);
									write(lastRef);
									write(VOID);
									
									// Check condition to continue or break
									write(CND);
									addPointer("LoopContinue") = write(ADDR);
									addPointer("LoopBreak") = write(ADDR);
									write(condition);
									write(VOID);
									
									applyPointerAddr("LoopContinue");
								}
								// while
								else if (firstWord == "while") {
									pushStack("loop");
									
									addPointer("LoopBegin") = addr();
									
									// Assign condition
									ByteCode condition = compileExpression(line.words, nextWordIndex, -1);
									
									// Check condition to continue or break
									write(CND);
									addPointer("LoopContinue") = write(ADDR);
									addPointer("LoopBreak") = write(ADDR);
									write(condition);
									write(VOID);
									
									applyPointerAddr("LoopContinue");
								}
								// break
								else if (firstWord == "break") {
									int s = stack.size();
									while (s-- > 0) {
										if (stack[s].type == "loop") {
											stack[s].pointers[std::to_string(stack[s].pointers.size()+1)] = gotoAddr(0);
											break;
										}
									}
								}
								// continue
								else if (firstWord == "continue") {
									int s = stack.size();
									while (s-- > 0) {
										if (stack[s].type == "loop") {
											gotoAddr(stack[s].pointers["LoopBegin"]);
											break;
										}
									}
								}
								// if
								else if (firstWord == "if") {
									pushStack("if");
									ByteCode ref = compileExpression(line.words, nextWordIndex, -1);
									write(CND);
									addPointer("gotoIfTrue") = write(ADDR);
									addPointer("gotoIfFalse") = write(ADDR);
									write(ref);
									write(VOID);
									applyPointerAddr("gotoIfTrue");
								}
								// elseif
								else if (firstWord == "elseif") {
									addPointer("") = gotoAddr(0); // endIf
									applyPointerAddr("gotoIfFalse");
									ByteCode ref = compileExpression(line.words, nextWordIndex, -1);
									write(CND);
									addPointer("gotoIfTrue") = write(ADDR);
									addPointer("gotoIfFalse") = write(ADDR);
									write(ref);
									write(VOID);
									applyPointerAddr("gotoIfTrue");
								}
								// else
								else if (firstWord == "else") {
									addPointer("") = gotoAddr(0); // endIf
									applyPointerAddr("gotoIfFalse");
								}
								// return
								else if (firstWord == "return") {
									if (line.words.size() > 1) {
										validate(currentFunctionName != "");
										ByteCode ret = getReturnVar(currentFunctionName);
										ByteCode val = compileExpression(line.words, nextWordIndex, -1);
										write(SET);
										write(ret);
										write(val);
										write(VOID);
									}
									write(RETURN);
								}
								else if (Device::deviceFunctionsByName.contains(firstWord.word) || firstWord == "recurse") {
									// Device Function call
									std::vector<ByteCode> args {};
									validate(readWord() == Word::ExpressionBegin);
									while (nextWordIndex < (int)line.words.size()) {
										int argEnd = GetArgEnd(line.words, nextWordIndex);
										if (argEnd == -1) {
											break;
										}
										args.push_back(compileExpression(line.words, nextWordIndex, argEnd));
										nextWordIndex = argEnd+1;
										if (readWord() != Word::CommaOperator) {
											break;
										}
									}
									if (firstWord == "recurse") {
										compileSelfCall(args, false);
									} else {
										compileFunctionCall(firstWord, args, false);
									}
								}
								// ERROR
								else {
									throw CompileError("Invalid statement");
								}
							}break;
							default: throw CompileError("Invalid statement");
						}
					}
				}
				while (currentScope > 0) {
					popStack();
				}
				closeCurrentFunction();
			} catch (CompileError& e) {
				std::stringstream err;
				err << e.what() << " in " << currentFile << ":" << currentLine;
				throw CompileError(err.str());
			}
			varsInitSize = rom_vars_init.size();
			programSize = rom_program.size();
			
			// Debug
			if (verbose) {
				bool nextLine = true;
				uint32_t address = 0;
				for (const ByteCode& code : rom_program) {
					if (nextLine) {
						std::cout << "\n" << getFunctionName({ADDR,address}) << std::endl;
						nextLine = false;
					}
					++address;
					switch (code.type) {
						case RETURN:
							std::cout << "RETURN"; if (code.value != 0) std::cout << "{" << code.value << "}";
							nextLine = true;
							break;
						case VOID:
							std::cout << "VOID"; if (code.value != 0) std::cout << "{" << code.value << "}";
							nextLine = true;
							break;
						case OP:{
							std::cout << "> ";
							char op[4];
							op[0] = (code.value & 0x0000ff);
							op[1] = (code.value & 0x00ff00) >> 8;
							op[2] = (code.value & 0xff0000) >> 16;
							op[3] = '\0';
							std::cout << op;
							std::cout << "  ";
							}break;
						case SOURCEFILE:{
							std::cout << "FILE: " << sourceFiles[code.value] << "\n";
						}break;
						case LINENUMBER:{
							std::cout << "LINE: " << code.value << "\n";
						}break;
						case ROM_CONST_NUMERIC:
							std::cout << "ROM_CONST_NUMERIC{";
							std::cout << "$" << getVarName(code);
							try {std::cout << "{" << GetConstValue(code) << "}";}catch(...){}
							std::cout << "} ";
							break;
						case ROM_CONST_TEXT:
							std::cout << "ROM_CONST_TEXT{";
							std::cout << "$" << getVarName(code);
							try {std::cout << "{" << GetConstValue(code) << "}";}catch(...){}
							std::cout << "} ";
							break;
						case STORAGE_VAR_NUMERIC:
							std::cout << "STORAGE_VAR_NUMERIC{";
							std::cout << "$" << getVarName(code);
							std::cout << "} ";
							break;
						case STORAGE_VAR_TEXT:
							std::cout << "STORAGE_VAR_TEXT{";
							std::cout << "$" << getVarName(code);
							std::cout << "} ";
							break;
						case STORAGE_ARRAY_NUMERIC:
							std::cout << "STORAGE_ARRAY_NUMERIC{";
							std::cout << "$" << getVarName(code);
							std::cout << "} ";
							break;
						case STORAGE_ARRAY_TEXT:
							std::cout << "STORAGE_ARRAY_TEXT{";
							std::cout << "$" << getVarName(code);
							std::cout << "} ";
							break;
						case RAM_VAR_NUMERIC:
							std::cout << "RAM_VAR_NUMERIC{";
							std::cout << "$" << getVarName(code);
							std::cout << "} ";
							break;
						case RAM_VAR_TEXT:
							std::cout << "RAM_VAR_TEXT{";
							std::cout << "$" << getVarName(code);
							std::cout << "} ";
							break;
						case RAM_ARRAY_NUMERIC:
							std::cout << "RAM_ARRAY_NUMERIC{";
							std::cout << "$" << getVarName(code);
							std::cout << "} ";
							break;
						case RAM_ARRAY_TEXT:
							std::cout << "RAM_ARRAY_TEXT{";
							std::cout << "$" << getVarName(code);
							std::cout << "} ";
							break;
						case ARRAY_INDEX:
							std::cout << "ARRAY_INDEX{" << code.value << "} ";
							break;
						case OBJ_KEY:
							std::cout << "OBJ_KEY ";
							break;
						case DEVICE_FUNCTION_INDEX:
							std::cout << "DEVICE_FUNCTION_INDEX{" << code.value << "} ";
							break;
						case INTEGER:
							std::cout << "INTEGER{" << code.value << "} ";
							break;
						case ADDR:
							std::cout << "ADDR{" << getFunctionName(code) << "} ";
							break;
						default:
							if (code.type >= RAM_OBJECT && Device::objectNamesById.contains(code.type & (RAM_OBJECT-1))) {
								std::cout << "RAM_OBJECT:" << Device::objectNamesById.at(code.type & (RAM_OBJECT-1)) << "{";
								std::cout << "$" << getVarName(code);
								std::cout << "} ";
							} else {
								std::cout << "UNKNOWN{" << code.value << "} ";
							}
					}
				}
				std::cout << std::endl;
			}
		}

		// From ByteCode stream
		explicit Assembly(std::istream& s) {
			assert(std::string(XC_APP_NAME) != "");
			assert(XC_APP_VERSION != 0);
			Read(s);
		}
		
		void Write(std::ostream& s) {
			{// Write Header
				// Write assembly file info
				s << parserFiletype << ' ' << parserVersionMajor << ' ' << parserVersionMinor << ' ' << XC_APP_NAME << ' ' << XC_APP_VERSION << '\n';
				
				// Write device compatibility info
				s << GetDeviceObjectsList() << '\n';
				for (auto&[name, o] : Device::objectTypesByName) {
					s << std::to_string(uint32_t(o.id)) << ' ' << GetDeviceFunctionsList(o.id) << '\n';
				}
				s << "0 " << GetDeviceFunctionsList(0) << '\n';
				
				// Write some sizes
				s << varsInitSize << ' ' << programSize << ' ' << storageRefs.size() << ' ' << functionRefs.size() << ' ' << timers.size() << ' ' << inputs.size() << ' ' << entryPoints.size() << ' ' << sourceFiles.size() << '\n';
				s << rom_numericConstants.size() << ' ';
				s << rom_textConstants.size() << '\n';
				s << ram_numericVariables << ' ';
				s << ram_textVariables << ' ';
				s << ram_objectReferences << ' ';
				s << ram_numericArrays << ' ';
				s << ram_textArrays << '\n';
				
				// Write sourceFile references for debug
				for (auto& f : sourceFiles) {
					s << f << '\n';
				}
				
				// Write memory references
				for (auto& name : storageRefs) {
					s << name << '\n';
				}
				
				// Write function references
				for (auto&[name,addr] : functionRefs) {
					s << name << ' ' << addr << '\n';
				}
				
				// Write timers
				for (auto&[interval, addr] : timers) {
					s << interval << ' ' << addr << '\n';
				}
				
				// Write inputs
				for (auto&[port, input] : inputs) {
					s << port << ' ' << input.addr << ' ' << input.args.size();
					for (auto& arg : input.args) s << ' ' << arg;
					s << '\n';
				}
				
				// Write entry points
				for (auto& entryPoint : entryPoints) {
					s << entryPoint.name << ' ' << entryPoint.addr << ' ' << entryPoint.ref.rawValue << ' ' << entryPoint.args.size();
					for (auto& arg : entryPoint.args) s << ' ' << arg;
					s << '\n';
				}
				
				// Write Rom data (constants)
				for (auto& value : rom_numericConstants) {
					s << value << ' ';
				}
				for (auto& value : rom_textConstants) {
					s << value << '\0';
				}
			}
			
			// Check ROM size
			if (varsInitSize + programSize > XC_MAX_ROM_SIZE) {
				throw CompileError("Maximum ROM size exceeded");
			}
			
			// Write vars_init bytecode
			assert(rom_vars_init.size() == varsInitSize);
			s.write((char*)rom_vars_init.data(), varsInitSize * sizeof(uint32_t));
			
			// Write program bytecode
			assert(rom_program.size() == programSize);
			s.write((char*)rom_program.data(), programSize * sizeof(uint32_t));
		}

	private:
		void Read(std::istream& s) {
			std::string filetype;
			std::string appName;
			uint32_t versionMajor;
			uint32_t versionMinor;
			uint32_t appVersion;
			size_t storageRefsSize;
			size_t functionRefsSize;
			size_t timersSize;
			size_t inputsSize;
			size_t entryPointsSize;
			size_t sourceFilesSize;
			size_t rom_numericConstantsSize;
			size_t rom_textConstantsSize;

			{// Read Header
				// Read assembly file info
				s >> filetype;
				if (filetype != parserFiletype) throw std::runtime_error("Bad XenonCode assembly");
				s >> versionMajor;
				if (versionMajor != parserVersionMajor) throw std::runtime_error("This XenonCode file version is incompatible with this interpreter");
				s >> versionMinor;
				if (versionMinor > parserVersionMinor) throw std::runtime_error("This XenonCode file version is more recent than this interpreter");
				s >> appName;
				if (appName != XC_APP_NAME) throw std::runtime_error("This XenonCode assembly is incompatible with this application");
				s >> appVersion;
				if (appVersion > XC_APP_VERSION) throw std::runtime_error("This XenonCode file version is more recent than this application");
				
				// Discard the \n
				s.get();
				
				// Read device compatibility info
				std::string deviceObjectsList;
				std::getline(s, deviceObjectsList, '\n');
				if (!deviceObjectsList.starts_with("OBJ") || !GetDeviceObjectsList().starts_with(deviceObjectsList)) throw std::runtime_error("This XenonCode assembly is incompatible with this device (objects do not match)");
				uint32_t objId;
				do {
					s >> objId;
					s.get(); // ignore space
					assert(objId < 128);
					std::string deviceFunctionsList;
					std::getline(s, deviceFunctionsList, '\n');
					if (!deviceFunctionsList.starts_with("FN") || !GetDeviceFunctionsList((uint8_t)objId).starts_with(deviceFunctionsList)) throw std::runtime_error("This XenonCode assembly is incompatible with this device (functions do not match)");
				} while (objId != 0);
				
				// Read some sizes
				s >> varsInitSize >> programSize >> storageRefsSize >> functionRefsSize >> timersSize >> inputsSize >> entryPointsSize >> sourceFilesSize;
				s >> rom_numericConstantsSize;
				s >> rom_textConstantsSize;
				s >> ram_numericVariables;
				s >> ram_textVariables;
				s >> ram_objectReferences;
				s >> ram_numericArrays;
				s >> ram_textArrays;

				// Check ROM size
				if (varsInitSize + programSize > XC_MAX_ROM_SIZE) {
					throw CompileError("Maximum ROM size exceeded");
				}
				
				// Prepare data
				rom_vars_init.resize(varsInitSize);
				rom_program.resize(programSize);
				rom_numericConstants.reserve(rom_numericConstantsSize);
				rom_textConstants.reserve(rom_textConstantsSize);
				storageRefs.reserve(storageRefsSize);
				functionRefs.reserve(functionRefsSize);
				entryPoints.reserve(entryPointsSize);
				sourceFiles.reserve(sourceFilesSize);

				// Discard the \n
				s.get();
				
				// Read sourceFile references for debug
				for (size_t i = 0; i < sourceFilesSize; ++i) {
					std::string f;
					std::getline(s, f, '\n');
					sourceFiles.push_back(f);
				}

				// Read memory references
				for (size_t i = 0; i < storageRefsSize; ++i) {
					std::string name;
					s >> name;
					storageRefs.push_back(name);
				}

				// Read function references
				for (size_t i = 0; i < functionRefsSize; ++i) {
					std::string name;
					uint32_t address;
					s >> name >> address;
					functionRefs.emplace(name, address);
				}
				
				// Read timers
				for (size_t i = 0; i < timersSize; ++i) {
					double interval;
					uint32_t address;
					s >> interval >> address;
					timers.emplace_back(interval, address);
				}
				
				// Read inputs
				for (size_t i = 0; i < inputsSize; ++i) {
					uint32_t port;
					uint32_t address;
					size_t argsSize;
					s >> port >> address >> argsSize;
					inputs[port].addr = address;
					inputs[port].args.resize(argsSize);
					for (size_t j = 0; j < argsSize; ++j) {
						s >> inputs[port].args[j];
					}
				}
				
				// Read entry points
				for (size_t i = 0; i < entryPointsSize; ++i) {
					std::string name;
					uint32_t address;
					ByteCode ref;
					size_t argsSize;
					s >> name >> address >> ref.rawValue >> argsSize;
					auto& entryPoint = entryPoints.emplace_back();
					entryPoint.name = name;
					entryPoint.addr = address;
					entryPoint.ref = ref;
					entryPoint.args.resize(argsSize);
					for (size_t j = 0; j < argsSize; ++j) {
						s >> entryPoint.args[j];
					}
				}

				// Discard the \n
				s.get();
				
				// Read Rom data (constants)
				for (size_t i = 0; i < rom_numericConstantsSize; ++i) {
					double value;
					s >> value;
					s.get();
					rom_numericConstants.push_back(value);
				}
				for (size_t i = 0; i < rom_textConstantsSize; ++i) {
					char value[XC_MAX_TEXT_LENGTH+1];
					s.getline(value, XC_MAX_TEXT_LENGTH, '\0');
					rom_textConstants.push_back(value);
				}
			}

			// Read vars_init bytecode
			s.read((char*)rom_vars_init.data(), varsInitSize * sizeof(uint32_t));

			// Read program bytecode
			s.read((char*)rom_program.data(), programSize * sizeof(uint32_t));
		}
	};

#pragma endregion

#pragma region Interpreter

	struct LocalVars {
		std::vector<double> numeric;
		std::vector<std::string> text;
		std::vector<std::vector<double>> numeric_arrays;
		std::vector<std::vector<std::string>> text_arrays;
		std::vector<uint64_t> objects;
	};

	inline static double GetCurrentTimestamp() {
		std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<double, std::milli>> time = std::chrono::high_resolution_clock::now();
		return time.time_since_epoch().count() * 0.001;
	}

	class Computer {
		// Data
		Assembly* assembly = nullptr;
		std::vector<double> ram_numeric {};
		std::vector<std::string> ram_text {};
		std::vector<std::vector<double>> ram_numeric_arrays {};
		std::vector<std::vector<std::string>> ram_text_arrays {};
		std::vector<uint64_t> ram_objects {};
		uint32_t recursion_depth = 0;

		LocalVars recursive_localvars {};
		std::unordered_map<uint32_t, uint32_t> currentLineByAddr {};
		std::unordered_map<uint32_t, std::string> currentFileByAddr;
		
		std::vector<double> timersLastRun {};
		
	public:
		struct Capability {
			uint32_t ipc = 0; // instructions per cycle
			uint32_t ram = 16'000'000; // number of ram variables * their memory penalty
			uint32_t storage = 16; // number of storage variables
			uint32_t io = 256; // Number of input/output ports
		} capability;
		
		enum class CycleState : uint8_t {
			NONE = 0,
			BOOT,
			VARS_INIT,
			SYSTEM_INIT,
			RUN,
		} cycleState = CycleState::NONE;
		
		uint64_t currentCycleInstructions = 0;
		std::unordered_map<std::string, std::vector<std::string>> storageCache {};
		bool storageDirty = false;
		
		bool IsLoaded() const {return assembly;}
		
		Computer() {}
		virtual ~Computer() {
			ClearAssemly();
		}
		
		// Compile to bytecode and write to output stream
		static bool CompileAssembly(std::ostream& stream, const std::vector<ParsedLine>& lines, bool verbose = false) {
			Assembly assembly(lines, verbose);
			assembly.Write(stream);
			return true;
		}

		// Compile to bytecode and save assembly
		static bool CompileAssembly(const std::string& directory, const std::vector<ParsedLine>& lines, bool verbose = false) {
			std::ofstream file{directory + "/" + XC_PROGRAM_EXECUTABLE, std::ios::out | std::ios::trunc | std::ios::binary};
			CompileAssembly(file, lines, verbose);
			return file.good();
		}
		
		// From a saved state
		virtual std::vector<uint8_t> SaveState() {
			if (!assembly) {
				return {};
			}
			
			std::vector<uint8_t> state;
			size_t pos = 0;
			uint32_t memsize;
			
			{// Assembly
				std::stringstream ss(std::ios::out | std::ios::in | std::ios::binary);
				assembly->Write(ss);
				memsize = ss.tellp();
				ss.seekg(0, std::ios::beg);
				state.reserve(
					+ sizeof(memsize) + memsize
					+ sizeof(memsize) + ram_numeric.size() * sizeof(double)
					+ sizeof(memsize) + ram_text.size() * 20 // approximate average text length
					+ sizeof(memsize) + ram_numeric_arrays.size() * sizeof(double) * 10 // approximate average numeric array length
					+ sizeof(memsize) + ram_text_arrays.size() * 20 * 10 // approximate average text array length
					+ sizeof(memsize) + ram_objects.size() * sizeof(uint64_t)
					+ sizeof(memsize) // checksum
				);
				state.resize(pos + sizeof(memsize) + memsize);
				memcpy(state.data() + pos, &memsize, sizeof(memsize));
				pos += sizeof(memsize);
				ss.read((char*)state.data() + pos, memsize);
				assert(ss.tellg() == memsize);
				pos += memsize;
			}
			
			{// ram_numeric
				state.resize(pos + sizeof(memsize) + ram_numeric.size() * sizeof(double));
				memsize = ram_numeric.size();
				memcpy(state.data() + pos, &memsize, sizeof(memsize));
				pos += sizeof(memsize);
				memcpy(state.data() + pos, ram_numeric.data(), ram_numeric.size() * sizeof(double));
				pos += ram_numeric.size() * sizeof(double);
			}
			
			{// ram_text
				state.resize(pos + sizeof(memsize));
				memsize = ram_text.size();
				memcpy(state.data() + pos, &memsize, sizeof(memsize));
				pos += sizeof(memsize);
				for (const std::string& text : ram_text) {
					state.resize(pos + text.size() + 1);
					memcpy(state.data() + pos, text.data(), text.size() + 1);
					pos += text.size() + 1;
				}
			}
			
			{// ram_numeric_arrays
				state.resize(pos + sizeof(memsize));
				memsize = ram_numeric_arrays.size();
				memcpy(state.data() + pos, &memsize, sizeof(memsize));
				pos += sizeof(memsize);
				for (const std::vector<double>& arr : ram_numeric_arrays) {
					state.resize(pos + sizeof(memsize) + arr.size() * sizeof(double));
					memsize = arr.size();
					memcpy(state.data() + pos, &memsize, sizeof(memsize));
					pos += sizeof(memsize);
					memcpy(state.data() + pos, arr.data(), arr.size() * sizeof(double));
					pos += arr.size() * sizeof(double);
				}
			}
			
			{// ram_text_arrays
				state.resize(pos + sizeof(memsize));
				memsize = ram_text_arrays.size();
				memcpy(state.data() + pos, &memsize, sizeof(memsize));
				pos += sizeof(memsize);
				for (const std::vector<std::string>& arr : ram_text_arrays) {
					state.resize(pos + sizeof(memsize));
					memsize = arr.size();
					memcpy(state.data() + pos, &memsize, sizeof(memsize));
					pos += sizeof(memsize);
					for (const std::string& text : arr) {
						state.resize(pos + text.size() + 1);
						memcpy(state.data() + pos, text.data(), text.size());
						pos += text.size();
						state[pos] = '\0';
						++pos;
					}
				}
			}
			
			{// ram_objects
				state.resize(pos + sizeof(memsize) + ram_objects.size() * sizeof(uint64_t));
				memsize = ram_objects.size();
				memcpy(state.data() + pos, &memsize, sizeof(memsize));
				pos += sizeof(memsize);
				memcpy(state.data() + pos, ram_objects.data(), ram_objects.size() * sizeof(uint64_t));
				pos += ram_objects.size() * sizeof(uint64_t);
			}
			
			{// Checksum (just the total size, don't need to confirm data integrity)
				size_t totalSize = state.size() + sizeof(totalSize);
				state.resize(pos + sizeof(totalSize));
				memcpy(state.data() + pos, &totalSize, sizeof(totalSize));
				pos += sizeof(totalSize);
			}
			
			return state;
		}
		
		// From a saved state
		virtual bool LoadState(const std::vector<uint8_t>& state) {
			size_t pos = 0;
			uint32_t memsize;
			
			{// Assembly
				std::stringstream ss(std::ios::out | std::ios::in | std::ios::binary);
				memcpy(&memsize, state.data() + pos, sizeof(memsize));
				pos += sizeof(memsize);
				ss.write((char*)state.data() + pos, memsize);
				assert(ss.tellp() == memsize);
				ss.seekg(0, std::ios::beg);
				pos += memsize;
				ClearAssemly();
				assembly = new Assembly(ss);
				if (!Bootup()) return false;
			}
			
			{// ram_numeric
				memcpy(&memsize, state.data() + pos, sizeof(memsize));
				pos += sizeof(memsize);
				ram_numeric.resize(memsize);
				memcpy(ram_numeric.data(), state.data() + pos, ram_numeric.size() * sizeof(double));
				pos += ram_numeric.size() * sizeof(double);
			}
			
			{// ram_text
				memcpy(&memsize, state.data() + pos, sizeof(memsize));
				pos += sizeof(memsize);
				ram_text.resize(memsize);
				for (std::string& text : ram_text) {
					text.resize(0);
					while (state[pos] != '\0') {
						text += state[pos];
						++pos;
					}
					++pos;
				}
			}
			
			{// ram_numeric_arrays
				memcpy(&memsize, state.data() + pos, sizeof(memsize));
				pos += sizeof(memsize);
				ram_numeric_arrays.resize(memsize);
				for (std::vector<double>& arr : ram_numeric_arrays) {
					memcpy(&memsize, state.data() + pos, sizeof(memsize));
					pos += sizeof(memsize);
					arr.resize(memsize);
					memcpy(arr.data(), state.data() + pos, arr.size() * sizeof(double));
					pos += arr.size() * sizeof(double);
				}
			}
			
			{// ram_text_arrays
				memcpy(&memsize, state.data() + pos, sizeof(memsize));
				pos += sizeof(memsize);
				ram_text_arrays.resize(memsize);
				for (std::vector<std::string>& arr : ram_text_arrays) {
					memcpy(&memsize, state.data() + pos, sizeof(memsize));
					pos += sizeof(memsize);
					arr.resize(memsize);
					for (std::string& text : arr) {
						text.resize(0);
						while (state[pos] != '\0') {
							text += state[pos];
							++pos;
						}
						++pos;
					}
				}
			}
			
			{// ram_objects
				memcpy(&memsize, state.data() + pos, sizeof(memsize));
				pos += sizeof(memsize);
				ram_objects.resize(memsize);
				memcpy(ram_objects.data(), state.data() + pos, ram_objects.size() * sizeof(uint64_t));
				pos += ram_objects.size() * sizeof(uint64_t);
			}
			
			{// Checksum (just the total size, don't need to confirm data integrity)
				size_t totalSize;
				memcpy(&totalSize, state.data() + pos, sizeof(totalSize));
				pos += sizeof(totalSize);
				if (pos != totalSize) {
					throw RuntimeError("Invalid state");
				}
			}
			
			recursion_depth = 0;
			
			// Ready
			return true;
		}

		// From an input stream
		virtual bool LoadProgram(std::istream& stream) {
			ClearAssemly();
			assembly = new Assembly(stream);
			return Bootup();
		}
		
		// From a compiled assembly
		virtual bool LoadProgram(const std::string& directory) {
			std::ifstream file{directory + "/" + XC_PROGRAM_EXECUTABLE, std::ios::binary};
			return LoadProgram(file);		
		}
		
		// From a source code
		virtual bool LoadProgram(const std::vector<ParsedLine>& lines, bool verbose = false) {
			
			ClearAssemly();
			assembly = new Assembly(lines, verbose);
			
			return Bootup();
		}

		virtual uint32_t RamLen() {
			return assembly->ram_numericVariables
								+ assembly->ram_textVariables * XC_TEXT_MEMORY_PENALTY
								+ assembly->ram_numericArrays * XC_ARRAY_NUMERIC_MEMORY_PENALTY
								+ assembly->ram_textArrays * XC_ARRAY_TEXT_MEMORY_PENALTY
								+ assembly->ram_objectReferences * XC_OBJECT_MEMORY_PENALTY;
		}

		virtual uint32_t RecursiveLocalVarsLen() {
			return (recursive_localvars.numeric.size()
							+ recursive_localvars.text.size() * XC_TEXT_MEMORY_PENALTY
							+ recursive_localvars.numeric_arrays.size() * XC_ARRAY_NUMERIC_MEMORY_PENALTY
							+ recursive_localvars.text_arrays.size() * XC_ARRAY_TEXT_MEMORY_PENALTY
							+ recursive_localvars.objects.size() * XC_OBJECT_MEMORY_PENALTY
						 ) * XC_RECURSIVE_MEMORY_PENALTY; 
		}
		
		virtual bool Bootup() {
			cycleState = CycleState::BOOT;
			{// Check Capabilities
				// Do we have enough RAM to run this program?
				if (capability.ram < RamLen()) {
					// Not enough RAM
					return false;
				}
				// Do we have enough storage space to run this program?
				if (capability.storage < assembly->storageRefs.size()) {
					// Not enough Storage
					return false;
				}
			}
			
			// Prepare RAM
			ram_numeric.clear();
			ram_numeric.resize(assembly->ram_numericVariables, 0.0);
			ram_text.clear();
			ram_text.resize(assembly->ram_textVariables, "");
			ram_numeric_arrays.clear();
			ram_numeric_arrays.resize(assembly->ram_numericArrays, {});
			ram_text_arrays.clear();
			ram_text_arrays.resize(assembly->ram_textArrays, {});
			ram_objects.clear();
			ram_objects.resize(assembly->ram_objectReferences, 0);
			
			// Prepare timers
			timersLastRun.clear();
			timersLastRun.resize(assembly->timers.size());
			
			recursion_depth = 0;

			currentFileByAddr.clear();
			currentLineByAddr.clear();
			
			return true;
		}
		
		void ClearAssemly() {
			if (assembly) {
				delete assembly;
				assembly = nullptr;
			}
			cycleState = CycleState::NONE;
		}
		
		virtual void Shutdown() {
			ClearAssemly();
		}
		
		virtual void LoadStorage(const std::string& storageDir) {
			storageCache.clear();
			for (const auto& name : assembly->storageRefs) {
				auto& storage = storageCache[name];
				storage.clear();
				std::ifstream file{storageDir + "/" + name};
				char value[XC_MAX_TEXT_LENGTH+1];
				while (file.getline(value, XC_MAX_TEXT_LENGTH)) {
					storage.emplace_back(std::string(value));
				}
			}
			storageDirty = false;
		}
		
		virtual void SaveStorage(const std::string& storageDir) {
			if (storageDirty) {
				std::filesystem::create_directories(storageDir);
				size_t storageSize = 0;
				for (const auto& name : assembly->storageRefs) {
					const auto& storage = storageCache[name];
					std::ofstream file{storageDir + "/" + name};
					for (const std::string& value : storage) {
						if ((storageSize += value.size()) > XC_MAX_STORAGE_MEMORY_SIZE) {
							throw RuntimeError("Max storage memory exceeded");
						}
						file << value << '\n';
					}
				}
				storageDirty = false;
			}
		}
		
		virtual void ClearStorage(const std::string& storageDir = "#") {
			if (storageDir != "#") {
				std::filesystem::remove_all(storageDir);
			}
			storageCache.clear();
			if (assembly) {
				for (const auto& name : assembly->storageRefs) {
					storageCache[name].clear();
				}
			}
			storageDirty = false;
		}
		
	private:
		std::vector<std::string>& GetStorage(ByteCode arr) {
			if ((arr.type != STORAGE_ARRAY_NUMERIC && arr.type != STORAGE_ARRAY_TEXT && arr.type != STORAGE_VAR_NUMERIC && arr.type != STORAGE_VAR_TEXT) || arr.value >= assembly->storageRefs.size()) {
				throw RuntimeError("Invalid storage reference");
			}
			std::string name = assembly->storageRefs[arr.value];
			if (!storageCache.contains(name)) {
				throw RuntimeError("Invalid storage reference");
			}
			if ((arr.type == STORAGE_VAR_NUMERIC || arr.type == STORAGE_VAR_TEXT) && storageCache.at(name).size() == 0) {
				storageCache.at(name).emplace_back();
			}
			return storageCache.at(name);
		}
		
		std::vector<double>& GetNumericArray(ByteCode arr) {
			if (arr.type != RAM_ARRAY_NUMERIC || arr.value >= ram_numeric_arrays.size()) throw RuntimeError("Invalid array reference");
			return ram_numeric_arrays[arr.value];
		}
		
		std::vector<std::string>& GetTextArray(ByteCode arr) {
			if (arr.type != RAM_ARRAY_TEXT || arr.value >= ram_text_arrays.size()) throw RuntimeError("Invalid array reference");
			return ram_text_arrays[arr.value];
		}
		
		void StorageSet(double value, ByteCode arr, uint32_t arrIndex = ARRAY_INDEX_NONE) {
			auto& storage = GetStorage(arr);
			if (arrIndex == ARRAY_INDEX_NONE) {
				storage[0] = ToString(value);
			} else{
				if (arrIndex >= storage.size()) {
					throw RuntimeError("Invalid array indexing");
				}
				storage[arrIndex] = ToString(value);
			}
			storageDirty = true;
		}
		void StorageSet(const std::string& value, ByteCode arr, uint32_t arrIndex = ARRAY_INDEX_NONE) {
			auto& storage = GetStorage(arr);
			if (arrIndex == ARRAY_INDEX_NONE) {
				storage[0] = value;
			} else{
				if (arrIndex >= storage.size()) {
					throw RuntimeError("Invalid array indexing");
				}
				storage[arrIndex] = value;
			}
			storageDirty = true;
		}
		
		double StorageGetNumeric(ByteCode arr, uint32_t arrIndex = ARRAY_INDEX_NONE) {
			auto& storage = GetStorage(arr);
			if (arrIndex == ARRAY_INDEX_NONE) {
				if (storage.empty()) {
					throw RuntimeError("Invalid array indexing");
				}
				return storage[0]==""? 0.0 : stod(storage[0]);
			} else{
				if (arrIndex >= storage.size()) {
					throw RuntimeError("Invalid array indexing");
				}
				return storage[arrIndex]==""? 0.0 : stod(storage[arrIndex]);
			}
		}
		const std::string& StorageGetText(ByteCode arr, uint32_t arrIndex = ARRAY_INDEX_NONE) {
			auto& storage = GetStorage(arr);
			if (arrIndex == ARRAY_INDEX_NONE) {
				if (storage.empty()) {
					throw RuntimeError("Invalid array indexing");
				}
				return storage[0];
			} else{
				if (arrIndex >= storage.size()) {
					throw RuntimeError("Invalid array indexing");
				}
				return storage[arrIndex];
			}
		}
		
		void MemSet(double value, ByteCode dst, uint32_t arrIndex = ARRAY_INDEX_NONE) {
			switch (dst.type) {
				case STORAGE_VAR_NUMERIC:
				case STORAGE_ARRAY_NUMERIC: {
					StorageSet(value, dst, arrIndex);
				}break;
				case RAM_VAR_NUMERIC: {
					if (dst.value >= ram_numeric.size()) throw RuntimeError("Invalid memory reference");
					ram_numeric[dst.value] = value;
				}break;
				case RAM_ARRAY_NUMERIC: {
					auto& arr = GetNumericArray(dst);
					if (arrIndex == ARRAY_INDEX_NONE || arrIndex >= arr.size()) throw RuntimeError("Invalid array indexing");
					arr[arrIndex] = value;
				}break;
				case STORAGE_ARRAY_TEXT:
				case RAM_ARRAY_TEXT:
				case ROM_CONST_TEXT:
				case STORAGE_VAR_TEXT:
				case RAM_VAR_TEXT: {
					MemSet(ToString(value), dst, arrIndex);
				}break;
				default: throw RuntimeError("Invalid memory reference");
			}
		}
		void MemSet(const std::string& value, ByteCode dst, uint32_t arrIndex = ARRAY_INDEX_NONE) {
			if (value.length() > XC_MAX_TEXT_LENGTH) {
				throw RuntimeError("Text too large");
			}
			switch (dst.type) {
				case STORAGE_VAR_TEXT: {
					auto& storage = GetStorage(dst);
					if (arrIndex == ARRAY_INDEX_NONE) {
						storage[0] = value;
						storageDirty = true;
					} else if (utf8length(value) == 1) {
						if (arrIndex >= utf8length(storage[0])) {
							throw RuntimeError("Invalid text indexing");
						}
						utf8assign(storage[0], arrIndex, value);
						storageDirty = true;
					} else {
						throw RuntimeError("Invalid char assignment");
					}
				}break;
				case STORAGE_ARRAY_TEXT: {
					StorageSet(value, dst, arrIndex);
				}break;
				case RAM_VAR_TEXT: {
					if (dst.value >= ram_text.size()) {
						throw RuntimeError("Invalid memory reference");
					}
					if (arrIndex == ARRAY_INDEX_NONE) {
						ram_text[dst.value] = value;
					} else if (utf8length(value) == 1) {
						if (arrIndex >= utf8length(ram_text[dst.value])) {
							throw RuntimeError("Invalid text indexing");
						}
						utf8assign(ram_text[dst.value], arrIndex, value);
					} else {
						throw RuntimeError("Invalid char assignment");
					}
				}break;
				case RAM_ARRAY_TEXT: {
					auto& arr = GetTextArray(dst);
					if (arrIndex == ARRAY_INDEX_NONE || arrIndex >= arr.size()) {
						throw RuntimeError("Invalid array indexing");
					}
					arr[arrIndex] = value;
				}break;
				case STORAGE_ARRAY_NUMERIC:
				case RAM_ARRAY_NUMERIC:
				case ROM_CONST_NUMERIC:
				case STORAGE_VAR_NUMERIC:
				case RAM_VAR_NUMERIC: {
					if (value.length() == 0) MemSet(0.0, dst, arrIndex);
					else MemSet(atof(value.c_str()), dst, arrIndex);
				}break;
				default: throw RuntimeError("Invalid memory reference");
			}
		}
		void MemSetObject(uint64_t value, ByteCode dst) {
			if (dst.type >= RAM_OBJECT) {
				if (dst.value >= ram_objects.size()) throw RuntimeError("Invalid memory reference");
				ram_objects[dst.value] = value;
			} else {
				throw RuntimeError("Invalid memory reference");
			}
		}
		
		double MemGetNumeric(ByteCode ref, uint32_t arrIndex = ARRAY_INDEX_NONE) {
			switch (ref.type) {
				case ROM_CONST_NUMERIC: {
					if (ref.value >= assembly->rom_numericConstants.size()) {
						throw RuntimeError("Invalid memory reference");
					}
					return assembly->rom_numericConstants[ref.value];
				}break;
				case STORAGE_VAR_NUMERIC:
				case STORAGE_ARRAY_NUMERIC: {
					return StorageGetNumeric(ref, arrIndex);
				}break;
				case RAM_VAR_NUMERIC: {
					if (ref.value >= ram_numeric.size()) {
						throw RuntimeError("Invalid memory reference");
					}
					return ram_numeric[ref.value];
				}break;
				case RAM_ARRAY_NUMERIC: {
					auto& arr = GetNumericArray(ref);
					if (arrIndex == ARRAY_INDEX_NONE || arrIndex >= arr.size()) {
						throw RuntimeError("Invalid array indexing");
					}
					return arr[arrIndex];
				}break;
				case VOID: return 0.0;
				case STORAGE_ARRAY_TEXT:
				case RAM_ARRAY_TEXT:
				case ROM_CONST_TEXT:
				case STORAGE_VAR_TEXT:
				case RAM_VAR_TEXT: {
					std::string text = MemGetText(ref, arrIndex);
					if (text.length() == 0) return 0.0;
					return atof(text.c_str());
				}break;
				default: throw RuntimeError("Invalid memory reference");
			}
		}
		bool MemGetBoolean(ByteCode ref, uint32_t arrIndex = ARRAY_INDEX_NONE) {
			if (IsText(ref)) {
				std::string txt = MemGetText(ref, arrIndex);
				return txt != "" && txt != "0";
			}
			return std::abs(MemGetNumeric(ref, arrIndex)) > EPSILON_DOUBLE;
		}
		const std::string& MemGetText(ByteCode ref) {
			switch (ref.type) {
				case ROM_CONST_TEXT: {
					if (ref.value >= assembly->rom_textConstants.size()) {
						throw RuntimeError("Invalid memory reference");
					}
					return assembly->rom_textConstants[ref.value];
				}break;
				case STORAGE_VAR_TEXT: {
					return StorageGetText(ref);
				}break;
				case RAM_VAR_TEXT: {
					if (ref.value >= ram_text.size()) {
						throw RuntimeError("Invalid memory reference");
					}
					return ram_text[ref.value];
				}break;
				case VOID: {
					static const std::string empty = "";
					return empty;
				}
				default: throw RuntimeError("Invalid memory reference");
			}
		}
		std::string MemGetText(ByteCode ref, uint32_t arrIndex) {
			switch (ref.type) {
				case ROM_CONST_TEXT: {
					if (ref.value >= assembly->rom_textConstants.size()) {
						throw RuntimeError("Invalid memory reference");
					}
					if (arrIndex == ARRAY_INDEX_NONE) {
						return assembly->rom_textConstants[ref.value];
					} else {
						const std::string& text = assembly->rom_textConstants[ref.value];
						if (arrIndex >= utf8length(text)) {
							throw RuntimeError("Invalid text indexing");
						}
						return utf8substr(text, arrIndex, 1);
					}
				}break;
				case STORAGE_ARRAY_TEXT: {
					return StorageGetText(ref, arrIndex);
				}break;
				case STORAGE_VAR_TEXT: {
					if (arrIndex == ARRAY_INDEX_NONE) {
						return StorageGetText(ref);
					} else {
						const std::string& text = StorageGetText(ref);
						if (arrIndex >= utf8length(text)) {
							throw RuntimeError("Invalid text indexing");
						}
						return utf8substr(text, arrIndex, 1);
					}
				}break;
				case RAM_VAR_TEXT: {
					if (ref.value >= ram_text.size()) {
						throw RuntimeError("Invalid memory reference");
					}
					if (arrIndex == ARRAY_INDEX_NONE) {
						return ram_text[ref.value];
					} else {
						if (arrIndex >= utf8length(ram_text[ref.value])) {
							throw RuntimeError("Invalid text indexing");
						}
						return utf8substr(ram_text[ref.value], arrIndex, 1);
					}
				}break;
				case RAM_ARRAY_TEXT: {
					auto& arr = GetTextArray(ref);
					if (arrIndex == ARRAY_INDEX_NONE || arrIndex >= arr.size()) {
						throw RuntimeError("Invalid array indexing");
					}
					return arr[arrIndex];
				}break;
				case VOID: {
					return "";
				}
				case STORAGE_ARRAY_NUMERIC:
				case RAM_ARRAY_NUMERIC:
				case ROM_CONST_NUMERIC:
				case STORAGE_VAR_NUMERIC:
				case RAM_VAR_NUMERIC: {
					return ToString(MemGetNumeric(ref, arrIndex));
				}break;
				default: throw RuntimeError("Invalid memory reference");
			}
		}
		
		Var MemGetObject(ByteCode ref) {
			if (ref.value >= ram_objects.size()) {
				throw RuntimeError("Invalid memory reference");
			}
			return {Var::Type(Var::Object | (ref & (RAM_OBJECT-1))), ram_objects[ref.value]};
		}
		
		template<typename CONTAINER, typename T>
		void ArrayInsertAuto(CONTAINER& container, const T& value) {
			if constexpr (std::is_same_v<CONTAINER, T>) {
				container.insert(container.end(), value.begin(), value.end());
			}
			else if constexpr (std::is_same_v<typename CONTAINER::value_type, T>) {
				container.push_back(value);
			}
			else if constexpr (std::is_same_v<typename CONTAINER::value_type, double> && std::is_same_v<T, std::string>) {
				if (value == "") {
					container.push_back(0.0);
				} else {
					container.push_back(stod(value));
				}
			}
			else if constexpr (std::is_same_v<typename CONTAINER::value_type, std::string> && std::is_same_v<T, double>) {
				container.push_back(ToString(value));
			}
			else if constexpr (std::is_same_v<typename CONTAINER::value_type, double> && std::is_same_v<typename T::value_type, std::string>) {
				for (const auto& v : value) {
					if (v == "") {
						container.push_back(0.0);
					} else {
						container.push_back(stod(v));
					}
				}
			}
			else if constexpr (std::is_same_v<typename CONTAINER::value_type, std::string> && std::is_same_v<typename T::value_type, double>) {
				for (const auto& v : value) {
					container.push_back(ToString(v));
				}
			}
			else {
				throw RuntimeError("Invalid operation");
			}
		}
		
		bool EntryPointMatches(const EntryPoint& entryPoint, const Var& ref) {
			if (entryPoint.ref.type == VOID) {
				if (ref.type != Var::Void) return false;
			} else {
				if (ref.type == Var::Void) return false;
				if (ref.type == Var::Numeric) {
					if (int64_t(ref) != int64_t(std::round(MemGetNumeric(entryPoint.ref)))) return false;
				} else if (ref.type == Var::Text) {
					if (std::string(ref) != MemGetText(entryPoint.ref)) return false;
				} else if (ref.type >= Var::Object) {
					if (ref.addrValue != MemGetObject(entryPoint.ref).addrValue) return false;
				} else {
					assert(!"Invalid ref type");
				}
			}
			return true;
		}
		
		void RunCode(const std::vector<ByteCode>& program, uint32_t index = 0);
		
	public:
		bool HasTick() {
			return assembly && assembly->functionRefs.contains("system.tick");
		}
		bool HasTimers() {
			return assembly && assembly->timers.size() > 0;
		}
		bool HasInputs() {
			return assembly && assembly->inputs.size() > 0;
		}
		bool ShouldRunContinuously() {
			return HasTick() || HasTimers() || HasInputs();
		}

		bool RunInit() {
			if (assembly) {
				currentCycleInstructions = 0;
				cycleState = CycleState::VARS_INIT;
				RunCode(assembly->rom_vars_init);
				if (assembly->functionRefs.contains("system.init")) {
					cycleState = CycleState::SYSTEM_INIT;
					RunCode(assembly->rom_program, assembly->functionRefs["system.init"]);
				}
				return true;
			}
			return false;
		}
		
		void RunCycle() {
			cycleState = CycleState::RUN;
			assert(assembly);
			currentCycleInstructions = 0;
			double time = GetCurrentTimestamp();
			
			// Tick
			if (assembly->functionRefs.contains("system.tick")) {
				RunCode(assembly->rom_program, assembly->functionRefs["system.tick"]);
			}
			
			// Timers
			for (size_t i = 0; i < assembly->timers.size(); ++i) {
				double lastRun = timersLastRun[i];
				double interval = assembly->timers[i].interval;
				if (lastRun + interval < time) {
					RunCode(assembly->rom_program, assembly->timers[i].addr);
					timersLastRun[i] = time;
				}
			}
		}
		
		void RunInput(uint32_t port, const std::vector<Var>& args) {
			if (assembly->inputs.contains(port)) {
				auto& input = assembly->inputs[port];
				
				// Write args
				for (size_t i = 0; i < args.size(); ++i) {
					if (i >= input.args.size()) break;
					const Var& arg = args[i];
					ByteCode dst = input.args[i];
					if (IsNumeric(dst) && arg.type == Var::Numeric) {
						MemSet(arg.numericValue, dst);
					} else if (IsText(dst) && arg.type == Var::Text) {
						MemSet(arg.textValue, dst);
					} else if (IsNumeric(dst) && arg.type == Var::Text) {
						MemSet(arg.textValue, dst);
					} else if (IsText(dst) && arg.type == Var::Numeric) {
						MemSet(arg.numericValue, dst);
					} else if (IsObject(dst) && arg.type == Var::Object) {
						MemSetObject(arg.addrValue, dst); // Reference assignment for objects
					} else if (arg.type == Var::Void) {
						MemSet("", dst);
					} else {
						throw RuntimeError("Input argument type mismatch");
					}
				}
				
				RunCode(assembly->rom_program, input.addr);
			}
		}
		
		bool HasEntryPoint(std::string name, const Var& ref = {}) {
			strtolower(name);
			for (const auto& entryPoint : assembly->entryPoints) {
				if (entryPoint.name == name && EntryPointMatches(entryPoint, ref)) {
					return true;
				}
			}
			return false;
		}
		
		bool RunEntryPoint(std::string name, const Var& ref = {}, const std::vector<Var>& args = {}) {
			bool found = false;
			strtolower(name);
			for (const auto& entryPoint : assembly->entryPoints) {
				if (entryPoint.name == name && EntryPointMatches(entryPoint, ref)) {
					found = true;
					
					// Write args
					for (size_t i = 0; i < args.size(); ++i) {
						if (i >= entryPoint.args.size()) break;
						const Var& arg = args[i];
						ByteCode dst = entryPoint.args[i];
						if (IsNumeric(dst) && arg.type == Var::Numeric) {
							MemSet(arg.numericValue, dst);
						} else if (IsText(dst) && arg.type == Var::Text) {
							MemSet(arg.textValue, dst);
						} else if (IsObject(dst) && arg.type == Var::Object) {
							MemSetObject(arg.addrValue, dst); // Reference assignment for objects
						} else {
							throw RuntimeError(name + " argument type mismatch");
						}
					}
					
					RunCode(assembly->rom_program, entryPoint.addr);
				}
			}
			return found;
		}
		
	};

#pragma endregion

}

#ifdef XENONCODE_IMPLEMENTATION

	namespace XC_NAMESPACE {
		
		std::vector<std::string> Implementation::entryPoints {};
		std::unordered_map<std::string, ObjectType> Device::objectTypesByName {};
		std::unordered_map<uint8_t, std::string> Device::objectNamesById {};
		std::vector<std::string> Device::objectTypesList {};
		std::unordered_map<std::string, Device::FunctionInfo> Device::deviceFunctionsByName {};
		std::unordered_map<uint32_t/*24 least significant bits only*/, std::string> Device::deviceFunctionNamesById {};
		std::unordered_map<uint32_t/*24 least significant bits only*/, DeviceFunction> Device::deviceFunctionsById {};
		std::unordered_map<uint8_t, std::vector<std::string>> Device::deviceFunctionsList {};
		OutputFunction Device::outputFunction = [](Computer*, uint32_t, const std::vector<Var>&){};
	
		void Computer::RunCode(const std::vector<ByteCode>& program, uint32_t index) {
			if (!assembly) return;
			if (program.size() <= index) return;
			
			// Find current file and line for debug
			std::string currentFile = currentFileByAddr[index];
			uint32_t currentLine = currentLineByAddr[index];
			for (int32_t tmpIndex = index; tmpIndex >= 0; --tmpIndex) {
				if (currentLine == 0 && program[tmpIndex].type == LINENUMBER) {
					currentLine = program[tmpIndex].value;
					currentLineByAddr[index] = currentLine;
				} else if (currentFile == "" && program[tmpIndex].type == SOURCEFILE) {
					if (program[tmpIndex].value < assembly->sourceFiles.size()) {
						currentFile = assembly->sourceFiles[program[tmpIndex].value];
						currentFileByAddr[index] = currentFile;
					}
				} else if (currentLine != 0 && currentFile != "") {
					break;
				}
			}
			
			// IPC
			auto ipcCheck = [this](int ipcPenalty = 1){
				if (capability.ipc > 0) {
					currentCycleInstructions += ipcPenalty;
					if (currentCycleInstructions > capability.ipc) {
						throw RuntimeError("Maximum IPC exceeded");
					}
				}
			};
			
			bool eol = false;
			auto nextCode = [&]() -> ByteCode {
				if (!eol && index+1 < program.size()) {
					return program[++index];
				}
				eol = true;
				return CODE_TYPE::VOID;
			};
			try {
				while (index < program.size()) {
					eol = false;
					const ByteCode& code = program[index];
					switch (code.type) {
						case RETURN: return;
						case VOID: break;
						case SOURCEFILE:{
							if (code.value < assembly->sourceFiles.size()) {
								currentFile = assembly->sourceFiles[code.value];
							}
						}break;
						case LINENUMBER:{
							currentLine = code.value;
						}break;
						case OP: {
							ipcCheck();
							switch (code.rawValue) {
								case SET: {// [ARRAY_INDEX|OBJ_KEY ifindexnone[REF_NUM]|REF_KEY] REF_DST [REF_VALUE]orZero
									ByteCode dst = nextCode();
									uint32_t arr_index = ARRAY_INDEX_NONE;
									if (dst.type == ARRAY_INDEX) {
										arr_index = dst.value;
										if (arr_index == ARRAY_INDEX_NONE) {
											arr_index = uint32_t(MemGetNumeric(nextCode()));
										}
										dst = nextCode();
									} else if (dst.type == OBJ_KEY) {
										ByteCode key = nextCode();
										dst = nextCode();
										if (dst.type != STORAGE_VAR_TEXT && dst.type != RAM_VAR_TEXT) {
											throw RuntimeError("Invalid Object Key indexing");
										}
										switch (key.type) {
											case STORAGE_VAR_TEXT:
											case RAM_VAR_TEXT:
											case ROM_CONST_TEXT:{
												std::string obj = MemGetText(dst);
												std::string k = MemGetText(key);
												std::string val = MemGetText(nextCode(), ARRAY_INDEX_NONE);
												if (k.length() == 0 || !isalpha_(k[0])) throw RuntimeError("Invalid Object Key");
												k = '.' + k + '{';
												if (obj.length() < k.length()) {
													MemSet(obj + k + val + '}', dst);
												} else for (auto it = obj.begin(); it != obj.end(); ++it) {
													if (it + k.length() > obj.end()) {
														MemSet(obj + k + val + '}', dst);
														break;
													}
													if (*it == '{') {
														int exprStack = 0;
														while (++it != obj.end()) {
															if (*it == '{') ++exprStack;
															else if (*it == '}') {
																if (exprStack == 0) {
																	++it;
																	break;
																}
																--exprStack;
															}
														}
														if (it == obj.end()) {
															MemSet(obj + k + val + '}', dst);
															break;
														}
													}
													if (std::equal(k.begin(), k.end(), it, [](unsigned char ch1, unsigned char ch2) { return std::tolower(ch1) == std::tolower(ch2); })) {
														int exprStack = 0;
														it += k.length();
														auto start = it;
														auto end = obj.end();
														while (it != obj.end()) {
															if (*it == '{') ++exprStack;
															else if (*it == '}') {
																if (exprStack == 0) {
																	end = it;
																	break;
																}
																--exprStack;
															}
															++it;
														}
														std::string newObj;
														newObj.reserve(obj.length() + val.length());
														newObj.append(obj.begin(), start);
														newObj.append(val);
														if (end == obj.end()) {
															newObj.append("}");
														} else {
															newObj.append(end, obj.end());
														}
														MemSet(newObj, dst);
														break;
													}
												}
											}break;
											default: throw RuntimeError("Invalid key");
										}
										break;
									}
									ByteCode val = nextCode();
									if (IsNumeric(dst) || (arr_index != ARRAY_INDEX_NONE && (dst.type == STORAGE_ARRAY_NUMERIC || dst.type == RAM_ARRAY_NUMERIC))) {
										MemSet(MemGetNumeric(val), dst, arr_index);
									} else if (IsText(dst) || (arr_index != ARRAY_INDEX_NONE && (dst.type == STORAGE_ARRAY_TEXT || dst.type == RAM_ARRAY_TEXT))) {
										MemSet(MemGetText(val), dst, arr_index);
									} else if (IsObject(dst) && IsObject(val) && arr_index == ARRAY_INDEX_NONE) {
										MemSetObject(MemGetObject(val).addrValue, dst); // Reference assignment for objects
									} else {
										throw RuntimeError("Invalid operation");
									}
								}break;
								case ADD: {// REF_DST REF_A REF_B
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									MemSet(MemGetNumeric(a) + MemGetNumeric(b), dst);
								}break;
								case SUB: {
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									MemSet(MemGetNumeric(a) - MemGetNumeric(b), dst);
								}break;
								case MUL: {
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									MemSet(MemGetNumeric(a) * MemGetNumeric(b), dst);
								}break;
								case DIV: {
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									double operand = MemGetNumeric(b);
									if (operand == 0) throw RuntimeError("Division by zero");
									MemSet(MemGetNumeric(a) / operand, dst);
								}break;
								case MOD: {
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									double value = MemGetNumeric(a);
									double operand = MemGetNumeric(b);
									if (std::fmod(value, 1.0) == 0.0 && std::fmod(operand, 1.0) == 0.0) {
										if (std::round(operand) == 0) throw RuntimeError("Division by zero");
										MemSet(double(int64_t(std::round(value)) % int64_t(std::round(operand))), dst);
									} else {
										if (operand == 0.0) throw RuntimeError("Division by zero");
										MemSet(std::fmod(value, operand), dst);
									}
								}break;
								case POW: {
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									MemSet(std::pow(MemGetNumeric(a), MemGetNumeric(b)), dst);
								}break;
								case CCT: {
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									MemSet(MemGetText(a) + MemGetText(b), dst);
								}break;
								case AND: {
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									MemSet(double(MemGetBoolean(a) && MemGetBoolean(b)), dst);
								}break;
								case ORR: {
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									MemSet(double(MemGetBoolean(a) || MemGetBoolean(b)), dst);
								}break;
								case XOR: {
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									MemSet(double(MemGetBoolean(a) != MemGetBoolean(b)), dst);
								}break;
								case EQQ: {
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									if (IsText(a) && IsText(b)) {
										MemSet(double(MemGetText(a) == MemGetText(b)), dst);
									} else if (IsNumeric(a) || IsNumeric(b)) {
										MemSet(double(std::abs(MemGetNumeric(a) - MemGetNumeric(b)) < EPSILON_DOUBLE), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case NEQ: {
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									if (IsText(a) && IsText(b)) {
										MemSet(double(MemGetText(a) != MemGetText(b)), dst);
									} else if (IsNumeric(a) || IsNumeric(b)) {
										MemSet(double(std::abs(MemGetNumeric(a) - MemGetNumeric(b)) >= EPSILON_DOUBLE), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case LST: {
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									MemSet(MemGetNumeric(a) < MemGetNumeric(b), dst);
								}break;
								case GRT: {
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									MemSet(MemGetNumeric(a) > MemGetNumeric(b), dst);
								}break;
								case LTE: {
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									MemSet(MemGetNumeric(a) <= MemGetNumeric(b), dst);
								}break;
								case GTE: {
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									MemSet(MemGetNumeric(a) >= MemGetNumeric(b), dst);
								}break;
								case INC: {// REF_NUM
									ByteCode ref = nextCode();
									MemSet(std::round(MemGetNumeric(ref)) + 1.0, ref);
								}break;
								case DEC: {
									ByteCode ref = nextCode();
									MemSet(std::round(MemGetNumeric(ref)) - 1.0, ref);
								}break;
								case NOT: {// REF_DST REF_VAL
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									if (IsNumeric(val)) {
										MemSet(double(!MemGetBoolean(val)), dst);
									} else if (IsText(dst) || IsText(val)) {
										std::string v = MemGetText(val);
										MemSet(double(v == "" || v == "0"), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case FLR: {// REF_DST REF_NUM
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									MemSet(std::floor(MemGetNumeric(val)), dst);
								}break;
								case CIL: {
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									MemSet(std::ceil(MemGetNumeric(val)), dst);
								}break;
								case RND: {
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									MemSet(std::round(MemGetNumeric(val)), dst);
								}break;
								case SIN: {
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									MemSet(std::sin(MemGetNumeric(val)), dst);
								}break;
								case COS: {
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									MemSet(std::cos(MemGetNumeric(val)), dst);
								}break;
								case TAN: {
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									MemSet(std::tan(MemGetNumeric(val)), dst);
								}break;
								case ASI: {
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									MemSet(std::asin(MemGetNumeric(val)), dst);
								}break;
								case ACO: {
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									MemSet(std::acos(MemGetNumeric(val)), dst);
								}break;
								case ATA: {
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									ByteCode val2 = nextCode();
									if (val2) {
										MemSet(std::atan2(MemGetNumeric(val), MemGetNumeric(val2)), dst);
									} else {
										MemSet(std::atan(MemGetNumeric(val)), dst);
									}
								}break;
								case ABS: {
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									MemSet(std::abs(MemGetNumeric(val)), dst);
								}break;
								case FRA: {
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									double intpart;
									MemSet(std::modf(MemGetNumeric(val), &intpart), dst);
								}break;
								case SQR: {
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									MemSet(std::sqrt(MemGetNumeric(val)), dst);
								}break;
								case SIG: {
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									ByteCode defaultVal = nextCode();
									double num = MemGetNumeric(val);
									if (num > 0.0) num = 1.0;
									else if (num < 0.0) num = -1.0;
									else if (defaultVal) num = MemGetNumeric(defaultVal);
									MemSet(num, dst);
								}break;
								case LOG: {// REF_DST REF_NUM REF_BASE
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									ByteCode base = nextCode();
									double b = MemGetNumeric(base);
									if (b == 0) b = 10.0;
									MemSet(std::log(MemGetNumeric(val)) / std::log(b), dst);
								}break;
								case CLP: {// REF_DST REF_NUM REF_MIN REF_MAX
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									ByteCode min = nextCode();
									ByteCode max = nextCode();
									double minVal = MemGetNumeric(min);
									double maxVal = MemGetNumeric(max);
									if (minVal > maxVal) throw RuntimeError("Invalid clamp operation (min > max)");
									MemSet(std::clamp(MemGetNumeric(val), minVal, maxVal), dst);
								}break;
								case STP: {// REF_DST REF_T1 REF_T2 REF_NUM
									ByteCode dst = nextCode();
									ByteCode t1 = nextCode();
									ByteCode t2 = nextCode();
									ByteCode val = nextCode();
									if (val.type == VOID) {
										val = t2;
										MemSet(step(MemGetNumeric(t1), MemGetNumeric(val)), dst);
									} else {
										MemSet(step(MemGetNumeric(t1), MemGetNumeric(t2), MemGetNumeric(val)), dst);
									}
								}break;
								case SMT: {
									ByteCode dst = nextCode();
									ByteCode t1 = nextCode();
									ByteCode t2 = nextCode();
									ByteCode val = nextCode();
									MemSet(smoothstep(MemGetNumeric(t1), MemGetNumeric(t2), MemGetNumeric(val)), dst);
								}break;
								case LRP: {
									ByteCode dst = nextCode();
									ByteCode t1 = nextCode();
									ByteCode t2 = nextCode();
									ByteCode val = nextCode();
									MemSet(std::lerp(MemGetNumeric(t1), MemGetNumeric(t2), MemGetNumeric(val)), dst);
								}break;
								case NUM: {// REF_DST REF_SRC
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									if (IsNumeric(dst) && IsText(val)) {
										std::string str = MemGetText(val);
										MemSet(str==""? 0.0 : stod(str), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case TXT: {// REF_DST REF_SRC [REPLACEMENT_VARS ...]
									//TODO: support C++20 format specifiers in the future: https://en.cppreference.com/w/cpp/utility/format/formatter#Standard_format_specification
									ByteCode dst = nextCode();
									ByteCode src = nextCode();
									std::vector<ByteCode> args {};
									for (ByteCode c; (c = nextCode()).type != VOID;) {
										args.emplace_back(c);
									}
									ipcCheck(args.size());
									if (IsText(dst) && (IsNumeric(src) || (IsText(src) && !args.empty()))) {
										if (IsNumeric(src)) {
											MemSet(ToString(MemGetNumeric(src)), dst);
										} else {
											size_t pos = 0;
											std::string txt = MemGetText(src);
											for (ByteCode c : args) {
												size_t p1 = txt.find('{', pos);
												size_t p2 = txt.find('}', p1);
												if (p1 == std::string::npos || p2 == std::string::npos || p1 > p2) break;
												std::string format = txt.substr(p1+1, p2-p1-1);
												std::string before = txt.substr(0, p1);
												std::string after = txt.substr(p2+1, txt.length()-p2-1);
												// {}
												if (format == "") {
													txt = before + (IsNumeric(c)? ToString(MemGetNumeric(c)) : MemGetText(c)) + after;
												}
												// {0} {00} {0e} {0e.00} {0.0} {0000.00}
												else if (format[0] == '0') {
													if (format.length() == 1) {
														// rounded to int
														txt = before + ToString(int64_t(std::round(MemGetNumeric(c)))) + after;
													} else {
														std::stringstream str;
														size_t ePos = format.find('e');
														bool e = ePos != std::string::npos;
														size_t dotPos = format.find('.');
														bool hasDecimal = dotPos != std::string::npos;
														int beforeDecimal = hasDecimal? dotPos : (format.length() - e);
														int afterDecimal = hasDecimal? (format.length() - dotPos - 1) : 0;
														if (e && hasDecimal) {
															if (ePos > dotPos) {
																assert(afterDecimal > 0);
																--afterDecimal;
															} else {
																assert(beforeDecimal > 0);
																--beforeDecimal;
															}
														}
														str << std::setfill('0');
														if (!e) str << std::setw(beforeDecimal + hasDecimal + afterDecimal);
														str << std::setprecision(afterDecimal);
														str << (e? std::scientific : std::fixed);
														str << MemGetNumeric(c);
														txt = before + str.str() + after;
													}
												} else break;
												pos = txt.length() - after.length();
											}
											MemSet(txt, dst);
										}
									} else throw RuntimeError("Invalid operation");
								}break;
								case DEV: {// DEVICE_FUNCTION_INDEX RET_DST [REF_ARG ...]
									ByteCode dev = nextCode();
									if (dev.type != DEVICE_FUNCTION_INDEX || !Device::deviceFunctionsById.contains(dev.value)) {
										throw RuntimeError("Invalid device function");
									}
									ByteCode dst = 0;
									if (Device::deviceFunctionsByName.at(Device::deviceFunctionNamesById.at(dev.value)).returnType != "") {
										dst = nextCode();
									}
									std::vector<Var> args {};
									for (ByteCode c; (c = nextCode()).type != VOID;) {
										if (IsNumeric(c)) args.emplace_back(MemGetNumeric(c));
										else if (IsText(c)) args.emplace_back(MemGetText(c));
										else if (IsObject(c)) args.emplace_back(MemGetObject(c));
										else throw RuntimeError("Invalid operation");
									}
									Var ret = Device::deviceFunctionsById[dev.value](this, args);
									if (dst && dst != DISCARD && ret.type != Var::Void) {
										if (ret.type == Var::Numeric) {
											MemSet(ret.numericValue, dst);
										} else if (ret.type == Var::Text) {
											MemSet(ret.textValue, dst);
										} else {
											if (ret.type >= Var::Object && Device::objectNamesById.contains(ret.type & (Var::Object-1))) {
												MemSetObject(ret.addrValue, dst);
											} else {
												throw RuntimeError("Invalid operation");
											}
										}
									}
								}break;
								case OUT: {// REF_NUM [REF_ARG ...]
									ByteCode io = nextCode();
									if (!IsNumeric(io)) {
										throw RuntimeError("Invalid output index");
									}
									std::vector<Var> args {};
									for (ByteCode c; (c = nextCode()).type != VOID;) {
										if (IsNumeric(c)) args.emplace_back(MemGetNumeric(c));
										else args.emplace_back(MemGetText(c));
									}
									Device::outputFunction(this, (uint32_t)MemGetNumeric(io), args);
								}break;
								case APP: {// REF_ARR REF_VALUE [REF_VALUE ...]
									ByteCode arr = nextCode();
									std::vector<ByteCode> args {};
									for (ByteCode c; (c = nextCode()).type != VOID;) {
										args.emplace_back(c);
									}
									if (args.size() == 0) throw RuntimeError("Not enough arguments");
									if (!IsArray(arr)) throw RuntimeError("Not an array");
									ipcCheck(args.size());
									switch (arr.type) {
										case STORAGE_ARRAY_NUMERIC:
										case STORAGE_ARRAY_TEXT:{
											auto& array = GetStorage(arr);
											if (array.size() + args.size() > XC_MAX_ARRAY_SIZE) {
												throw RuntimeError("Maximum array size exceeded");
											}
											array.reserve(args.size());
											for (const auto& c : args) {
												if (IsNumeric(c)) {
													array.push_back(ToString(MemGetNumeric(c)));
												} else {
													array.push_back(MemGetText(c));
												}
											}
											storageDirty = true;
										}break;
										case RAM_ARRAY_NUMERIC:{
											auto& array = GetNumericArray(arr);
											if (array.size() + args.size() > XC_MAX_ARRAY_SIZE) {
												throw RuntimeError("Maximum array size exceeded");
											}
											array.reserve(args.size());
											for (const auto& c : args) array.push_back(MemGetNumeric(c));
										}break;
										case RAM_ARRAY_TEXT:{
											auto& array = GetTextArray(arr);
											if (array.size() + args.size() > XC_MAX_ARRAY_SIZE) {
												throw RuntimeError("Maximum array size exceeded");
											}
											array.reserve(args.size());
											for (const auto& c : args) array.push_back(MemGetText(c));
										}break;
									}
								}break;
								case CLR: {
									ByteCode arr = nextCode();
									if (!IsArray(arr)) throw RuntimeError("Not an array");
									switch (arr.type) {
										case STORAGE_ARRAY_NUMERIC:
										case STORAGE_ARRAY_TEXT:{
											auto& array = GetStorage(arr);
											array.clear();
											storageDirty = true;
										}break;
										case RAM_ARRAY_NUMERIC:{
											auto& array = GetNumericArray(arr);
											array.clear();
										}break;
										case RAM_ARRAY_TEXT:{
											auto& array = GetTextArray(arr);
											array.clear();
										}break;
									}
								}break;
								case POP: {
									ByteCode arr = nextCode();
									if (!IsArray(arr)) throw RuntimeError("Not an array");
									switch (arr.type) {
										case STORAGE_ARRAY_NUMERIC:
										case STORAGE_ARRAY_TEXT:{
											auto& array = GetStorage(arr);
											if (array.size() == 0) throw RuntimeError("Invalid operation on empty array");
											array.pop_back();
											storageDirty = true;
										}break;
										case RAM_ARRAY_NUMERIC:{
											auto& array = GetNumericArray(arr);
											if (array.size() == 0) throw RuntimeError("Invalid operation on empty array");
											array.pop_back();
										}break;
										case RAM_ARRAY_TEXT:{
											auto& array = GetTextArray(arr);
											if (array.size() == 0) throw RuntimeError("Invalid operation on empty array");
											array.pop_back();
										}break;
									}
								}break;
								case ASC: {
									ByteCode arr = nextCode();
									if (!IsArray(arr)) throw RuntimeError("Not an array");
									switch (arr.type) {
										case STORAGE_ARRAY_NUMERIC:{
											auto& array = GetStorage(arr);
											ipcCheck(array.size());
											std::vector<double> values {};
											values.reserve(array.size());
											for (const auto& val : array) values.push_back(val==""? 0.0 : stod(val));
											sort(values.begin(), values.end());
											array.clear();
											for (const auto& val : values) array.push_back(ToString(val));
											storageDirty = true;
										}break;
										case STORAGE_ARRAY_TEXT:{
											auto& array = GetStorage(arr);
											ipcCheck(array.size());
											sort(array.begin(), array.end());
											storageDirty = true;
										}break;
										case RAM_ARRAY_NUMERIC:{
											auto& array = GetNumericArray(arr);
											ipcCheck(array.size());
											sort(array.begin(), array.end());
										}break;
										case RAM_ARRAY_TEXT:{
											auto& array = GetTextArray(arr);
											ipcCheck(array.size());
											sort(array.begin(), array.end());
										}break;
									}
								}break;
								case DSC: {
									ByteCode arr = nextCode();
									if (!IsArray(arr)) throw RuntimeError("Not an array");
									switch (arr.type) {
										case STORAGE_ARRAY_NUMERIC:{
											auto& array = GetStorage(arr);
											ipcCheck(array.size());
											std::vector<double> values {};
											values.reserve(array.size());
											for (const auto& val : array) values.push_back(val==""? 0.0 : stod(val));
											sort(values.begin(), values.end(), std::greater<double>());
											array.clear();
											for (const auto& val : values) array.push_back(ToString(val));
											storageDirty = true;
										}break;
										case STORAGE_ARRAY_TEXT:{
											auto& array = GetStorage(arr);
											ipcCheck(array.size());
											sort(array.begin(), array.end(), std::greater<std::string>());
											storageDirty = true;
										}break;
										case RAM_ARRAY_NUMERIC:{
											auto& array = GetNumericArray(arr);
											ipcCheck(array.size());
											sort(array.begin(), array.end(), std::greater<double>());
										}break;
										case RAM_ARRAY_TEXT:{
											auto& array = GetTextArray(arr);
											ipcCheck(array.size());
											sort(array.begin(), array.end(), std::greater<std::string>());
										}break;
									}
								}break;
								case INS: {//insert REF_ARR REF_INDEX REF_VALUE [REF_VALUE ...]
									ByteCode arr = nextCode();
									ByteCode idx = nextCode();
									std::vector<ByteCode> args {};
									for (ByteCode c; (c = nextCode()).type != VOID;) {
										args.emplace_back(c);
									}
									if (args.size() == 0) throw RuntimeError("Not enough arguments");
									if (!IsArray(arr)) throw RuntimeError("Not an array");
									if (!IsNumeric(idx)) throw RuntimeError("Invalid array index");
									int32_t arr_index = (int)std::round(MemGetNumeric(idx));
									if (arr_index < 0) throw RuntimeError("Invalid array index");
									ipcCheck(args.size());
									switch (arr.type) {
										case STORAGE_ARRAY_NUMERIC:
										case STORAGE_ARRAY_TEXT:{
											auto& array = GetStorage(arr);
											if (array.size() + args.size() > XC_MAX_ARRAY_SIZE) {
												throw RuntimeError("Maximum array size exceeded");
											}
											std::vector<std::string> values{};
											values.reserve(args.size());
											for (const auto& c : args) {
												if (IsNumeric(c)) {
													values.push_back(ToString(MemGetNumeric(c)));
												} else {
													values.push_back(MemGetText(c));
												}
											}
											if (arr_index > (int)array.size()) throw RuntimeError("Invalid array index out of bounds");
											array.insert(array.begin()+arr_index, values.begin(), values.end());
											storageDirty = true;
										}break;
										case RAM_ARRAY_NUMERIC:{
											auto& array = GetNumericArray(arr);
											if (array.size() + args.size() > XC_MAX_ARRAY_SIZE) {
												throw RuntimeError("Maximum array size exceeded");
											}
											std::vector<double> values{};
											values.reserve(args.size());
											for (const auto& c : args) values.push_back(MemGetNumeric(c));
											if (arr_index > (int)array.size()) throw RuntimeError("Invalid array index out of bounds");
											array.insert(array.begin()+arr_index, values.begin(), values.end());
										}break;
										case RAM_ARRAY_TEXT:{
											auto& array = GetTextArray(arr);
											if (array.size() + args.size() > XC_MAX_ARRAY_SIZE) {
												throw RuntimeError("Maximum array size exceeded");
											}
											std::vector<std::string> values{};
											values.reserve(args.size());
											for (const auto& c : args) values.push_back(MemGetText(c));
											if (arr_index > (int)array.size()) throw RuntimeError("Invalid array index out of bounds");
											array.insert(array.begin()+arr_index, values.begin(), values.end());
										}break;
									}
								}break;
								case DEL: {//erase REF_ARR REF_INDEX [REF_INDEX_END]
									ByteCode arr = nextCode();
									ByteCode idx = nextCode();
									ByteCode idx2 = nextCode();
									if (!IsArray(arr)) throw RuntimeError("Not an array");
									if (!IsNumeric(idx)) throw RuntimeError("Invalid array index");
									int32_t arr_index = (int)std::round(MemGetNumeric(idx));
									int32_t index2 = idx2.type != VOID? (int)std::round(MemGetNumeric(idx2)) : arr_index;
									if (arr_index < 0 || index2 < 0) throw RuntimeError("Invalid array index");
									++index2;
									if (index2 <= arr_index) throw RuntimeError("Invalid array index");
									ipcCheck();
									switch (arr.type) {
										case STORAGE_ARRAY_NUMERIC:
										case STORAGE_ARRAY_TEXT:{
											auto& array = GetStorage(arr);
											if (index2 > (int)array.size()) throw RuntimeError("Invalid array index out of bounds");
											if (index2 == arr_index+1) {
												array.erase(array.begin()+arr_index);
											} else {
												array.erase(array.begin()+arr_index, array.begin()+index2);
											}
											storageDirty = true;
										}break;
										case RAM_ARRAY_NUMERIC:{
											auto& array = GetNumericArray(arr);
											if (index2 > (int)array.size()) throw RuntimeError("Invalid array index out of bounds");
											if (index2 == arr_index+1) {
												array.erase(array.begin()+arr_index);
											} else {
												array.erase(array.begin()+arr_index, array.begin()+index2);
											}
										}break;
										case RAM_ARRAY_TEXT:{
											auto& array = GetTextArray(arr);
											if (index2 > (int)array.size()) throw RuntimeError("Invalid array index out of bounds");
											if (index2 == arr_index+1) {
												array.erase(array.begin()+arr_index);
											} else {
												array.erase(array.begin()+arr_index, array.begin()+index2);
											}
										}break;
									}
								}break;
								case FLL: {//fill REF_ARR REF_QTY REF_VAL
									ByteCode arr = nextCode();
									ByteCode qty = nextCode();
									ByteCode val = nextCode();
									if (!IsArray(arr)) throw RuntimeError("Not an array");
									if (!IsNumeric(qty)) throw RuntimeError("Invalid qty");
									size_t count = (size_t)std::round(MemGetNumeric(qty));
									if (count > XC_MAX_ARRAY_SIZE) {
										throw RuntimeError("Maximum array size exceeded");
									}
									ipcCheck(count);
									switch (arr.type) {
										case STORAGE_ARRAY_NUMERIC:{
											auto& array = GetStorage(arr);
											array.clear();
											array.resize(count, ToString(MemGetNumeric(val)));
											storageDirty = true;
										}break;
										case STORAGE_ARRAY_TEXT:{
											auto& array = GetStorage(arr);
											array.clear();
											array.resize(count, MemGetText(val));
											storageDirty = true;
										}break;
										case RAM_ARRAY_NUMERIC:{
											auto& array = GetNumericArray(arr);
											array.clear();
											array.resize(count, std::round(MemGetNumeric(val)));
										}break;
										case RAM_ARRAY_TEXT:{
											auto& array = GetTextArray(arr);
											array.clear();
											array.resize(count, MemGetText(val));
										}break;
									}
								}break;
								case FRM: {//from REF_DST REF_VAL [REF_SEPARATOR]
									ByteCode arr = nextCode();
									ByteCode val = nextCode();
									ByteCode sep = nextCode();
									std::string separator = sep.type != VOID? MemGetText(sep) : "";
									if (!IsArray(arr) && !IsText(arr)) throw RuntimeError("Not an array or text");
									auto fillArray = [&](std::vector<auto>& dst){
										dst.clear();
										if (IsStorage(val)) {
											auto& otherArray = GetStorage(val);
											if (IsArray(val)) {
												if (separator != "") throw RuntimeError("Invalid operation");
												dst.reserve(otherArray.size());
												ArrayInsertAuto(dst, otherArray);
											} else if (separator == "") {
												size_t len = utf8length(otherArray[0]);
												dst.reserve(len);
												for (size_t i = 0; i < len; ++i) {
													ArrayInsertAuto(dst, utf8substr(otherArray[0], i, 1));
												}
											} else {
												std::string str = otherArray[0];
												while (str != "") {
													auto pos = str.find(separator);
													if (pos == std::string::npos) {
														ArrayInsertAuto(dst, str);
														break;
													} else {
														ArrayInsertAuto(dst, str.substr(0, pos));
														str = str.substr(pos + separator.length());
													}
												}
											}
										}
										else if (IsArray(val)) {
											switch (val.type) {
												case RAM_ARRAY_NUMERIC:{
													const auto& values = GetNumericArray(val);
													dst.reserve(values.size());
													ArrayInsertAuto(dst, values);
												}break;
												case RAM_ARRAY_TEXT:{
													const auto& values = GetTextArray(val);
													dst.reserve(values.size());
													ArrayInsertAuto(dst, values);
												}break;
												default: throw RuntimeError("Invalid operation");
											}
										}
										else if (IsNumeric(val)) {
											if (separator != "") throw RuntimeError("Invalid operation");
											double value = MemGetNumeric(val);
											std::string str = ToString(value);
											for (char c : str) {
												ArrayInsertAuto(dst, std::string(1, c));
											}
										}
										else if (IsText(val)) {
											std::string str = MemGetText(val);
											if (separator == "") {
												size_t len = utf8length(str);
												dst.reserve(len);
												for (size_t i = 0; i < len; ++i) {
													ArrayInsertAuto(dst, utf8substr(str, i, 1));
												}
											} else {
												while (str != "") {
													auto pos = str.find(separator);
													if (pos == std::string::npos) {
														ArrayInsertAuto(dst, str);
														break;
													} else {
														ArrayInsertAuto(dst, str.substr(0, pos));
														str = str.substr(pos + separator.length());
													}
												}
											}
										}
										else throw RuntimeError("Invalid operation");
										ipcCheck(dst.size() + 1);
									};
									switch (arr.type) {
										case STORAGE_ARRAY_NUMERIC:
										case STORAGE_ARRAY_TEXT:{
											fillArray(GetStorage(arr));
											storageDirty = true;
										}break;
										case RAM_ARRAY_NUMERIC:{
											fillArray(GetNumericArray(arr));
										}break;
										case RAM_ARRAY_TEXT:{
											fillArray(GetTextArray(arr));
										}break;
										case RAM_VAR_TEXT:
										case STORAGE_VAR_TEXT:{
											std::string dst = "";
											if (IsStorage(val)) {
												auto& otherArray = GetStorage(val);
												if (IsArray(val)) {
													for (const auto& v : otherArray) {
														if (dst != "") dst += separator;
														dst += v;
													}
												} else if (separator == "") {
													dst = otherArray[0];
												} else {
													throw RuntimeError("Invalid operation");
												}
											}
											else if (IsArray(val)) {
												switch (val.type) {
													case RAM_ARRAY_NUMERIC:{
														for (const auto& v : GetNumericArray(val)) {
															if (dst != "") dst += separator;
															dst += ToString(v);
														}
													}break;
													case RAM_ARRAY_TEXT:{
														for (const auto& v : GetTextArray(val)) {
															if (dst != "") dst += separator;
															dst += v;
														}
													}break;
													default: throw RuntimeError("Invalid operation");
												}
											}
											else if (IsNumeric(val)) {
												if (separator != "") throw RuntimeError("Invalid operation");
												double value = MemGetNumeric(val);
												dst = ToString(value);
											}
											else if (IsText(val)) {
												if (separator != "") throw RuntimeError("Invalid operation");
												dst = MemGetText(val);
											}
											else throw RuntimeError("Invalid operation");
											ipcCheck(dst.length() + 1);
											if (arr.type == STORAGE_VAR_TEXT) {
												StorageSet(dst, arr);
												storageDirty = true;
											} else {
												MemSet(dst, arr);
											}
										}break;
									}
								}break;
								case SIZ: {//size REF_DST (REF_ARR | REF_TXT)
									ByteCode dst = nextCode();
									ByteCode ref = nextCode();
									if (!IsVar(dst)) throw RuntimeError("Invalid operation");
									if (!IsArray(ref) && !IsText(ref)) throw RuntimeError("Not an array or text");
									if (IsArray(ref)) {
										switch (ref.type) {
											case STORAGE_ARRAY_NUMERIC:
											case STORAGE_ARRAY_TEXT:{
												auto& array = GetStorage(ref);
												MemSet(array.size(), dst);
											}break;
											case RAM_ARRAY_NUMERIC:{
												auto& array = GetNumericArray(ref);
												MemSet(array.size(), dst);
											}break;
											case RAM_ARRAY_TEXT:{
												auto& array = GetTextArray(ref);
												MemSet(array.size(), dst);
											}break;
										}
									} else {
										MemSet(utf8length(MemGetText(ref)), dst);
									}
								}break;
								case LAS: {
									ByteCode dst = nextCode();
									ByteCode ref = nextCode();
									if (!IsArray(ref) && !IsText(ref)) throw RuntimeError("Not an array or text");
									if (IsArray(ref)) {
										switch (ref.type) {
											case STORAGE_ARRAY_NUMERIC:{
												auto& array = GetStorage(ref);
												if (array.size() == 0) throw RuntimeError("Empty array");
												std::string last = array.back();
												MemSet(last==""? 0.0 : stod(last), dst);
											}break;
											case STORAGE_ARRAY_TEXT:{
												auto& array = GetStorage(ref);
												if (array.size() == 0) throw RuntimeError("Empty array");
												MemSet(array.back(), dst);
											}break;
											case RAM_ARRAY_NUMERIC:{
												auto& array = GetNumericArray(ref);
												if (array.size() == 0) throw RuntimeError("Empty array");
												MemSet(array.back(), dst);
											}break;
											case RAM_ARRAY_TEXT:{
												auto& array = GetTextArray(ref);
												if (array.size() == 0) throw RuntimeError("Empty array");
												MemSet(array.back(), dst);
											}break;
										}
									} else {
										const std::string& text = MemGetText(ref);
										size_t len = utf8length(text);
										MemSet(utf8substr(text, len-1, 1), dst);
									}
								}break;
								case FND: {//find REF_DST (REF_ARR | REF_TXT) REF_VAL
									ByteCode dst = nextCode();
									ByteCode ref = nextCode();
									ByteCode val = nextCode();
									if (!IsVar(dst)) throw RuntimeError("Invalid operation");
									if (!IsArray(ref) && !IsText(ref)) throw RuntimeError("Not an array or text");
									if (IsArray(ref)) {
										switch (ref.type) {
											case STORAGE_ARRAY_NUMERIC:{
												bool found = false;
												auto& array = GetStorage(ref);
												for (size_t i = 0; i < array.size(); ++i) {
													if ((array[i]==""? 0.0 : stod(array[i])) == MemGetNumeric(val)) {
														MemSet(int(i), dst);
														found = true;
														break;
													}
												}
												if (!found) MemSet(-1, dst);
											}break;
											case STORAGE_ARRAY_TEXT:{
												auto& array = GetStorage(ref);
												auto pos = std::find(array.begin(), array.end(), MemGetText(val));
												MemSet(pos != array.end()? int(pos - array.begin()) : -1, dst);
											}break;
											case RAM_ARRAY_NUMERIC:{
												auto& array = GetNumericArray(ref);
												auto pos = std::find(array.begin(), array.end(), MemGetNumeric(val));
												MemSet(pos != array.end()? int(pos - array.begin()) : -1, dst);
											}break;
											case RAM_ARRAY_TEXT:{
												auto& array = GetTextArray(ref);
												auto pos = std::find(array.begin(), array.end(), MemGetText(val));
												MemSet(pos != array.end()? int(pos - array.begin()) : -1, dst);
											}break;
										}
									} else {
										auto pos = MemGetText(ref).find(MemGetText(val));
										MemSet(pos != std::string::npos? int(utf8length(MemGetText(ref).substr(0, pos))) : -1, dst);
									}
								}break;
								case CON: {//contains REF_DST (REF_ARR | REF_TXT) REF_VAL
									ByteCode dst = nextCode();
									ByteCode ref = nextCode();
									ByteCode val = nextCode();
									if (!IsVar(dst)) throw RuntimeError("Invalid operation");
									if (!IsArray(ref) && !IsText(ref)) throw RuntimeError("Not an array or text");
									if (IsArray(ref)) {
										switch (ref.type) {
											case STORAGE_ARRAY_NUMERIC:{
												auto& array = GetStorage(ref);
												for (const auto& v : array) {
													if ((v==""? 0.0 : stod(v)) == MemGetNumeric(val)) {
														MemSet(1, dst);
														break;
													}
												}
											}break;
											case STORAGE_ARRAY_TEXT:{
												auto& array = GetStorage(ref);
												auto pos = std::find(array.begin(), array.end(), MemGetText(val));
												MemSet(pos != array.end()? 1 : 0, dst);
											}break;
											case RAM_ARRAY_NUMERIC:{
												auto& array = GetNumericArray(ref);
												auto pos = std::find(array.begin(), array.end(), MemGetNumeric(val));
												MemSet(pos != array.end()? 1 : 0, dst);
											}break;
											case RAM_ARRAY_TEXT:{
												auto& array = GetTextArray(ref);
												auto pos = std::find(array.begin(), array.end(), MemGetText(val));
												MemSet(pos != array.end()? 1 : 0, dst);
											}break;
										}
									} else {
										auto pos = MemGetText(ref).find(MemGetText(val));
										MemSet(pos != std::string::npos? 1 : 0, dst);
									}
								}break;
								case MIN: {// REF_DST (REF_ARR | (REF_NUM [REF_NUM ...]))
									ByteCode dst = nextCode();
									std::vector<ByteCode> args {};
									for (ByteCode c; (c = nextCode()).type != VOID;) {
										args.emplace_back(c);
									}
									if (!IsVar(dst)) throw RuntimeError("Invalid operation");
									if (args.size() == 0) throw RuntimeError("Not enough arguments");
									ByteCode arr = args[0];
									double min = std::numeric_limits<double>::max();
									if (IsArray(arr)) {
										switch (arr.type) {
											case STORAGE_ARRAY_NUMERIC:{
												auto& array = GetStorage(arr);
												ipcCheck(array.size());
												if (array.size() == 0) min = 0;
												else for (const auto& val : array) {
													double value = val==""? 0.0 : stod(val);
													if (value < min) min = value;
												}
											}break;
											case RAM_ARRAY_NUMERIC:{
												auto& array = GetNumericArray(arr);
												ipcCheck(array.size());
												if (array.size() == 0) min = 0;
												else for (const auto& value : array) {
													if (value < min) min = value;
												}
											}break;
											case STORAGE_ARRAY_TEXT:
											case RAM_ARRAY_TEXT:{
												throw RuntimeError("Invalid operation (not numbers)");
											}break;
										}
									} else {
										ipcCheck(args.size());
										for (const ByteCode& c : args) {
											double value = MemGetNumeric(c);
											if (value < min) min = value;
										}
									}
									MemSet(min, dst);
								}break;
								case MAX: {
									ByteCode dst = nextCode();
									std::vector<ByteCode> args {};
									for (ByteCode c; (c = nextCode()).type != VOID;) {
										args.emplace_back(c);
									}
									if (!IsVar(dst)) throw RuntimeError("Invalid operation");
									if (args.size() == 0) throw RuntimeError("Not enough arguments");
									ByteCode arr = args[0];
									double max = std::numeric_limits<double>::lowest();
									if (IsArray(arr)) {
										switch (arr.type) {
											case STORAGE_ARRAY_NUMERIC:{
												auto& array = GetStorage(arr);
												ipcCheck(array.size());
												if (array.size() == 0) max = 0;
												else for (const auto& val : array) {
													double value = val==""? 0.0 : stod(val);
													if (value > max) max = value;
												}
											}break;
											case RAM_ARRAY_NUMERIC:{
												auto& array = GetNumericArray(arr);
												ipcCheck(array.size());
												if (array.size() == 0) max = 0;
												else for (const auto& value : array) {
													if (value > max) max = value;
												}
											}break;
											case STORAGE_ARRAY_TEXT:
											case RAM_ARRAY_TEXT:{
												throw RuntimeError("Invalid operation");
											}break;
										}
									} else {
										ipcCheck(args.size());
										for (const ByteCode& c : args) {
											double value = MemGetNumeric(c);
											if (value > max) max = value;
										}
									}
									MemSet(max, dst);
								}break;
								case AVG: {
									ByteCode dst = nextCode();
									std::vector<ByteCode> args {};
									for (ByteCode c; (c = nextCode()).type != VOID;) {
										args.emplace_back(c);
									}
									if (!IsVar(dst)) throw RuntimeError("Invalid operation");
									if (args.size() == 0) throw RuntimeError("Not enough arguments");
									ByteCode arr = args[0];
									double total = 0;
									double size = 0;
									if (IsArray(arr)) {
										switch (arr.type) {
											case STORAGE_ARRAY_NUMERIC:{
												auto& array = GetStorage(arr);
												ipcCheck(array.size());
												size = array.size();
												if (size == 0) size = 1;
												else for (const auto& value : array) {
													total += value==""? 0.0 : stod(value);
												}
											}break;
											case RAM_ARRAY_NUMERIC:{
												auto& array = GetNumericArray(arr);
												ipcCheck(array.size());
												size = array.size();
												if (size == 0) size = 1;
												else for (const auto& value : array) {
													total += value;
												}
											}break;
											case STORAGE_ARRAY_TEXT:
											case RAM_ARRAY_TEXT:{
												throw RuntimeError("Invalid operation");
											}break;
										}
									} else {
										ipcCheck(args.size());
										for (const ByteCode& c : args) {
											double value = MemGetNumeric(c);
											total += value;
										}
										size = args.size();
									}
									MemSet(total / size, dst);
								}break;
								case SUM: {
									ByteCode dst = nextCode();
									std::vector<ByteCode> args {};
									for (ByteCode c; (c = nextCode()).type != VOID;) {
										args.emplace_back(c);
									}
									if (!IsVar(dst)) throw RuntimeError("Invalid operation");
									if (args.size() == 0) throw RuntimeError("Not enough arguments");
									ByteCode arr = args[0];
									double total = 0;
									if (IsArray(arr)) {
										switch (arr.type) {
											case STORAGE_ARRAY_NUMERIC:{
												auto& array = GetStorage(arr);
												ipcCheck(array.size());
												for (const auto& value : array) {
													total += value==""? 0.0 : stod(value);
												}
											}break;
											case RAM_ARRAY_NUMERIC:{
												auto& array = GetNumericArray(arr);
												ipcCheck(array.size());
												for (const auto& value : array) {
													total += value;
												}
											}break;
											case STORAGE_ARRAY_TEXT:
											case RAM_ARRAY_TEXT:{
												throw RuntimeError("Invalid operation");
											}break;
										}
									} else {
										ipcCheck(args.size());
										for (const ByteCode& c : args) {
											double value = MemGetNumeric(c);
											total += value;
										}
									}
									MemSet(total, dst);
								}break;
								case MED: {
									ByteCode dst = nextCode();
									std::vector<ByteCode> args {};
									for (ByteCode c; (c = nextCode()).type != VOID;) {
										args.emplace_back(c);
									}
									if (!IsVar(dst)) throw RuntimeError("Invalid operation");
									if (args.size() == 0) throw RuntimeError("Not enough arguments");
									ByteCode arr = args[0];
									double med = 0;
									if (IsArray(arr)) {
										switch (arr.type) {
											case STORAGE_ARRAY_NUMERIC:{
												auto& array = GetStorage(arr);
												ipcCheck(array.size());
												if (array.size() > 0) {
													std::string val = array[array.size()/2];
													med = val==""? 0.0 : stod(val);
												}
											}break;
											case RAM_ARRAY_NUMERIC:{
												auto& array = GetNumericArray(arr);
												ipcCheck(array.size());
												if (array.size() > 0) {
													med = array[array.size()/2];
												}
											}break;
											case STORAGE_ARRAY_TEXT:
											case RAM_ARRAY_TEXT:{
												throw RuntimeError("Invalid operation");
											}break;
										}
									} else {
										throw RuntimeError("Invalid operation");
									}
									MemSet(med, dst);
								}break;
								case SBS: {// REF_DST REF_SRC REF_START REF_LENGTH
									ByteCode dst = nextCode();
									ByteCode src = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									std::string text = MemGetText(src);
									int start = (int)std::round(MemGetNumeric(a));
									int len = b.type != VOID? (int)std::round(MemGetNumeric(b)) : int(utf8length(text)-start);
									// if (!IsText(dst)) throw RuntimeError("Invalid operation");
									MemSet(utf8substr(text, start, len), dst);
								}break;
								case IDX: {// REF_DST REF_ARR|REF_TEXT ARRAY_INDEX|OBJ_KEY ifindexnone[REF_NUM]|REF_KEY
									ByteCode dst = nextCode();
									ByteCode arr = nextCode();
									ByteCode idx = nextCode();
									if (idx.type != ARRAY_INDEX && idx.type != OBJ_KEY) throw RuntimeError("Invalid indexing type");
									if (idx.type == ARRAY_INDEX) {
										uint32_t arr_index = idx.value;
										if (arr_index == ARRAY_INDEX_NONE) {
											idx = nextCode();
											if (!IsNumeric(idx)) throw RuntimeError("Invalid array index type");
											arr_index = (int)std::round(MemGetNumeric(idx));
										}
										switch (arr.type) {
											case STORAGE_ARRAY_NUMERIC:
											case RAM_ARRAY_NUMERIC:
												MemSet(MemGetNumeric(arr, arr_index), dst);
											break;
											case STORAGE_ARRAY_TEXT:
											case STORAGE_VAR_TEXT:
											case RAM_ARRAY_TEXT:
											case RAM_VAR_TEXT:
											case ROM_CONST_TEXT:
												MemSet(MemGetText(arr, arr_index), dst);
											break;
											default: throw RuntimeError("Not an array");
										}
									} else {
										if (arr.type != STORAGE_VAR_TEXT && arr.type != RAM_VAR_TEXT) {
											throw RuntimeError("Invalid Object Key indexing");
										}
										ByteCode key = nextCode();
										switch (key.type) {
											case STORAGE_VAR_TEXT:
											case RAM_VAR_TEXT:
											case ROM_CONST_TEXT:{
												std::string obj = MemGetText(arr);
												std::string k = MemGetText(key);
												if (k.length() == 0 || !isalpha_(k[0])) throw RuntimeError("Invalid Object Key");
												k = '.' + k + '{';
												if (obj.length() < k.length()) {
													MemSet("", dst);
												} else for (auto it = obj.begin(); it != obj.end(); ++it) {
													if (it + k.length() > obj.end()) {
														MemSet("", dst);
														break;
													}
													if (*it == '{') {
														int exprStack = 0;
														while (++it != obj.end()) {
															if (*it == '{') ++exprStack;
															else if (*it == '}') {
																if (exprStack == 0) {
																	++it;
																	break;
																}
																--exprStack;
															}
														}
														if (it == obj.end()) {
															MemSet("", dst);
															break;
														}
													}
													if (std::equal(k.begin(), k.end(), it, [](unsigned char ch1, unsigned char ch2) { return std::tolower(ch1) == std::tolower(ch2); })) {
														it += k.length();
														int exprStack = 0;
														std::string val;
														while (it != obj.end()) {
															if (*it == '{') ++exprStack;
															else if (*it == '}') {
																if (exprStack == 0) break;
																--exprStack;
															}
															val += *(it++);
														}
														MemSet(val, dst);
														break;
													}
												}
											}break;
											default: throw RuntimeError("Invalid key");
										}
									}
								}break;
								case JMP: {// ADDR
									ByteCode addr = nextCode();
									if (addr.type != ADDR) throw RuntimeError("Invalid address");
									recursion_depth++;
									ipcCheck(recursion_depth * 2);
									if (recursion_depth > XC_MAX_CALL_DEPTH) {
										throw RuntimeError("Max call recursion_depth exceeded");
									}
									RunCode(program, addr.value);
									assert(recursion_depth > 0);
									recursion_depth--;
								}break;
								case GTO: {
									ByteCode addr = nextCode();
									if (addr.type != ADDR) throw RuntimeError("Invalid address");
									index = addr.value;
									continue;
								}break;
								case CND: {// ADDR_TRUE ADDR_FALSE REF_BOOL
									ByteCode addrTrue = nextCode();
									ByteCode addrFalse = nextCode();
									ByteCode ref = nextCode();
									if (addrTrue.type != ADDR || addrFalse.type != ADDR) throw RuntimeError("Invalid address");
									bool val;
									if (IsNumeric(ref)) {
										val = MemGetBoolean(ref);
									} else if (IsText(ref)) {
										std::string v = MemGetText(ref);
										val = v != "" && v != "0";
									} else {
										throw RuntimeError("Invalid operation");
									}
									index = val? addrTrue.value : addrFalse.value;
									continue;
								}break;
								case KEY: {// REF_DST REF_OBJ REF_OFFSET
									ByteCode dst = nextCode();
									std::string obj = MemGetText(nextCode());
									ByteCode offset = nextCode();
									size_t pos = size_t(round(MemGetNumeric(offset)));
									if (obj.length() > pos && obj[pos] == '{') {
										int exprStack = 0;
										while (++pos < obj.length()) {
											if (obj[pos] == '{') ++exprStack;
											else if (obj[pos] == '}') {
												if (exprStack == 0) {
													++pos;
													break;
												}
												--exprStack;
											}
										}
									}
									size_t dotPos = obj.find('.', pos);
									if (dotPos == std::string::npos || int(dotPos) >= int(obj.length())-4 || !isalpha_(obj[dotPos+1])) {
										MemSet("", dst);
									} else {
										size_t objPos = obj.find('{', dotPos+1);
										if (objPos == std::string::npos || objPos == dotPos + 1) {
											MemSet("", dst);
										} else {
											std::string key = obj.substr(dotPos+1, objPos-dotPos-1);
											MemSet(key, dst);
											MemSet(double(objPos), offset);
										}
									}
								}break;
								case STR: {
									uint32_t addr = nextCode().rawValue;
									uint32_t len = nextCode().rawValue;
									uint32_t type = nextCode().type;

									// TODO Implement runtime memory bounds checking.
									switch(type) {
										case RAM_VAR_NUMERIC: {
											for (uint32_t i = addr; i < addr + len; i++) {
												if (RamLen() + RecursiveLocalVarsLen() + len * XC_RECURSIVE_MEMORY_PENALTY >= capability.ram) {
													throw RuntimeError("Recursion ran out of memory");
												}
												recursive_localvars.numeric.push_back(ram_numeric[i]);
											}
										} break;
										case RAM_VAR_TEXT: {
											if (RamLen() + RecursiveLocalVarsLen() + len * XC_TEXT_MEMORY_PENALTY * XC_RECURSIVE_MEMORY_PENALTY >= capability.ram) {
												throw RuntimeError("Recursion ran out of memory");
											}
											for (uint32_t i = addr; i < addr + len; i++) {
												recursive_localvars.text.push_back(ram_text[i]);
											}
										} break;
										case RAM_OBJECT: {
											if (RamLen() + RecursiveLocalVarsLen() + len * XC_OBJECT_MEMORY_PENALTY * XC_RECURSIVE_MEMORY_PENALTY >= capability.ram) {
												throw RuntimeError("Recursion ran out of memory");
											}
											for (uint32_t i = addr; i < addr + len; i++) {
												recursive_localvars.objects.push_back(ram_objects[i]);
											}
										} break;
										case RAM_ARRAY_NUMERIC: {
											if (RamLen() + RecursiveLocalVarsLen() + len * XC_ARRAY_NUMERIC_MEMORY_PENALTY * XC_RECURSIVE_MEMORY_PENALTY >= capability.ram) {
												throw RuntimeError("Recursion ran out of memory");
											}
											for (uint32_t i = addr; i < addr + len; i++) {
												recursive_localvars.numeric_arrays.push_back(ram_numeric_arrays[i]);
											}
										} break;
										case RAM_ARRAY_TEXT: {
											if (RamLen() + RecursiveLocalVarsLen() + len * XC_ARRAY_TEXT_MEMORY_PENALTY * XC_RECURSIVE_MEMORY_PENALTY >= capability.ram) {
												throw RuntimeError("Recursion ran out of memory");
											}
											for (uint32_t i = addr; i < addr + len; i++) {
												recursive_localvars.text_arrays.push_back(ram_text_arrays[i]);
											}
										} break;
										default:
										 throw RuntimeError("TODO this type for self recursion");
									}
								}break;
								case RST: {
									uint32_t addr = nextCode().rawValue;
									uint32_t len = nextCode().rawValue;
									uint32_t type = nextCode().type;
									switch(type) {
										case RAM_VAR_NUMERIC: {
											for (uint32_t i = 0; i < len; i++) {
												ram_numeric[addr + i] = recursive_localvars.numeric[recursive_localvars.numeric.size() - len + i];
											}
											recursive_localvars.numeric.resize(recursive_localvars.numeric.size() - len);
										} break;
										case RAM_VAR_TEXT: {
											for (uint32_t i = 0; i < len; i++) {
												ram_text[addr + i] = recursive_localvars.text[recursive_localvars.text.size() - len + i];
											}
											recursive_localvars.text.resize(recursive_localvars.text.size() - len);
										} break;
										case RAM_OBJECT: {
											for (uint32_t i = 0; i < len; i++) {
												ram_objects[addr + i] = recursive_localvars.objects[recursive_localvars.objects.size() - len + i];
											}
											recursive_localvars.objects.resize(recursive_localvars.objects.size() - len);
										} break;
										case RAM_ARRAY_NUMERIC: {
											for (uint32_t i = 0; i < len; i++) {
												ram_numeric_arrays[addr + i] = recursive_localvars.numeric_arrays[recursive_localvars.numeric_arrays.size() - len + i];
											}
											recursive_localvars.numeric_arrays.resize(recursive_localvars.numeric_arrays.size() - len);
										} break;
										case RAM_ARRAY_TEXT: {
											for (uint32_t i = 0; i < len; i++) {
												ram_text_arrays[addr + i] = recursive_localvars.text_arrays[recursive_localvars.text_arrays.size() - len + i];
											}
											recursive_localvars.text_arrays.resize(recursive_localvars.text_arrays.size() - len);
										} break;
										default:
										 throw RuntimeError("TODO this type for self recursion");
									}
								} break;
								case HSH: {// REF_DST REF_SRC
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									if (IsNumeric(dst) && IsText(val)) {
										std::string str = MemGetText(val);
										MemSet((double)((int64_t)(std::hash<std::string>{}(str)) & ((1ll<<53)-1)), dst);
									} else throw RuntimeError("Invalid text operation on non-text values");
								}break;
								case UPP: {// REF_DST REF_SRC
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									if (IsText(dst) && IsText(val)) {
										std::string str = MemGetText(val);
										strtoupper(str);
										MemSet(str, dst);
									} else throw RuntimeError("Invalid text operation on non-text values");
								}break;
								case LCC: {// REF_DST REF_SRC
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									if (IsText(dst) && IsText(val)) {
										std::string str = MemGetText(val);
										strtolower(str);
										MemSet(str, dst);
									} else throw RuntimeError("Invalid text operation on non-text values");
								}break;
								case ISN: {// REF_DST REF_SRC
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									if (IsText(val)) {
										std::string s = MemGetText(val);
										bool isNum = false;
										try {
											size_t pos;
											std::stod(s, &pos);
											isNum = (pos == s.size());
										} catch(...) {}
										MemSet(isNum, dst);
									} else throw RuntimeError("Invalid text operation on non-text values");
								}break;
								case IFF: { // REF_DST REF_BOOL REF_TRUE REF_FALSE
									ByteCode dst = nextCode();
									ByteCode cond = nextCode();
									ByteCode valTrue = nextCode();
									ByteCode valFalse = nextCode();
									if (IsText(valTrue)) {
										std::string str = MemGetBoolean(cond)? MemGetText(valTrue) : MemGetText(valFalse, ARRAY_INDEX_NONE);
										MemSet(str, dst);
									} else if (IsNumeric(valTrue)) {
										double num = MemGetBoolean(cond)? MemGetNumeric(valTrue) : MemGetNumeric(valFalse);
										MemSet(num, dst);
									} else {
										throw RuntimeError("Invalid operation");
									}
								}break;
							}
						}break;
						default: throw RuntimeError("Program Corrupted");
					}
					++index;
				}
			} catch (RuntimeError& err) {
				std::stringstream str;
				str << err.what();
				if (currentFile != "" && currentLine) {
					str << " on bytecode " << index << " in " << currentFile << ":" << currentLine << std::endl;
				}
				throw RuntimeError(str.str());
			} catch (std::exception& err) {
				std::stringstream str;
				str << err.what();
				if (currentFile != "" && currentLine) {
					str << " on bytecode " << index << " in " << currentFile << ":" << currentLine << std::endl;
				}
				throw std::runtime_error(str.str());
			}
		}
		
	}

#endif
