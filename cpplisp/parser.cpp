#include "parser.hpp"

#include <regex>

using std :: regex;
using std :: regex_replace;

#include <cstdlib>

using std :: atoi;

std :: list < std :: string > tokenize (const std :: string& sourceCode) {
	regex openParens  ("\\(");
	regex closeParens ("\\)");
	regex tokens      ("([^\\s]+)");

	auto openParensProcessed  = regex_replace (sourceCode, openParens, " ( ");
	auto closeParensProcessed = regex_replace (openParensProcessed, closeParens, " ) ");

	std :: list <std :: string> result;

	std :: copy (
		std :: sregex_token_iterator (closeParensProcessed .begin (), closeParensProcessed .end (), tokens), // all matches of regex
		std :: sregex_token_iterator (), // until they end
		std :: back_inserter (result)); // into result vector

	return result;
};

#include <stack>

const int parens_balance (const std :: list <std :: string>& tokens) {
	std :: stack < char > parens; // FIXME replace by list
	
	for (const auto& token : tokens) {
		if (token == "(") {
			parens .emplace ('(');
		} else

		if (token == ")") {
			if (parens .empty () or parens .top () == ')')
				return -1;
			parens .pop ();
		}
	}
	
	return parens .empty () ? 0 : 1;
};

/**
	FIXME rename!
*/

std :: shared_ptr < object > build_s_expr (std :: list <std :: string>& tokens) {
	regex integer ("\\d+");

	if (tokens .empty ())
		throw std :: runtime_error ("Failed to build expression: token list is empty!");

	std :: string token = tokens .back ();
	tokens .pop_back ();
	
	if (token == "(")
		throw std :: runtime_error ("Failed to build expression: unexpected character '('!");

	if (token == ")") {
		std :: shared_ptr < object > list = NIL;
		
		while (tokens .back () != "(")
			list = cons (build_s_expr (tokens), list);
		tokens .pop_back ();

		return list;
	} else

	if (std :: regex_match (token, integer)) {
		return create_object (type :: INT, std :: make_shared < int > (atoi (token .c_str ())));
	} else

	return create_object (type :: SYMBOL, std :: make_shared < std :: string > (token));
}
