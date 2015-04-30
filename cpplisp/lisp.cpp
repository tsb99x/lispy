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
		auto tokens = tokenize ("(cons (quote A) (quote (B C)))");
		//auto tokens = tokenize ("(equal (car (quote (A B))) (quote A))");
		
		std :: cout << "Tokenized expression : ";
		for (auto& it : tokens) std :: cout << " " << it << " ";
		std :: cout << std :: endl;

		auto expr   = build_s_expr (tokens);
		std :: cout << "Parsed expression : " << print (get_object_data <cons_cell> (expr) -> car) << std :: endl;

		auto result = evaluate (get_object_data <cons_cell> (expr) -> car);
		std :: cout << "Interpret result : " << print (result) << std :: endl;
	} catch (std :: exception& e) {
		std :: cout << "Exception thrown: " << e .what () << std :: endl;
	};
	
	return EXIT_SUCCESS;
};