#include "parser.hpp"

#include <regex>
#include <cstdlib>

std :: list <std :: string> tokenize (const std :: string& sourceCode) {
	std :: regex openParens  ("\\(");
	std :: regex closeParens ("\\)");
	std :: regex tokens      ("([^\\s]+)");

	auto openParensProcessed  = std :: regex_replace (sourceCode, openParens, " ( ");
	auto closeParensProcessed = std :: regex_replace (openParensProcessed, closeParens, " ) ");

	std :: list <std :: string> result;

	std :: copy (
		std :: sregex_token_iterator (closeParensProcessed .begin (), closeParensProcessed .end (), tokens), // all matches of regex
		std :: sregex_token_iterator (), // until they end
		std :: back_inserter (result)); // into result vector

	return result;
};

std :: shared_ptr < object > build_s_expr (std :: list <std :: string>& tokens) {
	std :: shared_ptr < object > cell;

	std :: regex integer ("[1-9]+");

	while (tokens .size () > 0) {
		std :: string token = tokens .back ();
		tokens .pop_back ();
		
		if (token == ")") {
			auto cons_cell = build_s_expr (tokens);
			cell = cons (cons_cell, cell);
		} else
			
		if (token == "(") {
			return cell;
		} else

		if (std :: regex_match (token, integer)) {
			auto integer = create_object (type :: INT, std :: shared_ptr < int > (new int (std :: atoi (token .c_str ()))));
			cell = cons (integer, cell);
		} else {
			auto symbol = create_object (type :: SYMBOL, std :: shared_ptr < std :: string > (new std :: string (token)));
			cell = cons (symbol, cell);
		}
	};
	
	return cell;
};