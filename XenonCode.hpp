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
#pragma endregion

namespace XenonCode {

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

#pragma endregion

#pragma region Math helpers

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

	inline static std::string ToString(double val) {
		std::stringstream str;
		str << val;
		return str.str();
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
			AssignmentOperatorGroup, // = += -= *= /= ^= %= &=
			CommaOperator, // ,
			
			MAX_ENUM
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
				}
				if (word == Word::Expression) {
					words.push_back(Word::ExpressionBegin);
					ParseWords(word, words, scope);
					words.push_back(Word::ExpressionEnd);
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
						std::cout << "&& ";
						break;
					case Word::OrOperator:
						std::cout << "|| ";
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
					
					// Suffix operators (INVALID IN EXPRESSIONS, for now at least...)
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
							if (word != Word::CastOperator && word != Word::TrailOperator && after != Word::ExpressionBegin && (next == Word::Funcname || after != Word::Empty)) {
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
				throw ParseError("Invalid expression within function arguments");
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
		
		ParsedLine(const std::string& str, int line_ = 0) : line(line_) {
			ParseWords(str, words, scope);
			
			if (words.size() > 0) {
				
				// Check first word validity for this Scope
				if (scope == 0) { // Global scope
					if (words[0] != Word::Name || (find(begin(globalScopeFirstWords), end(globalScopeFirstWords), words[0].word) == end(globalScopeFirstWords) && find(begin(Implementation::entryPoints), end(Implementation::entryPoints), words[0].word) == end(Implementation::entryPoints))) {
						throw ParseError("Invalid first word", words[0], "in the global scope");
					}
				} else { // Function scope
					if (words[0] != Word::Varname && words[0] != Word::Funcname && words[0] != Word::Name) {
						throw ParseError("Invalid first word", words[0], "in a function scope");
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
						if (find(begin(Implementation::entryPoints), end(Implementation::entryPoints), words[0].word) != end(Implementation::entryPoints)) {
							if (words.size() != 1) throw ParseError("Too many words");
						} else
						
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
							 || (words.size() > 2 && words[2] != Word::CastOperator && words[2] != "=")
							 || (words[2] == "=" && words.size() > 3 && words[3] != Word::Varname)
							) {
								throw ParseError("Second word must be a variable name (starting with $) followed by either a colon and the array type (number or text) or an assignment to another array variable");
							}
							if (words.size() > 3 && words[3] != "number" && words[3] != "text" && words[3] != Word::Varname) {
								throw ParseError("Invalid array type", words[3], "it must be either 'number' or 'text'");
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
						if (words.size() == 1 || words[1] != Word::ExpressionBegin) {
							throw ParseError("A device function call must be followed by a set of parenthesis, optionally containing arguments");
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
							if (words.size() < 5) {
								throw ParseError("Too few words");
							}
							// Array accessor
							if (words[2] == Word::Numeric || words[2] == Word::Varname) {
								if (words[3] == Word::AssignmentOperatorGroup) {
									if (!ParseExpression(words, 4)) {
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
			int scope = 0;
			
			// Parse all lines
			for (std::string lineStr; getline(stream, lineStr); lineNumber++) {
				try {
					auto& line = lines.emplace_back(lineStr, lineNumber);
					scope = line.scope;
				} catch (ParseError& e) {
					std::stringstream err {};
					err << e.what() << " in " << filepath << ":" << lineNumber;
					throw ParseError(err.str());
				}
			}
		}
		
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
	inline static SourceFile GetParsedFile(const std::string& filedir, const std::string& filename) {
		SourceFile src(filedir + "/" + filename);
		// Include other files (and replace the include statement by the lines of the other file, recursively)
		for (unsigned int i = 0; i < src.lines.size(); ++i) {
			if (auto& line = src.lines[i]; line) {
				if (line.scope == 0 && line.words[0] == "include") {
					std::string includeFilename = line.words[1];
					line.words.clear();
					auto includeSrc = GetParsedFile(filedir, includeFilename);
					src.lines.insert(src.lines.begin()+i, includeSrc.lines.begin(), includeSrc.lines.end());
					i += includeSrc.lines.size();
					src.lines.insert(src.lines.begin()+i, Word{Word::FileInfo, filedir + "/" + filename});
					++i;
				}
			}
		}
		return src;
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

	DEF_OP( SET /* [ARRAY_INDEX ifindexnone[REF_NUM]] REF_DST [REF_VALUE]orZero */ ) // Assign a value
	DEF_OP( ADD /* REF_DST REF_A REF_B */ ) // +
	DEF_OP( SUB /* REF_DST REF_A REF_B */ ) // -
	DEF_OP( MUL /* REF_DST REF_A REF_B */ ) // *
	DEF_OP( DIV /* REF_DST REF_A REF_B */ ) // /
	DEF_OP( MOD /* REF_DST REF_A REF_B */ ) // %
	DEF_OP( POW /* REF_DST REF_A REF_B */ ) // ^
	DEF_OP( CCT /* REF_DST REF_A REF_B */ ) // & (concat)
	DEF_OP( AND /* REF_DST REF_A REF_B */ ) // &&
	DEF_OP( ORR /* REF_DST REF_A REF_B */ ) // ||
	DEF_OP( EQQ /* REF_DST REF_A REF_B */ ) // ==
	DEF_OP( NEQ /* REF_DST REF_A REF_B */ ) // !=
	DEF_OP( LST /* REF_DST REF_A REF_B */ ) // <
	DEF_OP( GRT /* REF_DST REF_A REF_B */ ) // >
	DEF_OP( LTE /* REF_DST REF_A REF_B */ ) // >=
	DEF_OP( GTE /* REF_DST REF_A REF_B */ ) // <=
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
	DEF_OP( SIG /* REF_DST REF_NUM */ ) // sign(number)
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
	DEF_OP( SIZ /* REF_DST (REF_ARR | REF_TXT) */ ) // array.size  text.size
	DEF_OP( LAS /* REF_DST (REF_ARR | REF_TXT) */ ) // array.last  text.last
	DEF_OP( MIN /* REF_DST (REF_ARR | (REF_NUM [REF_NUM ...])) */ ) // array.min  min(number, number...)
	DEF_OP( MAX /* REF_DST (REF_ARR | (REF_NUM [REF_NUM ...])) */ ) // array.max  max(number, number...)
	DEF_OP( AVG /* REF_DST (REF_ARR | (REF_NUM [REF_NUM ...])) */ ) // array.avg  avg(number, number...)
	DEF_OP( SUM /* REF_DST (REF_ARR | (REF_NUM [REF_NUM ...])) */ ) // array.sum
	DEF_OP( MED /* REF_DST REF_ARR */ ) // array.med
	DEF_OP( SBS /* REF_DST REF_SRC REF_START REF_LENGTH */ ) // substring(text, start, length)
	DEF_OP( IDX /* REF_DST REF_ARR ARRAY_INDEX ifindexnone[REF_NUM] */ ) // get an indexed value from an array or text
	DEF_OP( JMP /* ADDR */ ) // jump to addr while pushing the stack so that we return here after
	DEF_OP( GTO /* ADDR */ ) // goto addr without pushing the stack
	DEF_OP( CND /* ADDR_TRUE ADDR_FALSE REF_BOOL */ ) // conditional goto (gotoAddrIfTrue, gotoAddrIfFalse, boolExpression)

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
		Var(double value) : type(Numeric), textValue(""), numericValue(value) {}
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
		operator int64_t() const {
			if (type == Numeric) return (int64_t)std::round(numericValue);
			else if (type == Text) return stol(textValue);
			return 0;
		}
		operator uint64_t() const {
			if (type == Numeric) return (uint64_t)std::round(numericValue);
			else if (type == Text) return stoul(textValue);
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
			if (type == Numeric) return std::to_string(numericValue);
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
			
			uint32_t id;
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
	inline static void DeclareEntryPoint(const std::string& entryName) {
		assert(find(globalScopeFirstWords.begin(), globalScopeFirstWords.end(), entryName) == globalScopeFirstWords.end());
		Implementation::entryPoints.emplace_back(entryName);
	}

	// Implementation SHOULD declare device functions
	inline static Device::FunctionInfo& DeclareDeviceFunction(const std::string& prototype, DeviceFunction&& func, uint8_t base = 0) {
		static std::map<uint8_t, uint32_t> nextID {};
		assert(nextID[base] < 65535);
		uint32_t id = (++nextID[base]) | (uint32_t(base) << 16);
		Device::FunctionInfo f {id, prototype};
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
			auto& func = DeclareDeviceFunction(name + "::" + prototype, [method](Computer* computer, const std::vector<Var>& args) -> XenonCode::Var {
				if (args.size() > 0) {
					return method(computer, args[0], std::vector<Var>(args.begin()+1, args.end()));
				} else {
					throw XenonCode::RuntimeError("Invalid object member arguments");
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
		std::string type; // function, if, elseif, else, foreach, while, repeat
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
		if (func == "substring") {returnType = RAM_VAR_TEXT; return SBS;}
		if (func == "text") {returnType = RAM_VAR_TEXT; return TXT;}
		if (func == "sort") {returnType = VOID; return ASC;}
		if (func == "sortd") {returnType = VOID; return DSC;}
		if (func == "system.output") {returnType = VOID; return OUT;}
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

	inline static bool IsNumeric(ByteCode v) {
		switch (v.type) {
			case ROM_CONST_NUMERIC:
			case STORAGE_VAR_NUMERIC:
			case RAM_VAR_NUMERIC:
			return true;
		}
		return false;
	}

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
		std::vector<std::string> sourceFiles {};
		/* Function names:
			system.init
			system.tick
			system.input (not present in functionRefs)
			system.timer (not present in functionRefs)
			system.* (implementation-defined entry points)
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
				if (name != "system.timer" && name != "system.input" && functionRefs.contains(name)) {
					throw CompileError("Function " + name + " is already defined");
				}
				currentFunctionName = name;
				currentFunctionAddr = addr();
			};
			auto closeCurrentFunction = [&](){
				if (currentFunctionName != "") {
					write(RETURN);
					if (currentFunctionName == "system.timer") {
						assert(currentTimerInterval);
						timers.emplace_back(currentTimerInterval, currentFunctionAddr);
						currentTimerInterval = 0;
						userVars["system.timer"].clear();
					} else if (currentFunctionName == "system.input") {
						inputs[currentInputPort].addr = currentFunctionAddr;
						currentInputPort = 0;
						userVars["system.input"].clear();
					} else {
						functionRefs.emplace(currentFunctionName, currentFunctionAddr);
					}
					currentFunctionName = "";
					currentFunctionAddr = 0;
				}
				currentStackId = 0;
			};
			// If it's a user-declared function and it has a return type defined, returns the return var ref of that function, otherwise returns VOID
			auto compileFunctionCall = [&](Word func, const std::vector<ByteCode>& args, bool getReturn, bool isTrailingFunction = false) -> ByteCode {
				std::string funcName = func;
				if (func == Word::Funcname) {
					if (!functionRefs.contains(funcName)) {
						throw CompileError("Function", func, "is not defined");
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
								default: throw CompileError("Invalid arguments");
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
			
			// Stack helpers
			auto addPointer = [&](std::string reg) -> uint32_t& {
				if (reg == "") reg = std::to_string(stack.back().pointers.size()+1);
				return stack.back().pointers[reg];
			};
			auto getPointer = [&](const std::string& reg) -> ByteCode {
				assert(stack.size() > 0);
				if (stack.back().pointers.contains(reg)) {
					return rom_program[stack.back().pointers.at(reg)];
				}
			};
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
						return Word{double(long(std::round(double(word1))) % long(std::round(double(word2))))};
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
						if (words[startIndex+1] == Word::ExpressionBegin) {
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
						ref1 = compileFunctionCall(word1, args, true);
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
						} else { // trailing function call, in an expression
						
							// validate(words[opIndex+2] == Word::ExpressionBegin);
							// std::vector<ByteCode> args {};
							// args.push_back(ref1);
							// int argBegin = opIndex+3;
							// while (argBegin <= endIndex) {
							// 	int argEnd = GetArgEnd(words, argBegin, endIndex);
							// 	if (argEnd == -1) {
							// 		++argBegin;
							// 		break;
							// 	}
							// 	args.push_back(compileExpression(words, argBegin, argEnd));
							// 	argBegin = argEnd + 2;
							// 	if (argEnd+1 > endIndex || words[argEnd+1] != Word::CommaOperator) {
							// 		break;
							// 	}
							// }
							// return compileFunctionCall(operand, args, true);
							
							throw CompileError("A trailing function call is invalid within an expression"); // because as per the spec, a trailing function call WILL modify the value of the variable that it's called on, defined by what it returns
							
						}
					} else if (operand == Word::Numeric || operand == Word::Varname) {
						validate(IsArray(ref1) || IsText(ref1));
						ByteCode tmp = declareVar("", GetRamVarType(ref1.type));
						write(IDX);
						write(tmp);
						write(ref1);
						if (operand == Word::Varname) {
							ByteCode ref2 = getVar(operand);
							validate(IsNumeric(ref2));
							write({ARRAY_INDEX, ARRAY_INDEX_NONE});
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
						validate(IsText(ref1) && IsText(ref2));
						ByteCode tmp = declareTmpText();
						write(CCT);
						write(tmp);
						write(ref1);
						write(ref2);
						write(VOID);
						return tmp;
					} else if (op == Word::MulOperatorGroup) {
						validate(IsNumeric(ref1) && IsNumeric(ref2));
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
						validate(IsNumeric(ref1) && IsNumeric(ref2));
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
						validate(IsNumeric(ref1) && IsNumeric(ref2));
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
						if (IsNumeric(ref1) && IsNumeric(ref2)) {
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
						} else if (IsText(ref1) && IsText(ref2)) {
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
						} else {
							validate(false);
							return VOID;
						}
					} else if (op == Word::AndOperator) {
						validate(IsNumeric(ref1) && IsNumeric(ref2));
						ByteCode tmp = declareTmpNumeric();
						write(AND);
						write(tmp);
						write(ref1);
						write(ref2);
						write(VOID);
						return tmp;
					} else if (op == Word::OrOperator) {
						validate(IsNumeric(ref1) && IsNumeric(ref2));
						ByteCode tmp = declareTmpNumeric();
						write(ORR);
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
											
											ByteCode var;
											CODE_TYPE retType;
											rom_vars_init.emplace_back(DEV);
											std::string funcName = value.word;
											
											if (!Device::deviceFunctionsByName.contains(funcName)) {
												throw CompileError("Function", funcName, "does not exist");
											}
											auto& function = Device::deviceFunctionsByName.at(funcName);
											rom_vars_init.emplace_back(DEVICE_FUNCTION_INDEX, function.id);
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
											
											var = declareVar(name, retType);
											rom_vars_init.emplace_back(var);
											
											if (Word nextWord = readWord(); nextWord) {
												validate(nextWord == Word::ExpressionBegin);
												
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
														rom_vars_init.emplace_back(declareVar(name+".init."+std::to_string(argIndex), ROM_CONST_NUMERIC, arg));
													} else if (arg == Word::Text) {
														rom_vars_init.emplace_back(declareVar(name+".init."+std::to_string(argIndex), ROM_CONST_TEXT, arg));
													} else {
														validate(false);
													}
													nextWordIndex = argEnd+1;
												}
											}
											
											rom_vars_init.emplace_back(VOID);
											
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
								else if (firstWord == "function") {
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
									openFunction("system." + firstWord.word);
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
											validate(operation == Word::AssignmentOperatorGroup);
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
													validate(IsNumeric(idx));
													write({ARRAY_INDEX, ARRAY_INDEX_NONE});
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
												validate(IsNumeric(idx));
												write({ARRAY_INDEX, ARRAY_INDEX_NONE});
												write(idx);
											} else /*numeric*/ {
												write({ARRAY_INDEX, uint32_t(std::round(double(operand)))});
											}
											write(dst);
											write(ref);
											write(VOID);
										} else if (operand == Word::Funcname || operand == Word::Name) {
											// Trailing function call statement
											std::vector<ByteCode> args {};
											args.push_back(dst);
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
											compileFunctionCall(operand, args, false, true);
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
									validate(IsArray(arr));
									readWord(Word::ExpressionBegin);
									Word item = readWord(Word::Varname);
									Word index = Word::Empty;
									Word next = readWord();
									if (next == Word::CommaOperator) {
										index = readWord(Word::Varname);
										validate(readWord() == Word::ExpressionEnd);
									} else {
										validate(next == Word::ExpressionEnd);
									}
									validate(nextWordIndex == (int)line.words.size());
									ByteCode itemRef, indexRef;
									switch (arr.type) {
										case STORAGE_ARRAY_NUMERIC:
										case RAM_ARRAY_NUMERIC:
											itemRef = declareVar(item, RAM_VAR_NUMERIC);
											break;
										case STORAGE_VAR_TEXT:
										case STORAGE_ARRAY_TEXT:
										case RAM_VAR_TEXT:
										case RAM_ARRAY_TEXT:
											itemRef = declareVar(item, RAM_VAR_TEXT);
											break;
										default: validate(false);
									}
									if (index) {
										indexRef = declareVar(index, RAM_VAR_NUMERIC);
									} else {
										indexRef = declareTmpNumeric();
									}
									
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
								// next
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
									validate(currentFunctionName != "");
									ByteCode ret = getReturnVar(currentFunctionName);
									ByteCode val = compileExpression(line.words, nextWordIndex, -1);
									write(SET);
									write(ret);
									write(val);
									write(VOID);
								}
								else if (Device::deviceFunctionsByName.contains(firstWord.word)) {
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
									compileFunctionCall(firstWord, args, false);
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
				s << varsInitSize << ' ' << programSize << ' ' << storageRefs.size() << ' ' << functionRefs.size() << ' ' << timers.size() << ' ' << inputs.size() << ' ' << sourceFiles.size() << '\n';
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
				s >> varsInitSize >> programSize >> storageRefsSize >> functionRefsSize >> timersSize >> inputsSize >> sourceFilesSize;
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
				sourceFiles.reserve(sourceFilesSize);

				// Read sourceFile references for debug
				for (size_t i = 0; i < sourceFilesSize; ++i) {
					std::string f;
					s >> f;
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

	inline static double GetCurrentTimestamp() {
		std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<double, std::milli>> time = std::chrono::high_resolution_clock::now();
		return time.time_since_epoch().count() * 0.001;
	}

	class Computer {
		// Data
		std::string directory;
		Assembly* assembly = nullptr;
		std::vector<double> ram_numeric {};
		std::vector<std::string> ram_text {};
		std::vector<std::vector<double>> ram_numeric_arrays {};
		std::vector<std::vector<std::string>> ram_text_arrays {};
		std::vector<uint64_t> ram_objects {};
		std::unordered_map<std::string, std::vector<std::string>> storageCache {};
		
		// State
		bool storageDirty = false;
		uint64_t currentCycleInstructions = 0;
		std::vector<double> timersLastRun {};
		
	public:
		struct Capability {
			uint32_t ipc = 1'000'000; // instructions per cycle
			uint32_t ram = 16'000'000; // number of ram variables * their memory penalty
			uint32_t storage = 256; // number of storage references
			uint32_t io = 256; // Number of input/output ports
		} capability;
		
		Computer() {}
		~Computer() {
			if (assembly) {
				delete assembly;
			}
		}
		
		bool LoadProgram(const std::string& directory_) {
			directory = directory_;
			
			{// Load Assembly
				if (assembly) delete assembly;
				std::ifstream file{directory + "/" + XC_PROGRAM_EXECUTABLE};
				assembly = new Assembly(file);
			}
			
			{// Check Capabilities
				// Do we have enough RAM to run this program?
				if (capability.ram < assembly->ram_numericVariables
								+ assembly->ram_textVariables * XC_TEXT_MEMORY_PENALTY
								+ assembly->ram_numericArrays * XC_ARRAY_NUMERIC_MEMORY_PENALTY
								+ assembly->ram_textArrays * XC_ARRAY_TEXT_MEMORY_PENALTY
								+ assembly->ram_objectReferences * XC_OBJECT_MEMORY_PENALTY
				) {
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
			
			// Prepare storage cache
			storageCache.clear();
			for (const auto& name : assembly->storageRefs) {
				auto& storage = storageCache[name];
				storage.clear();
				std::ifstream file{directory + "/storage/" + name};
				char value[XC_MAX_TEXT_LENGTH+1];
				while (file.getline(value, XC_MAX_TEXT_LENGTH, '\0')) {
					storage.emplace_back(std::string(value));
				}
			}
			
			// Prepare timers
			timersLastRun.clear();
			timersLastRun.resize(assembly->timers.size());
			
			return true;
		}
		
		void SaveStorage() {
			if (storageDirty) {
				std::string storagePath = directory + "/storage/";
				std::filesystem::create_directories(storagePath);
				for (const auto& name : assembly->storageRefs) {
					const auto& storage = storageCache[name];
					std::ofstream file{storagePath + name};
					for (const std::string& value : storage) {
						file << value << '\0';
					}
				}
				storageDirty = false;
			}
		}
		
		void ClearStorage() {
			std::string storagePath = directory + "/storage/";
			std::filesystem::remove_all(storagePath);
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
				default: throw RuntimeError("Invalid memory reference");
			}
		}
		void MemSet(const std::string& value, ByteCode dst, uint32_t arrIndex = ARRAY_INDEX_NONE) {
			if (value.length() > XC_MAX_TEXT_LENGTH) {
				throw RuntimeError("Text too large");
			}
			switch (dst.type) {
				case STORAGE_VAR_TEXT:
				case STORAGE_ARRAY_TEXT: {
					StorageSet(value, dst, arrIndex);
				}break;
				case RAM_VAR_TEXT: {
					if (dst.value >= ram_text.size()) {
						throw RuntimeError("Invalid memory reference");
					}
					ram_text[dst.value] = value;
				}break;
				case RAM_ARRAY_TEXT: {
					auto& arr = GetTextArray(dst);
					if (arrIndex == ARRAY_INDEX_NONE || arrIndex >= arr.size()) {
						throw RuntimeError("Invalid array indexing");
					}
					arr[arrIndex] = value;
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
				default: throw RuntimeError("Invalid memory reference");
			}
		}
		const std::string& MemGetText(ByteCode ref, uint32_t arrIndex = ARRAY_INDEX_NONE) {
			switch (ref.type) {
				case ROM_CONST_TEXT: {
					if (ref.value >= assembly->rom_textConstants.size()) {
						throw RuntimeError("Invalid memory reference");
					}
					return assembly->rom_textConstants[ref.value];
				}break;
				case STORAGE_VAR_TEXT:
				case STORAGE_ARRAY_TEXT: {
					return StorageGetText(ref, arrIndex);
				}break;
				case RAM_VAR_TEXT: {
					if (ref.value >= ram_text.size()) {
						throw RuntimeError("Invalid memory reference");
					}
					return ram_text[ref.value];
				}break;
				case RAM_ARRAY_TEXT: {
					auto& arr = GetTextArray(ref);
					if (arrIndex == ARRAY_INDEX_NONE || arrIndex >= arr.size()) {
						throw RuntimeError("Invalid array indexing");
					}
					return arr[arrIndex];
				}break;
				case VOID: {
					static const std::string empty = "";
					return empty;
				}
				default: throw RuntimeError("Invalid memory reference");
			}
		}
		
		Var MemGetObject(ByteCode ref) {
			if (ref.value >= ram_objects.size()) {
				throw RuntimeError("Invalid memory reference");
			}
			return {Var::Type(Var::Object | (ref & (RAM_OBJECT-1))), ram_objects[ref.value]};
		}
		
		void RunCode(const std::vector<ByteCode>& program, uint32_t index = 0);
		
	public:
		bool HasTick() {
			return assembly && assembly->functionRefs.contains("system.tick");
		}
		bool HasEntryPoint(const std::string& name) {
			return assembly && assembly->functionRefs.contains("system." + name);
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
				RunCode(assembly->rom_vars_init);
				if (assembly->functionRefs.contains("system.init")) {
					RunCode(assembly->rom_program, assembly->functionRefs["system.init"]);
				}
				return true;
			}
			return false;
		}
		
		void RunCycle() {
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
					} else if (IsObject(dst) && arg.type == Var::Object) {
						MemSetObject(arg.addrValue, dst); // Reference assignment for objects
					} else {
						throw RuntimeError("Input argument type mismatch");
					}
				}
				
				RunCode(assembly->rom_program, input.addr);
			}
		}
		
		void RunEntryPoint(const std::string& name) {
			if (HasEntryPoint(name)) {
				RunCode(assembly->rom_program, assembly->functionRefs.at("system." + name));
			}
		}
		
	};

#pragma endregion

}

#ifdef XENONCODE_IMPLEMENTATION

	namespace XenonCode {
		
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
			if (program.size() <= index) return;
			
			// Find current file and line for debug
			std::string currentFile = "";
			uint32_t currentLine = 0;
			for (int32_t tmpIndex = index; tmpIndex >= 0; --tmpIndex) {
				if (currentLine == 0 && program[tmpIndex].type == LINENUMBER) {
					currentLine = program[tmpIndex].value;
				} else if (currentFile == "" && program[tmpIndex].type == SOURCEFILE) {
					if (program[tmpIndex].value < assembly->sourceFiles.size()) {
						currentFile = assembly->sourceFiles[program[tmpIndex].value];
					}
				} else if (currentLine != 0 && currentFile != "") {
					break;
				}
			}
			
			// IPC
			auto ipcCheck = [this](int ipcPenalty = 1){
				currentCycleInstructions += ipcPenalty;
				if (currentCycleInstructions > capability.ipc) {
					throw RuntimeError("Maximum IPC exceeded");
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
								case SET: {// [ARRAY_INDEX ifindexnone[REF_NUM]] REF_DST [REF_VALUE]orZero
									ByteCode dst = nextCode();
									uint32_t arr_index = ARRAY_INDEX_NONE;
									if (dst.type == ARRAY_INDEX) {
										arr_index = dst.value;
										if (arr_index == ARRAY_INDEX_NONE) {
											arr_index = uint32_t(MemGetNumeric(nextCode()));
										}
										dst = nextCode();
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
									if (IsNumeric(dst) && IsNumeric(a) && IsNumeric(b)) {
										MemSet(MemGetNumeric(a) + MemGetNumeric(b), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case SUB: {
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									if (IsNumeric(dst) && IsNumeric(a) && IsNumeric(b)) {
										MemSet(MemGetNumeric(a) - MemGetNumeric(b), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case MUL: {
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									if (IsNumeric(dst) && IsNumeric(a) && IsNumeric(b)) {
										MemSet(MemGetNumeric(a) * MemGetNumeric(b), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case DIV: {
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									if (IsNumeric(dst) && IsNumeric(a) && IsNumeric(b)) {
										double operand = MemGetNumeric(b);
										if (operand == 0) throw RuntimeError("Division by zero");
										MemSet(MemGetNumeric(a) / operand, dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case MOD: {
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									if (IsNumeric(dst) && IsNumeric(a) && IsNumeric(b)) {
										double operand = MemGetNumeric(b);
										if (operand == 0) throw RuntimeError("Division by zero");
										MemSet(double(int64_t(std::round(MemGetNumeric(a))) % int64_t(std::round(operand))), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case POW: {
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									if (IsNumeric(dst) && IsNumeric(a) && IsNumeric(b)) {
										MemSet(std::pow(MemGetNumeric(a), MemGetNumeric(b)), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case CCT: {
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									if (IsText(dst) && IsText(a) && IsText(b)) {
										MemSet(MemGetText(a) + MemGetText(b), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case AND: {
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									if (IsNumeric(dst) && IsNumeric(a) && IsNumeric(b)) {
										MemSet(double(MemGetNumeric(a) && MemGetNumeric(b)), dst);
									} else if (IsText(dst) && IsText(a) && IsText(b)) {
										MemSet(double(MemGetText(a) != "" && MemGetText(b) != ""), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case ORR: {
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									if (IsNumeric(dst) && IsNumeric(a) && IsNumeric(b)) {
										MemSet(double(MemGetNumeric(a) || MemGetNumeric(b)), dst);
									} else if (IsText(dst) && IsText(a) && IsText(b)) {
										MemSet(double(MemGetText(a) != "" && MemGetText(b) != ""), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case EQQ: {
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									if (IsNumeric(dst) && IsNumeric(a) && IsNumeric(b)) {
										MemSet(double(MemGetNumeric(a) == MemGetNumeric(b)), dst);
									} else if (IsText(dst) && IsText(a) && IsText(b)) {
										MemSet(double(MemGetText(a) == MemGetText(b)), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case NEQ: {
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									if (IsNumeric(dst) && IsNumeric(a) && IsNumeric(b)) {
										MemSet(double(MemGetNumeric(a) != MemGetNumeric(b)), dst);
									} else if (IsText(dst) && IsText(a) && IsText(b)) {
										MemSet(double(MemGetText(a) != MemGetText(b)), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case LST: {
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									if (IsNumeric(dst) && IsNumeric(a) && IsNumeric(b)) {
										MemSet(MemGetNumeric(a) < MemGetNumeric(b), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case GRT: {
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									if (IsNumeric(dst) && IsNumeric(a) && IsNumeric(b)) {
										MemSet(MemGetNumeric(a) > MemGetNumeric(b), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case LTE: {
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									if (IsNumeric(dst) && IsNumeric(a) && IsNumeric(b)) {
										MemSet(MemGetNumeric(a) <= MemGetNumeric(b), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case GTE: {
									ByteCode dst = nextCode();
									ByteCode a = nextCode();
									ByteCode b = nextCode();
									if (IsNumeric(dst) && IsNumeric(a) && IsNumeric(b)) {
										MemSet(MemGetNumeric(a) >= MemGetNumeric(b), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case INC: {// REF_NUM
									ByteCode ref = nextCode();
									if (IsNumeric(ref)) {
										MemSet(std::round(MemGetNumeric(ref)) + 1.0, ref);
									} else throw RuntimeError("Invalid operation");
								}break;
								case DEC: {
									ByteCode ref = nextCode();
									if (IsNumeric(ref)) {
										MemSet(std::round(MemGetNumeric(ref)) - 1.0, ref);
									} else throw RuntimeError("Invalid operation");
								}break;
								case NOT: {// REF_DST REF_VAL
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									if (IsNumeric(dst) && IsNumeric(val)) {
										MemSet(double(MemGetNumeric(val) == 0.0), dst);
									} else if (IsText(dst) && IsText(val)) {
										MemSet(double(MemGetText(val) == ""), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case FLR: {// REF_DST REF_NUM
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									if (IsNumeric(dst) && IsNumeric(val)) {
										MemSet(std::floor(MemGetNumeric(val)), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case CIL: {
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									if (IsNumeric(dst) && IsNumeric(val)) {
										MemSet(std::ceil(MemGetNumeric(val)), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case RND: {
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									if (IsNumeric(dst) && IsNumeric(val)) {
										MemSet(std::round(MemGetNumeric(val)), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case SIN: {
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									if (IsNumeric(dst) && IsNumeric(val)) {
										MemSet(std::sin(MemGetNumeric(val)), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case COS: {
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									if (IsNumeric(dst) && IsNumeric(val)) {
										MemSet(std::cos(MemGetNumeric(val)), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case TAN: {
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									if (IsNumeric(dst) && IsNumeric(val)) {
										MemSet(std::tan(MemGetNumeric(val)), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case ASI: {
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									if (IsNumeric(dst) && IsNumeric(val)) {
										MemSet(std::asin(MemGetNumeric(val)), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case ACO: {
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									if (IsNumeric(dst) && IsNumeric(val)) {
										MemSet(std::acos(MemGetNumeric(val)), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case ATA: {
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									if (IsNumeric(dst) && IsNumeric(val)) {
										MemSet(std::atan(MemGetNumeric(val)), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case ABS: {
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									if (IsNumeric(dst) && IsNumeric(val)) {
										MemSet(std::abs(MemGetNumeric(val)), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case FRA: {
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									if (IsNumeric(dst) && IsNumeric(val)) {
										double intpart;
										MemSet(std::modf(MemGetNumeric(val), &intpart), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case SQR: {
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									if (IsNumeric(dst) && IsNumeric(val)) {
										MemSet(std::sqrt(MemGetNumeric(val)), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case SIG: {
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									if (IsNumeric(dst) && IsNumeric(val)) {
										double num = MemGetNumeric(val);
										if (num > 0.0) num = 1.0;
										else if (num < 0.0) num = -1.0;
										MemSet(num, dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case LOG: {// REF_DST REF_NUM REF_BASE
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									ByteCode base = nextCode();
									if (IsNumeric(dst) && IsNumeric(val) && IsNumeric(base)) {
										double b = MemGetNumeric(base);
										if (b == 0) b = 10.0;
										MemSet(std::log(MemGetNumeric(val)) / std::log(b), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case CLP: {// REF_DST REF_NUM REF_MIN REF_MAX
									ByteCode dst = nextCode();
									ByteCode val = nextCode();
									ByteCode min = nextCode();
									ByteCode max = nextCode();
									if (IsNumeric(dst) && IsNumeric(val) && IsNumeric(min) && IsNumeric(max)) {
										MemSet(std::clamp(MemGetNumeric(val), MemGetNumeric(min), MemGetNumeric(max)), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case STP: {// REF_DST REF_T1 REF_T2 REF_NUM
									ByteCode dst = nextCode();
									ByteCode t1 = nextCode();
									ByteCode t2 = nextCode();
									ByteCode val = nextCode();
									if (IsNumeric(dst) && IsNumeric(t1) && IsNumeric(t2) && (val.type == VOID || IsNumeric(val))) {
										if (val.type == VOID) {
											val = t2;
											MemSet(step(MemGetNumeric(t1), MemGetNumeric(val)), dst);
										} else {
											MemSet(step(MemGetNumeric(t1), MemGetNumeric(t2), MemGetNumeric(val)), dst);
										}
									} else throw RuntimeError("Invalid operation");
								}break;
								case SMT: {
									ByteCode dst = nextCode();
									ByteCode t1 = nextCode();
									ByteCode t2 = nextCode();
									ByteCode val = nextCode();
									if (IsNumeric(dst) && IsNumeric(val) && IsNumeric(t1) && IsNumeric(t2)) {
										MemSet(smoothstep(MemGetNumeric(t1), MemGetNumeric(t2), MemGetNumeric(val)), dst);
									} else throw RuntimeError("Invalid operation");
								}break;
								case LRP: {
									ByteCode dst = nextCode();
									ByteCode t1 = nextCode();
									ByteCode t2 = nextCode();
									ByteCode val = nextCode();
									if (IsNumeric(dst) && IsNumeric(val) && IsNumeric(t1) && IsNumeric(t2)) {
										MemSet(std::lerp(MemGetNumeric(t1), MemGetNumeric(t2), MemGetNumeric(val)), dst);
									} else throw RuntimeError("Invalid operation");
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
											std::string txt = MemGetText(src);
											for (ByteCode c : args) {
												size_t p1 = txt.find('{');
												size_t p2 = txt.find('}');
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
													if (!IsNumeric(c)) throw RuntimeError("Invalid operation");
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
													array.push_back(std::to_string(MemGetNumeric(c)));
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
											array.pop_back();
											storageDirty = true;
										}break;
										case RAM_ARRAY_NUMERIC:{
											auto& array = GetNumericArray(arr);
											array.pop_back();
										}break;
										case RAM_ARRAY_TEXT:{
											auto& array = GetTextArray(arr);
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
											for (const auto& val : values) array.push_back(std::to_string(val));
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
											for (const auto& val : values) array.push_back(std::to_string(val));
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
								case INS: {// REF_ARR REF_INDEX REF_VALUE [REF_VALUE ...]
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
													values.push_back(std::to_string(MemGetNumeric(c)));
												} else {
													values.push_back(MemGetText(c));
												}
											}
											if (arr_index >= (int)array.size()) throw RuntimeError("Invalid array index");
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
											if (arr_index >= (int)array.size()) throw RuntimeError("Invalid array index");
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
											if (arr_index >= (int)array.size()) throw RuntimeError("Invalid array index");
											array.insert(array.begin()+arr_index, values.begin(), values.end());
										}break;
									}
								}break;
								case DEL: {// REF_ARR REF_INDEX [REF_INDEX_END]
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
											if (index2 > (int)array.size()) throw RuntimeError("Invalid array index");
											if (index2 == arr_index+1) {
												array.erase(array.begin()+arr_index);
											} else {
												array.erase(array.begin()+arr_index, array.begin()+index2);
											}
											storageDirty = true;
										}break;
										case RAM_ARRAY_NUMERIC:{
											auto& array = GetNumericArray(arr);
											if (index2 > (int)array.size()) throw RuntimeError("Invalid array index");
											if (index2 == arr_index+1) {
												array.erase(array.begin()+arr_index);
											} else {
												array.erase(array.begin()+arr_index, array.begin()+index2);
											}
										}break;
										case RAM_ARRAY_TEXT:{
											auto& array = GetTextArray(arr);
											if (index2 > (int)array.size()) throw RuntimeError("Invalid array index");
											if (index2 == arr_index+1) {
												array.erase(array.begin()+arr_index);
											} else {
												array.erase(array.begin()+arr_index, array.begin()+index2);
											}
										}break;
									}
								}break;
								case FLL: {// REF_ARR REF_QTY REF_VAL
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
											array.resize(count, std::to_string(MemGetNumeric(val)));
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
								case SIZ: {// REF_DST (REF_ARR | REF_TXT)
									ByteCode dst = nextCode();
									ByteCode ref = nextCode();
									if (!IsNumeric(dst)) throw RuntimeError("Invalid operation");
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
										MemSet(MemGetText(ref).length(), dst);
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
												std::string last = array.back();
												MemSet(last==""? 0.0 : stod(last), dst);
											}break;
											case STORAGE_ARRAY_TEXT:{
												auto& array = GetStorage(ref);
												MemSet(array.back(), dst);
											}break;
											case RAM_ARRAY_NUMERIC:{
												auto& array = GetNumericArray(ref);
												MemSet(array.back(), dst);
											}break;
											case RAM_ARRAY_TEXT:{
												auto& array = GetTextArray(ref);
												MemSet(array.back(), dst);
											}break;
										}
									} else {
										MemSet(std::string(1, MemGetText(ref).back()), dst);
									}
								}break;
								case MIN: {// REF_DST (REF_ARR | (REF_NUM [REF_NUM ...]))
									ByteCode dst = nextCode();
									std::vector<ByteCode> args {};
									for (ByteCode c; (c = nextCode()).type != VOID;) {
										args.emplace_back(c);
									}
									if (!IsNumeric(dst)) throw RuntimeError("Invalid operation");
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
												throw RuntimeError("Invalid operation");
											}break;
										}
									} else {
										ipcCheck(args.size());
										for (const ByteCode& c : args) {
											if (!IsNumeric(c)) throw RuntimeError("Invalid operation");
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
									if (!IsNumeric(dst)) throw RuntimeError("Invalid operation");
									if (args.size() == 0) throw RuntimeError("Not enough arguments");
									ByteCode arr = args[0];
									double max = std::numeric_limits<double>::min();
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
											if (!IsNumeric(c)) throw RuntimeError("Invalid operation");
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
									if (!IsNumeric(dst)) throw RuntimeError("Invalid operation");
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
											if (!IsNumeric(c)) throw RuntimeError("Invalid operation");
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
									if (!IsNumeric(dst)) throw RuntimeError("Invalid operation");
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
											if (!IsNumeric(c)) throw RuntimeError("Invalid operation");
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
									if (!IsNumeric(dst)) throw RuntimeError("Invalid operation");
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
									int len = b.type != VOID? (int)std::round(MemGetNumeric(b)) : int(text.length()-start);
									if (!IsText(dst)) throw RuntimeError("Invalid operation");
									MemSet(text.substr(start, len), dst);
								}break;
								case IDX: {// REF_DST REF_ARR ARRAY_INDEX ifindexnone[REF_NUM]
									ByteCode dst = nextCode();
									ByteCode arr = nextCode();
									ByteCode idx = nextCode();
									if (idx.type != ARRAY_INDEX) throw RuntimeError("Invalid array index type");
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
										case RAM_ARRAY_TEXT:
											MemSet(MemGetText(arr, arr_index), dst);
										break;
										default: throw RuntimeError("Not an array");
									}
								}break;
								case JMP: {// ADDR
									ByteCode addr = nextCode();
									if (addr.type != ADDR) throw RuntimeError("Invalid address");
									RunCode(program, addr.value);
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
										val = MemGetNumeric(ref);
									} else if (IsText(ref)) {
										val = MemGetText(ref) != "";
									} else {
										throw RuntimeError("Invalid operation");
									}
									index = val? addrTrue.value : addrFalse.value;
									continue;
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
