#include "parser.hpp"

#include <regex>
#include <iostream>
#include <cstdlib>

std :: list < std :: string > tokenize (const std :: string& sourceCode) {
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

const int parens_balance (const std :: list <std :: string>& tokens) {
	int open_brackets_count = 0;

	for (const auto& token : tokens) {
		if (token == "(") open_brackets_count++;
		if (token == ")") open_brackets_count--;
	}

	return open_brackets_count;
};

std :: shared_ptr < object > build_s_expr (std :: list <std :: string>& tokens) {
	std :: shared_ptr < object > cell = NIL; // FIXME analogue of NIL

	std :: regex integer ("\\d+");

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
