#include "parser.hpp"

#include <regex>
#include <cstdlib>

std :: list <std :: string> tokenize (const std :: string& sourceCode) {
	std :: regex openParens ("\\(");
	std :: regex closeParens ("\\)");
	std :: regex tokens ("([^\\s]+)");

	std :: string openParensProcessed = std :: regex_replace (sourceCode, openParens, " ( ");
	std :: string closeParensProcessed = std :: regex_replace (openParensProcessed, closeParens, " ) ");
	
	std :: list <std :: string> result;

	std :: copy (
		std :: sregex_token_iterator (closeParensProcessed .begin (), closeParensProcessed .end (), tokens), // all matches of regex
		std :: sregex_token_iterator (), // until they end
		std :: back_inserter (result)); // into result vector

	return result;
};

cons_cell* build_s_expr (std :: list <std :: string>& tokens) {
	cons_cell* cell = nullptr;

	std :: regex integer ("[1-9]+"); // FIXME minus in front of number?
	
	while (tokens .size () > 0) {
		std :: string token = tokens .back ();
		tokens .pop_back ();
		
		if (token == ")")
			cell = cons (create_object (type :: CONS, build_s_expr (tokens)), cell);
		else
		if (token == "(")
			return cell;
		else
		if (std :: regex_match (token, integer))
			cell = cons (create_object (type :: INT, new int (std :: atoi (token .c_str ()))), cell);
		else
			cell = cons (create_object (type :: SYMBOL, new std :: string (token)), cell);
	};
	
	return cell;
};