#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <regex>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <stack>
#include <cassert>
#include <algorithm>
#include <cmath>

using namespace std;

namespace XenonCode {

const int VERSION_MAJOR = 0;
const int VERSION_MINOR = 0;
const int VERSION_PATCH = 0;

const size_t MAX_TEXT_LENGTH = 256;

#pragma region Parser

struct ParseError : public runtime_error {
	using runtime_error::runtime_error;
	ParseError(const string& pre, const string& word) : runtime_error(pre + " '" + word + "'") {}
	ParseError(const string& pre, const string& word, const string& post) : runtime_error(pre + " '" + word + "' " + post) {}
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

string ToString(double val) {
	stringstream str;
	str << val;
	return str.str();
}

static inline const int WORD_ENUM_FINAL_TYPE_START = 11;
static inline const int WORD_ENUM_OPERATOR_START = 101;

struct Word {
	string word = "";
	
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
		CastOperator, // :
		ConcatOperator, // & (text concatenation operator)
		SuffixOperatorGroup, // ++ --
		NotOperator, // !
		MulOperatorGroup, // ^ % * /
		AddOperatorGroup, // + -
		CompareOperatorGroup, // < <= > >=
		EqualityOperatorGroup, // == != <>
		AndOperator, // && and
		OrOperator, // || or
		AssignmentOperatorGroup, // = += -= *= /= ^= %=
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
	
	Word(Type type, string word = "") : word(word), type(type) {
		// Automatically assign a string word when constructed only via the type
		if (word == "") {
			switch (type) {
				case Empty:{
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
	explicit Word(const string& value) : word(value), type(Text) {}
	explicit Word(double value) : word(ToString(value)), type(Numeric) {}
	Word(const Word& other) = default;
	Word(Word&& other) = default;
	Word& operator= (const Word& other) = default;
	Word& operator= (Word&& other) = default;
	Word& operator= (const string& value) {
		word = value;
		type = Text;
		return *this;
	}
	Word& operator= (double value) {
		word = ToString(value);
		type = Numeric;
		return *this;
	}
	
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

// Parses one or more words from given string (including expressions done recursively), and assign final types to words. Sets resulting words in given words ref. This will NOT add expressions according to operator precedence. 
void ParseWords(const string& str, vector<Word>& words, int& scope) {
	istringstream line(str);
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
				else if (word == ":")
					word.type = Word::CastOperator;
				else if (word == "++" || word == "--")
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
				else if (word == "=" || word == "+=" || word == "-=" || word == "*=" || word == "/=" || word == "^=" || word == "%=")
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
int GetExpressionEnd(const vector<Word>& words, int begin, int end = -1) {
	int stack = 0;
	if (end == -1) end = words.size()-1;
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

// From a given index of a closing parenthesis, returns the index of the corresponsing opening parenthesis, or -1 if not found. This function will include a leading Name if there is one (for a function call)
int GetExpressionBegin(const vector<Word>& words, int end) {
	int stack = 0;
	while(--end >= 0) {
		if (words[end] == Word::ExpressionBegin) {
			if (stack == 0) {
				if (end > 0 && words[end-1] == Word::Name) {
					return end-1;
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

// Prints out the interpreted code after it's fully parsed (after ParseLine, or after a ParseExpression)
void DebugWords(vector<Word>& words, int startIndex = 0, bool verbose = false) {
	for (int i = startIndex; i < words.size(); ++i) {
		const auto& word = words[i];
		if (verbose) {
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
				case Word::ConcatOperator:
					cout << "ConcatOperator{" << word << "} ";
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
				case Word::Void:
					cout << "void ";
					break;
					
				case Word::FileInfo:
					cout << "FileInfo{" << word << "} ";
					break;
					
				default: assert(!"Parse Error");
			}
		} else {
			switch (word.type) {
				case Word::Numeric:
					cout << word << " ";
					break;
				case Word::Text:
					cout << "\"" << word << "\" ";
					break;
				case Word::Varname:
					cout << "$" << word << " ";
					break;
				case Word::Funcname:
					cout << "@" << word << " ";
					break;
				case Word::ExpressionBegin:
					cout << "( ";
					break;
				case Word::ExpressionEnd:
					cout << ") ";
					break;
				case Word::Name:
					cout << word << " ";
					break;
				case Word::TrailOperator:
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
					cout << word << " ";
					break;
				case Word::AndOperator:
					cout << "&& ";
					break;
				case Word::OrOperator:
					cout << "|| ";
					break;
				case Word::Void:
					break;
					
				case Word::FileInfo:
					cout << "// <" << word << "> ";
					break;
					
				default: assert(!"Parse Error");
			}
		}
	}
}

// Parse an expression following an assignement or function call arguments excluding surrounding parenthesis. Returns true on success, false on error. This function may add words to the given vector, mostly to add parenthesis around operations based on operator precedence. 
bool ParseExpression(vector<Word>& words, int startIndex, int endIndex = -1) {
	auto lastWord = [&words, &endIndex]{
		return endIndex == -1? words.size()-1 : min(size_t(endIndex), words.size()-1);
	};
	
	// Handle single-word literal expressions (also includes single variable name)
	if (startIndex == lastWord() && (words[startIndex] == Word::Numeric || words[startIndex] == Word::Text || words[startIndex] == Word::Varname)) {
		return true;
	}
	
	// Any other type of single-word expression should be invalid
	if (startIndex >= lastWord()) {
		return false;
	}
	
	// Encapsulate operations within expressions, based on operator precedence
	for (Word::Type op = Word::Type(WORD_ENUM_OPERATOR_START); op < Word::CommaOperator; op = Word::Type(+op+1)) {
		for (int opIndex = startIndex; opIndex <= lastWord(); ++opIndex) {
			if (words[opIndex] == op) {
				auto word = words[opIndex];
				int prevPos = opIndex-1;
				int nextPos = opIndex+1;
				Word prev = prevPos >= startIndex? words[prevPos] : Word::Empty;
				Word next = nextPos <= lastWord()? words[nextPos] : Word::Empty;
				
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
						next = nextPos <= lastWord()? words[nextPos] : Word::Empty;
					}
					if (next == Word::ExpressionBegin) {
						nextPos = GetExpressionEnd(words, nextPos);
						if (nextPos == -1 || nextPos > lastWord()) {
							return false;
						}
					}
					if (prev == Word::ExpressionEnd) {
						prevPos = GetExpressionBegin(words, prevPos);
						if (prevPos < startIndex) {
							return false;
						}
					}
					// Handle function calls (and cast) within expressions
					if (next == Word::Name || next == Word::Funcname) {
						Word after = nextPos+1 <= lastWord()? words[nextPos+1] : Word::Empty;
						if (word != Word::CastOperator && word != Word::TrailOperator && after != Word::ExpressionBegin) {
							return false;
						}
						if (after == Word::ExpressionBegin) {
							nextPos = GetExpressionEnd(words, nextPos+1);
							if (nextPos == -1 || nextPos > lastWord()) {
								return false;
							}
						}
					}
					// Middle operators
					if (prev && next && prev != Word::ExpressionBegin && next != Word::ExpressionEnd && prev.type < WORD_ENUM_OPERATOR_START && next.type < WORD_ENUM_OPERATOR_START) {
						Word before = prevPos >= startIndex && prevPos > 0? words[prevPos-1] : Word::Empty;
						Word after = nextPos+1 <= lastWord()? words[nextPos+1] : Word::Empty;
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
								words.insert(words.begin() + nextPos + 1, Word::ExpressionEnd);
								words.insert(words.begin() + opIndex, Word::Numeric);
								words.insert(words.begin() + opIndex, Word::ExpressionBegin);
								opIndex += 2;
								if (endIndex != -1) endIndex += 3;
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
	const map<Word::Type, set<Word::Type>> authorizedSemantics {
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
	const map<Word::Type, map<Word::Type, set<Word::Type>>> authorizedSemanticsWhenPreceeded {
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
	for (int index = startIndex; index < lastWord(); ++index) {
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
int ParseDeclarationArgs(vector<Word>& words, int index) {
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
	if (end+1 >= words.size()) return -1;
	return end + 1;
	Error: throw ParseError("Invalid arguments in function declaration");
}

// Returns the position at which to continue parsing after the closing parenthesis, or -1 if reached the end of the line. This function also ensures that the returned value is less than the word count.
int ParseArgs(vector<Word>& words, int index) {
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
		if (end == words.size()-1) return -1;
		return end + 1;
	} else {
		throw ParseError("Invalid expression within function arguments");
	}
	return -1;
}

// Valid first words for the possible statements in the global scope
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

// Valid first words for the possible statements in a function scope
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

// Upon construction, it will parse the entire line from the given string, including expressions, and may throw ParseError
struct ParsedLine {
	int scope = 0;
	int line = 0;
	vector<Word> words {};
	
	operator bool () const {
		return words.size() > 0;
	}
	
	ParsedLine(const string& str, int line = 0) : line(line) {
		ParseWords(str, words, scope);
		
		if (words.size() > 0) {
			
			// Check first word validity for this Scope
			if (scope == 0) { // Global scope
				if (words[0] != Word::Name || find(begin(globalScopeFirstWords), end(globalScopeFirstWords), words[0]) == end(globalScopeFirstWords)) {
					throw ParseError("Invalid first word", words[0], "in the global scope");
				}
			} else { // Function scope
				if (words[0] != Word::Varname && words[0] != Word::Funcname && (words[0] != Word::Name || find(begin(functionScopeFirstWords), end(functionScopeFirstWords), words[0]) == end(functionScopeFirstWords))) {
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
							if (!ParseExpression(words, 3)) throw ParseError("Invalid expression after const assignment");
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
							throw ParseError("Invalid storage type", words[4], "it must be either 'number' or 'text'");
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
							if (!ParseExpression(words, 3)) {
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
						if (words.size() == 1 || !ParseExpression(words, 1)) {
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
					
				throw ParseError("Invalid word", words[0]);
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
	
	ParsedLine(const vector<Word>& words) : words(words) {}
	ParsedLine(const Word& word) {
		words.emplace_back(word);
	}
};

// Upon construction, it will parse all lines from the given stream, and may throw ParseError
struct SourceFile {
	string filepath;
	vector<ParsedLine> lines;

	SourceFile(const string& filepath) : filepath(filepath) {
		ifstream stream{filepath};
		
		if (stream.fail()) {
			throw ParseError("File not found '" + filepath + "'");
		}
		
		lines.emplace_back(Word{Word::FileInfo, filepath});
		
		int lineNumber = 1;
		int scope = 0;
		
		// Parse all lines
		for (string lineStr; getline(stream, lineStr); lineNumber++) {
			try {
				auto& line = lines.emplace_back(lineStr, lineNumber);
				if (line.scope > scope + 1) {
					throw ParseError("Too many leading tabs");
				}
				scope = line.scope;
			} catch (ParseError& e) {
				stringstream err {};
				err << e.what() << " at line " << lineNumber << " in " << filepath;
				throw ParseError(err.str());
			}
		}
	}
	
	void DebugParsedLines() {
		for (auto& line : lines) {
			if (line) {
				cout << setw(8) << line.line << ": " << string(line.scope,'\t');
				DebugWords(line.words);
				cout << endl;
			}
		}
		cout << "\nParsing Success!\n" << endl;
	}
};

#pragma endregion

#pragma region Compiler

constexpr uint32_t Interpret3CharsAsInt(const char* str) {
	return uint32_t(str[0]) | (uint32_t(str[1]) << 8) | (uint32_t(str[2]) << 16) | (32 << 24);
}
#define DEF_OP(op) inline constexpr uint32_t op = Interpret3CharsAsInt(#op);
#define NOT_IMPLEMENTED_YET assert(!"Not Implemented Yet");

DEF_OP( SET /* REF_DST REF_SRC */ ) // Assign
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
DEF_OP( LOG /* REF_DST REF_NUM REF_BASE */ ) // log(number, base)
DEF_OP( SQR /* REF_DST REF_NUM */ ) // sqrt(number)
DEF_OP( SIG /* REF_DST REF_NUM */ ) // sign(number)
DEF_OP( CLP /* REF_DST REF_NUM REF_MIN REF_MAX */ ) // clamp(number, min, max)
DEF_OP( STP /* REF_DST REF_T1 REF_T2 REF_NUM */ ) // step(t1, t2, number)
DEF_OP( SMT /* REF_DST REF_T1 REF_T2 REF_NUM */ ) // smoothstep(t1, t2, number)
DEF_OP( LRP /* REF_DST REF_NUM REF_NUM REF_T */ ) // lerp(number, number, t)
DEF_OP( SLP /* REF_DST REF_NUM REF_NUM REF_T */ ) // slerp(number, number, t)
DEF_OP( NUM /* REF_DST REF_SRC */ ) // cast numeric
DEF_OP( TXT /* REF_DST REF_SRC */ ) // cast text
DEF_OP( FMT /* REF_DST REF_SRC FORMAT_FLOAT */ ) // format cast text
DEF_OP( DEV /* DEVICE_FUNCTION_INDEX RET_DST [REF_ARG ...] */ ) // device function
DEF_OP( OUT /* OUTPUT_INDEX [REF_ARG ...] */ ) // output
DEF_OP( CLR /* REF_ARR */ ) // array.clear()
DEF_OP( APP /* REF_ARR REF_VALUE [REF_VALUE ...] */ ) // array.append(value, value...)
DEF_OP( POP /* REF_ARR */ ) // array.pop()
DEF_OP( INS /* REF_ARR REF_INDEX REF_VALUE [REF_VALUE ...] */ ) // array.insert(index, value, value...)
DEF_OP( DEL /* REF_ARR REF_INDEX */ ) // array.erase(index)
DEF_OP( FLL /* REF_ARR REF_QTY REF_VAL */ ) // array.fill(qty, value)
DEF_OP( ASC /* REF_ARR */ ) // array.sort()
DEF_OP( DSC /* REF_ARR */ ) // array.sortd()
DEF_OP( SIZ /* REF_DST (REF_ARR | REF_TXT) */ ) // array.size  text.size
DEF_OP( LAS /* REF_DST (REF_ARR | REF_TXT) */ ) // array.last  text.last
DEF_OP( SUM /* REF_DST REF_ARR */ ) // array.sum
DEF_OP( MIN /* REF_DST (REF_ARR | (REF_NUM [REF_NUM ...])) */ ) // array.min  min(number, number...)
DEF_OP( MAX /* REF_DST (REF_ARR | (REF_NUM [REF_NUM ...])) */ ) // array.max  max(number, number...)
DEF_OP( AVG /* REF_DST (REF_ARR | (REF_NUM [REF_NUM ...])) */ ) // array.avg  avg(number, number...)
DEF_OP( MED /* REF_DST REF_ARR */ ) // array.med
DEF_OP( SBS /* REF_DST REF_SRC REF_START REF_LENGTH */ ) // substring(text, start, length)
DEF_OP( IDX /* REF_DST REF_ARR REF_NUM */ ) // index an array
DEF_OP( JMP /* ADDR */ ) // jump to addr / goto
DEF_OP( CND /* ADDR REF_BOOL */ ) // conditional jump (gotoAddrIfFalse, boolExpression)

struct DeviceFunction {
	struct Arg {
		string name;
		string type;
	};
	
	uint32_t id;
	string name = "";
	vector<Arg> args {};
	string returnType = "";
	
	DeviceFunction(uint32_t id, const string& line) : id(id) {
		vector<Word> words;
		
		int nextWordIndex = 0;
		auto readWord = [&] (Word::Type type = Word::Empty) -> Word {
			if (nextWordIndex < words.size()) {
				assert(type == Word::Empty || type == words[nextWordIndex]);
				return words[nextWordIndex++];
			}
			return Word::Empty;
		};
		
		int scope = 0;
		ParseWords(line, words, scope);
		
		assert(words.size() > 0);
		assert(scope == 0);
		
		name = readWord(Word::Name).word;
		
		if (words.size() == 1) return;
		
		readWord(Word::ExpressionBegin);
		
		// Arguments
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
			string type = readWord(Word::Name);
			if (type != "number" && type != "text" && type != "data") {
				assert(!"Invalid argument type in device function prototype");
			}
			args.emplace_back(string(word), type);
		}
		
		// Return type
		if (readWord() == Word::CastOperator) {
			Word type = readWord(Word::Name);
			if (type != "number" && type != "text" && type != "data") {
				assert(!"Invalid return type in device function prototype");
			}
			returnType = string(type);
		}
	}
};

static vector<DeviceFunction> deviceFunctions {}; // Implementation SHOULD add functions to it

enum CODE_TYPE : uint8_t {
	// Statements
	RETURN = 0, // Must add this at the end of each function block
	VOID = 10, // Must add this after each OP statement, helps validate validity and also makes the bytecode kind of human readable
	OP = 32, // OP [...] VOID

	// Reference Values
	ROM_CONST_NUMERIC = 130,
	ROM_CONST_TEXT = 131,
	STORAGE_VAR_NUMERIC = 140,
	STORAGE_VAR_TEXT = 141,
	STORAGE_ARRAY_NUMERIC = 150,
	STORAGE_ARRAY_TEXT = 151,
	RAM_VAR_NUMERIC = 160,
	RAM_VAR_TEXT = 161,
	RAM_ARRAY_NUMERIC = 170,
	RAM_ARRAY_TEXT = 171,
	RAM_DATA = 180,
	// Extra reference
	ARRAY_INDEX = 201,
	OUTPUT_INDEX = 205,
	ADDR = 255,
};

struct ByteCode {
	union {
		uint32_t rawValue;
		struct {
			uint32_t value : 24; // 16 million possible values per type
			uint32_t type : 8; // 256 possible types
		};
	};
	ByteCode(uint32_t rawValue = 0) : rawValue(rawValue) {}
	ByteCode(uint8_t type, uint32_t value) : value(value), type(type) {}
};

ByteCode GetOperator(string op) {
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
	} else {
		throw ParseError("Invalid Operator", op);
	}
}

ByteCode GetBuiltInFunctionOp(string func) {
	if (func == "floor") return FLR;
	if (func == "ceil") return CIL;
	if (func == "round") return RND;
	if (func == "sin") return SIN;
	if (func == "cos") return COS;
	if (func == "tan") return TAN;
	if (func == "asin") return ASI;
	if (func == "acos") return ACO;
	if (func == "atan") return ATA;
	if (func == "abs") return ABS;
	if (func == "fract") return FRA;
	if (func == "sqrt") return SQR;
	if (func == "sign") return SIG;
	if (func == "pow") return POW;
	if (func == "log") return LOG;
	if (func == "clamp") return CLP;
	if (func == "step") return STP;
	if (func == "smoothstep") return SMT;
	if (func == "lerp") return LRP;
	if (func == "slerp") return SLP;
	if (func == "size") return SIZ;
	if (func == "last") return LAS;
	if (func == "mod") return MOD;
	if (func == "min") return MIN;
	if (func == "max") return MAX;
	if (func == "avg") return AVG;
	if (func == "med") return MED;
	if (func == "sum") return SUM;
	if (func == "add") return ADD;
	if (func == "sub") return SUB;
	if (func == "mul") return MUL;
	if (func == "div") return DIV;
	if (func == "clear") return CLR;
	if (func == "append") return APP;
	if (func == "pop") return POP;
	if (func == "insert") return INS;
	if (func == "erase") return DEL;
	if (func == "fill") return FLL;
	if (func == "substring") return SBS;
	if (func == "sort") return ASC;
	if (func == "sortd") return DSC;
	return DEV;
}

// Is non-const non-array assignable var
bool IsVar(ByteCode v) {
	switch (v.type) {
		case STORAGE_VAR_NUMERIC:
		case STORAGE_VAR_TEXT:
		case RAM_VAR_NUMERIC:
		case RAM_VAR_TEXT:
		return true;
	}
	return false;
}

bool IsArray(ByteCode v) {
	switch (v.type) {
		case STORAGE_ARRAY_NUMERIC:
		case STORAGE_ARRAY_TEXT:
		case RAM_ARRAY_NUMERIC:
		case RAM_ARRAY_TEXT:
		return true;
	}
	return false;
}

class Assembly {
	static inline const string parserFiletype = "XenonCode!";
	static inline const uint32_t parserVersion = VERSION_MINOR;
	
public:
	uint32_t varsInitSize = 0; // number of byte codes in the vars_init code (uint32_t)
	uint32_t programSize = 0; // number of byte codes in the program code (uint32_t)
	vector<string> storageRefs {}; // storage references (addr)
	unordered_map<string, uint32_t> functionRefs {}; // function references (addr)
	/* Function names:
		system.init
		system.tick
		system.input.1
		system.timer.frequency.1
	Varnames related to functions:
		@funcname.1 		argument
		@funcname:			return
		system.input.1.1	input function argument
	*/
	
	// ROM
	vector<ByteCode> rom_vars_init {}; // the bytecode of this computer's vars_init function
	vector<ByteCode> rom_program {}; // the actual bytecode of this program
	vector<double> rom_numericConstants {}; // the actual numeric constant values
	vector<string> rom_textConstants {}; // the actual text constant values
	
	// RAM size
	uint32_t ram_numericVariables = 0;
	uint32_t ram_textVariables = 0;
	uint32_t ram_dataReferences = 0;
	uint32_t ram_numericArrays = 0;
	uint32_t ram_textArrays = 0;
	
	// Helpers
	Word GetConstValue(ByteCode ref) const {
		if (ref.type == ROM_CONST_NUMERIC) {
			return {Word::Numeric, ToString(rom_numericConstants[ref.value])};
		} else if (ref.type == ROM_CONST_TEXT) {
			return {Word::Text, rom_textConstants[ref.value]};
		} else {
			throw ParseError("Not const");
		}
	}
	
	// From Parsed lines of code
	explicit Assembly(const vector<ParsedLine>& lines) {
		// Current context
		string currentFile = "";
		int currentLine = 0;
		int currentScope = 0;
		string currentFunctionName = "";
		uint32_t currentFunctionAddr = 0;
		
		// Temporary user-defined symbol maps
		unordered_map<string/*functionScope*/, map<int/*scope*/, unordered_map<string/*name*/, ByteCode>>> userVars {};
		
		// Validation helper
		auto validate = [](bool condition){
			if (!condition) throw ParseError("Invalid statement");
		};
		
		// Lambda functions to Get/Add user-defined symbols
		auto getVar = [&](const string& name) -> ByteCode {
			int scope = currentScope;
			string functionScope = currentFunctionName;
			while (!userVars[functionScope][scope].contains(name)) {
				if (--scope < 0) {
					if (functionScope == "") {
						throw ParseError("$" + name + " is undefined");
					} else {
						functionScope = "";
						scope = 0;
					}
				}
			}
			return userVars.at(functionScope).at(scope).at(name);
		};
		auto declareVar = [&](const string& name, CODE_TYPE type, Word initialValue/*Only for Const*/ = Word::Empty) -> ByteCode {
			int scope = currentScope;
			if (name != "") {
				for (int scope = currentScope; scope >= 0; --scope) {
					if (userVars[currentFunctionName][scope].contains(name)) {
						throw ParseError("$" + name + " is already defined");
					}
				}
				if (currentFunctionName != "") {
					if (userVars[""][0].contains(name)) {
						throw ParseError("$" + name + " is already defined");
					}
				}
			}
			
			uint32_t index;
			switch (type) {
				case STORAGE_VAR_NUMERIC:
				case STORAGE_VAR_TEXT:
				case STORAGE_ARRAY_NUMERIC:
				case STORAGE_ARRAY_TEXT:
					index = storageRefs.size();
					assert(name != "");
					storageRefs.emplace_back(name);
					break;
					
				case ROM_CONST_NUMERIC:
					if (!initialValue) throw ParseError("Const value not provided");
					index = rom_numericConstants.size();
					rom_numericConstants.emplace_back(initialValue);
					break;
				case ROM_CONST_TEXT:
					if (!initialValue) throw ParseError("Const value not provided");
					index = rom_textConstants.size();
					rom_textConstants.emplace_back(initialValue);
					break;
					
				case RAM_VAR_NUMERIC:
					index = ram_numericVariables++;
					break;
				case RAM_VAR_TEXT:
					index = ram_textVariables++;
					break;
				case RAM_DATA:
					index = ram_dataReferences++;
					break;
				case RAM_ARRAY_NUMERIC:
					index = ram_numericArrays++;
					break;
				case RAM_ARRAY_TEXT:
					index = ram_textArrays++;
					break;
				
				default: assert(!"Invalid CODE_TYPE");
			}
			
			ByteCode byteCode{uint8_t(type), index};
			if (name != "") {
				userVars[currentFunctionName][currentScope].emplace(name, byteCode);
			}
			return byteCode;
		};
		auto getReturnVar = [&](const string& funcName) -> ByteCode {
			string retVarName = "@"+funcName+":";
			if (userVars.contains(funcName) && userVars.at(funcName).contains(0) && userVars.at(funcName).at(0).contains(retVarName)) {
				return userVars.at(funcName).at(0).at(retVarName);
			} else {
				return VOID;
			}
		};
		
		// Compile helpers
		auto addr = [&]{
			return rom_program.size();
		};
		auto write = [&](ByteCode c){
			rom_program.emplace_back(c);
		};
		auto jump = [&](uint32_t jumpToAddress){
			rom_program.emplace_back(JMP);
			rom_program.emplace_back(ADDR, jumpToAddress);
			rom_program.emplace_back(VOID);
		};
		
		// Function helpers
		auto openFunction = [&](const string& name){
			assert(currentScope == 0);
			assert(currentFunctionName == "");
			if (functionRefs.contains(name)) {
				throw ParseError("Function " + name + " is already defined");
			}
			currentFunctionName = name;
			currentFunctionAddr = addr();
		};
		auto closeCurrentFunction = [&](){
			if (currentFunctionName != "") {
				write(RETURN);
				functionRefs.emplace(currentFunctionName, currentFunctionAddr);
				currentFunctionName = "";
				currentFunctionAddr = 0;
			}
		};
		// If it's a user-declared function and it has a return type defined, returns the return var ref of that function, otherwise returns VOID
		auto compileFunctionCall = [&](Word func, const vector<ByteCode>& args) -> ByteCode {
			string funcName = func;
			if (func == Word::Funcname) {
				if (!functionRefs.contains(funcName)) {
					throw ParseError("Function", func, "is not defined");
				}
				
				// Set arguments
				int i = 0;
				for (auto arg : args) {
					string argVarName = "@"+funcName+"."+to_string(++i);
					if (userVars.contains(funcName) && userVars.at(funcName).contains(0) && userVars.at(funcName).at(0).contains(argVarName)) {
						write(SET);
						write(userVars.at(funcName).at(0).at(argVarName));
						write(arg);
						write(VOID);
					} else break;
				}
				
				// Call the function
				jump(functionRefs.at(funcName));
				
				// Get the Return value
				return getReturnVar(funcName);
				
			} else if (func == Word::Name) {
				ByteCode f = GetBuiltInFunctionOp(funcName);
				write(f);
				if (f.value == DEV) {
					auto function = find_if(deviceFunctions.begin(), deviceFunctions.end(), [&](auto& a){return a.name == funcName;});
					if (function == deviceFunctions.end()) {
						throw ParseError("Function", func, "does not exist");
					}
					write(function->id);
				}
				for (auto arg : args) {
					write(arg);
				}
				write(VOID);
				return VOID;
			} else {
				throw ParseError("Invalid function name");
			}
		};
		
		// Scope helpers
		auto pushScope = [&](/*...???...*/) {
			++currentScope;
		};
		auto popScope = [&] {
			--currentScope;
			assert(currentScope >= 0);
		};
		
		// Expression helpers
		/*computeConstExpression*/ function<Word(const vector<Word>&, int, int)> computeConstExpression = [&](const vector<Word>& words, int startIndex, int endIndex = -1) -> Word {
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
					throw ParseError("Const expression may only be cast to either 'number' or 'text'");
				}
			} else if (op == Word::NotOperator) {
				if (word2 == Word::Numeric) {
					double value = word2;
					word2 = double(value == 0.0);
					return word2;
				} else if (word2 == Word::Text) {
					string value = word2;
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
						throw ParseError("Division by zero in const expression");
					}
					return Word{double(word1) / double(word2)};
				} else if (op == "^") {
					return Word{pow(double(word1), double(word2))};
				} else if (op == "%") {
					if (double(word2) == 0.0) {
						throw ParseError("Division by zero in const expression");
					}
					return Word{double(long(round(double(word1))) % long(round(double(word2))))};
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
						return Word{double(string(word1) == string(word2))};
					} else if (op == "!=" || op == "<>") {
						return Word{double(string(word1) != string(word2))};
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
			throw ParseError("Invalid operator", op, "in const expression");
		};
		/*compileExpression*/ function<ByteCode(const vector<Word>&, int, int)> compileExpression = [&](const vector<Word>& words, int startIndex, int endIndex = -1) -> ByteCode {
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
				case Word::Funcname:{
					vector<ByteCode> args {};//TODO fill this
					ref1 = compileFunctionCall(word1, args);
				}break;
				case Word::Name:{
					
				}break;
				case Word::Void: break;
				default: validate(false);
			}
			
			// Get Operator
			Word op = words[opIndex];
			validate(op.type >= WORD_ENUM_OPERATOR_START);
	
			// // Get Word 2
			// //TODO....
			
			// // Validate that there are no more words remaining
			// validate(word2Index == endIndex);
			
			// 		// Compute result
			// 		if (op == Word::CastOperator) {
			// 			if (word2 == "number") {
			// 				word1.type = Word::Numeric;
			// 				return word1;
			// 			} else if (word2 == "text") {
			// 				word1.type = Word::Text;
			// 				return word1;
			// 			} else {
			// 				throw ParseError("Const expression may only be cast to either 'number' or 'text'");
			// 			}
			// 		} else if (op == Word::NotOperator) {
			// 			if (word2 == Word::Numeric) {
			// 				double value = word2;
			// 				word2 = double(value == 0.0);
			// 				return word2;
			// 			} else if (word2 == Word::Text) {
			// 				string value = word2;
			// 				word2 = double(value == "");
			// 				return word2;
			// 			} else {
			// 				validate(false);
			// 			}
			// 		} else if (op == Word::ConcatOperator) {
			// 			validate(word1 == Word::Text && word2 == Word::Text);
			// 			word1.word += word2.word;
			// 			return word1;
			// 		} else if (op == Word::MulOperatorGroup) {
			// 			validate(word1 == Word::Numeric && word2 == Word::Numeric);
			// 			if (op == "*") {
			// 				return Word{double(word1) * double(word2)};
			// 			} else if (op == "/") {
			// 				if (double(word2) == 0.0) {
			// 					throw ParseError("Division by zero in const expression");
			// 				}
			// 				return Word{double(word1) / double(word2)};
			// 			} else if (op == "^") {
			// 				return Word{pow(double(word1), double(word2))};
			// 			} else if (op == "%") {
			// 				if (double(word2) == 0.0) {
			// 					throw ParseError("Division by zero in const expression");
			// 				}
			// 				return Word{double(long(round(double(word1))) % long(round(double(word2))))};
			// 			} else {
			// 				validate(false);
			// 			}
			// 		} else if (op == Word::AddOperatorGroup) {
			// 			validate(word1 == Word::Numeric && word2 == Word::Numeric);
			// 			if (op == "+") {
			// 				return Word{double(word1) + double(word2)};
			// 			} else if (op == "-") {
			// 				return Word{double(word1) - double(word2)};
			// 			} else {
			// 				validate(false);
			// 			}
			// 		} else if (op == Word::CompareOperatorGroup) {
			// 			validate(word1 == Word::Numeric && word2 == Word::Numeric);
			// 			if (op == "<") {
			// 				return Word{double(double(word1) < double(word2))};
			// 			} else if (op == "<=") {
			// 				return Word{double(double(word1) <= double(word2))};
			// 			} else if (op == ">") {
			// 				return Word{double(double(word1) > double(word2))};
			// 			} else if (op == ">=") {
			// 				return Word{double(double(word1) >= double(word2))};
			// 			} else {
			// 				validate(false);
			// 			}
			// 		} else if (op == Word::EqualityOperatorGroup) {
			// 			if (word1 == Word::Numeric && word2 == Word::Numeric) {
			// 				if (op == "==") {
			// 					return Word{double(double(word1) == double(word2))};
			// 				} else if (op == "!=" || op == "<>") {
			// 					return Word{double(double(word1) != double(word2))};
			// 				} else {
			// 					validate(false);
			// 				}
			// 			} else if (word1 == Word::Text && word2 == Word::Text) {
			// 				if (op == "==") {
			// 					return Word{double(string(word1) == string(word2))};
			// 				} else if (op == "!=" || op == "<>") {
			// 					return Word{double(string(word1) != string(word2))};
			// 				} else {
			// 					validate(false);
			// 				}
			// 			} else {
			// 				validate(false);
			// 			}
			// 		} else if (op == Word::AndOperator) {
			// 			validate(word1 == Word::Numeric && word2 == Word::Numeric);
			// 			return Word{double(double(word1) && double(word2))};
			// 		} else if (op == Word::OrOperator) {
			// 			validate(word1 == Word::Numeric && word2 == Word::Numeric);
			// 			return Word{double(double(word1) || double(word2))};
			// 		}
			// 		throw ParseError("Invalid operator", op, "in const expression");
					
			
			
			
			
			
			
			
			
		};
		
		// Add a Return in addr 0
		write(RETURN);
		
		// Start parsing
		try {
			for (const auto& line : lines) if (line) {
				currentLine = line.line;
				int nextWordIndex = 0;
				auto readWord = [&] (Word::Type type = Word::Empty) -> Word {
					if (nextWordIndex < line.words.size()) {
						validate(type == Word::Empty || type == line.words[nextWordIndex]);
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
						}break;
						case Word::Name: {
							
							// Adjust scope
							while (line.scope < currentScope) {
								popScope();
							}
							
							// Close current function, if any
							closeCurrentFunction();
							
							// Parse declarations from global scope
							
							// const
							if (firstWord == "const") {
								string name = readWord(Word::Varname);
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
									throw ParseError("Cannot assign non-const expression to const value");
								}
							}
							// var
							else if (firstWord == "var") {
								string name = readWord(Word::Varname);
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
										if (string(value) != "") {
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
											default: throw ParseError("Invalid assignment");
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
										throw ParseError("Var declaration in global scope can only be of type 'number' or 'text'");
									}
									validate(!readWord());
								}
							}
							// array
							else if (firstWord == "array") {
								string name = readWord();
								validate(name != "");
								validate(readWord() == ":");
								Word type = readWord();
								if (type == "number") {
									declareVar(name, RAM_ARRAY_NUMERIC);
								} else if (type == "text") {
									declareVar(name, RAM_ARRAY_TEXT);
								} else {
									throw ParseError("Array declaration in global scope can only be of type 'number' or 'text'");
								}
								validate(!readWord());
							}
							// storage
							else if (firstWord == "storage") {
								string what = readWord();
								validate(what == "var" || what == "array");
								string name = readWord();
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
									throw ParseError("Storage declaration in global scope can only be of type 'number' or 'text'");
								}
								validate(!readWord());
							}
							// init
							else if (firstWord == "init") {
								openFunction("system.init");
								pushScope();
							}
							// tick
							else if (firstWord == "tick") {
								openFunction("system.tick");
								pushScope();
							}
							// function
							else if (firstWord == "function") {
								string name = readWord(Word::Funcname);
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
									string type = readWord(Word::Name);
									ByteCode arg;
									if (type == "number") {
										arg = declareVar(word, RAM_VAR_NUMERIC);
									} else if (type == "text") {
										arg = declareVar(word, RAM_VAR_TEXT);
									} else if (type == "data") {
										arg = declareVar(word, RAM_DATA);
									} else {
										throw ParseError("Invalid argument type in function declaration");
									}
									userVars[currentFunctionName][0].emplace("@"+name+"."+to_string(argN), arg);
								}
								// Return type
								if (readWord() == Word::CastOperator) {
									Word type = readWord(Word::Name);
									if (type == "number") {
										declareVar("@"+name+":", RAM_VAR_NUMERIC);
									} else if (type == "text") {
										declareVar("@"+name+":", RAM_VAR_TEXT);
									} else if (type == "data") {
										declareVar("@"+name+":", RAM_DATA);
									} else {
										throw ParseError("Invalid return type in function declaration");
									}
								}
								pushScope();
							}
							// timer
							else if (firstWord == "timer") {
								string timerType = readWord(Word::Name);
								Word timerValue = readWord();
								if (timerValue == Word::Varname) {
									timerValue = GetConstValue(getVar(timerValue));
								}
								validate(timerValue == Word::Numeric);
								openFunction("system.timer."+timerType+"."+string(timerValue));
								pushScope();
							}
							// input
							else if (firstWord == "input") {
								readWord(Word::TrailOperator);
								Word inputIndex = readWord();
								if (inputIndex == Word::Varname) {
									inputIndex = GetConstValue(getVar(inputIndex));
								}
								validate(inputIndex == Word::Numeric);
								string name = "system.input."+string(inputIndex);
								openFunction(name);
								readWord(Word::ExpressionBegin);
								// Arguments
								int argN = 0;
								for (;;) {
									Word word = readWord();
									if (!word || word == Word::ExpressionEnd) break;
									++argN;
									validate(word == Word::Varname);
									readWord(Word::CastOperator);
									string type = readWord(Word::Name);
									ByteCode arg;
									if (type == "number") {
										arg = declareVar(word, RAM_VAR_NUMERIC);
									} else if (type == "text") {
										arg = declareVar(word, RAM_VAR_TEXT);
									} else if (type == "data") {
										arg = declareVar(word, RAM_DATA);
									} else {
										throw ParseError("Invalid argument type in function declaration");
									}
									userVars[currentFunctionName][0].emplace(name+"."+to_string(argN), arg);
								}
								validate(!readWord());
								pushScope();
							}
							// ERROR
							else {
								throw ParseError("Invalid statement");
							}
						}break;
						default: throw ParseError("Invalid statement");
					}
				} else {
					// Function Scope
					if (line.scope > currentScope) {
						throw ParseError("Invalid scope");
					}
					while (line.scope < currentScope) {
						popScope();
					}
					switch (firstWord.type) {
						case Word::Varname: {
							// Variable assignment
							ByteCode dst = getVar(firstWord);
							Word operation = readWord();
							switch (operation.type) {
								case Word::AssignmentOperatorGroup:{
									validate(IsVar(dst));
									ByteCode op = GetOperator(operation);
									ByteCode ref = compileExpression(line.words, nextWordIndex, -1);
									write(op);
									write(dst);
									if (op.value != SET) write(dst);
									write(ref);
									write(VOID);
								}break;
								case Word::TrailOperator:{
									NOT_IMPLEMENTED_YET
								}break;
								default: throw ParseError("Invalid operator", operation);
							}
						}break;
						case Word::Funcname: {
							// Function call
							NOT_IMPLEMENTED_YET
						}break;
						case Word::Name: {
							// var
							if (firstWord == "var") {
								NOT_IMPLEMENTED_YET
							}
							// array
							else if (firstWord == "array") {
								NOT_IMPLEMENTED_YET
							}
							// output
							else if (firstWord == "output") {
								NOT_IMPLEMENTED_YET
							}
							// foreach
							else if (firstWord == "foreach") {
								NOT_IMPLEMENTED_YET
							}
							// repeat
							else if (firstWord == "repeat") {
								NOT_IMPLEMENTED_YET
							}
							// while
							else if (firstWord == "while") {
								NOT_IMPLEMENTED_YET
							}
							// break
							else if (firstWord == "break") {
								NOT_IMPLEMENTED_YET
							}
							// next
							else if (firstWord == "next") {
								NOT_IMPLEMENTED_YET
							}
							// if
							else if (firstWord == "if") {
								NOT_IMPLEMENTED_YET
							}
							// elseif
							else if (firstWord == "elseif") {
								NOT_IMPLEMENTED_YET
							}
							// else
							else if (firstWord == "else") {
								NOT_IMPLEMENTED_YET
							}
							// return
							else if (firstWord == "return") {
								NOT_IMPLEMENTED_YET
							}
							// ERROR
							else {
								throw ParseError("Invalid statement");
							}
						}break;
						default: throw ParseError("Invalid statement");
					}
				}
			}
			closeCurrentFunction();
		} catch (ParseError& e) {
			stringstream err;
			err << e.what() << " at line " << currentLine << " in " << currentFile;
			throw ParseError(err.str());
		}
		varsInitSize = rom_vars_init.size();
		programSize = rom_program.size();
	}

	// From ByteCode stream
	explicit Assembly(istream& s) {Read(s);}
	
	void Write(ostream& s) {
		{// Write Header
			// Write assembly file info
			s << parserFiletype << ' ' << parserVersion << '\n';
			
			// Write some sizes
			s << varsInitSize << ' ' << programSize << ' ' << storageRefs.size() << ' ' << functionRefs.size() << '\n';
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
		
		// Write vars_init bytecode
		assert(rom_vars_init.size() == varsInitSize);
		s.write((char*)rom_vars_init.data(), varsInitSize * sizeof(uint32_t));
		
		// Write program bytecode
		assert(rom_program.size() == programSize);
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
			s >> varsInitSize >> programSize >> storageRefsSize >> functionRefsSize;
			s >> rom_numericConstantsSize;
			s >> rom_textConstantsSize;
			s >> ram_numericVariables;
			s >> ram_textVariables;
			s >> ram_dataReferences;
			s >> ram_numericArrays;
			s >> ram_textArrays;

			// Prepare data
			rom_vars_init.resize(varsInitSize);
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

		// Read vars_init bytecode
		s.read((char*)rom_vars_init.data(), varsInitSize * sizeof(uint32_t));

		// Read program bytecode
		s.read((char*)rom_program.data(), programSize * sizeof(uint32_t));
	}
};

#pragma endregion

}
