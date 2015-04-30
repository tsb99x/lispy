#include <iostream>
#include <vector>
#include <memory>

#include "object.hpp"
#include "cons_cell.hpp"

#include "parser.hpp"
#include "evaluate.hpp"

context variables_context;

int main (int argc, char* argv[]) {
	
	std :: pair < std :: string, std :: shared_ptr <object> > a ("X", create_object (type :: INT, std :: shared_ptr <int> (new int (100))));
	
	variables_context .insert (a);
	
	try {
		std :: string src;

		if (argc > 1)
			src = std :: string (argv [ 1 ]);
		else
		//auto tokens = tokenize ("(quote A)");
		//auto tokens = tokenize ("(car (quote (A B C)))");
		//auto tokens = tokenize ("(cdr (quote (A B C)))");
		//auto tokens = tokenize ("(cons (quote A) (quote (B C)))");
		//src = "(equal (car (quote (A B))) (quote A))";

		// FIXME check for parenteses balance!

		while (true) {
			std :: string input;
			
			std :: cout << "lisp repl > ";
			std :: getline (std :: cin, input);
			
			if (input .empty ())
				continue;
			
			auto tokens = tokenize (input);
			auto expr = build_s_expr (tokens);
			auto result = evaluate (get_object_data <cons_cell> (expr) -> car, :: variables_context);

			std :: cout << print (result) << std :: endl;
		}
		
		/*auto tokens = tokenize (src);
		
		std :: cout << "Tokenized expression : ";
		for (auto& it : tokens) std :: cout << " " << it << " ";
		std :: cout << std :: endl;

		auto expr   = build_s_expr (tokens);
		std :: cout << "Parsed expression : " << print (get_object_data <cons_cell> (expr) -> car) << std :: endl;

		auto result = evaluate (get_object_data <cons_cell> (expr) -> car);
		std :: cout << "Interpret result : " << print (result) << std :: endl;*/
	} catch (std :: exception& e) {
		std :: cout << "Exception thrown: " << e .what () << std :: endl;
	};
	
	return EXIT_SUCCESS;
};