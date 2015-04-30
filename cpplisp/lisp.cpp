#include <iostream>
#include <vector>
#include <memory>

#include "object.hpp"
#include "cons_cell.hpp"

#include "parser.hpp"
#include "evaluate.hpp"

int main () {
	try {
		//auto tokens = tokenize ("(quote A)");
		//auto tokens = tokenize ("(car (quote (A B C)))");
		//auto tokens = tokenize ("(cdr (quote (A B C)))");
		//auto tokens = tokenize ("(cons (quote A) (quote (B C)))");
		auto tokens = tokenize ("(equal (car (quote (A B))) (quote A))");
		auto expr   = build_s_expr (tokens);
		auto result = evaluate (get_object_data <cons_cell> (expr -> car));

		std :: cout << "Parsed expression : " << print (expr -> car) << std :: endl;
		std :: cout << "Interpret result : " << print (result) << std :: endl;
	} catch (std :: exception& e) {
		std :: cout << "Exception thrown: " << e .what () << std :: endl;
	};
	
	return EXIT_SUCCESS;
};