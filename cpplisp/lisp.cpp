#include <iostream>
#include <vector>
#include <memory>

#include "object.hpp"
#include "cons_cell.hpp"

#include "parser.hpp"
#include "evaluate.hpp"

int main (int argc, char* argv[]) {

	try {
		std :: cout << "C++ LISP interpret REPL." << std :: endl;
		std :: cout << "C-c to exit." << std :: endl;
		std :: cout << std :: endl;

		while (true) {
			std :: string input;
			
			std :: cout << "lisp repl > ";
			std :: getline (std :: cin, input);
			
			if (input .empty ())
				continue;
			
			auto tokens = tokenize (input);

			while (parens_balance (tokens) > 0) {
				std :: string temp;

				std :: cout << ">> ";
				std :: getline (std :: cin, temp);

				input += temp;
				tokens = tokenize (input);
			};

			if (parens_balance (tokens) < 0)
				throw std :: runtime_error ("THERE ARE MORE CLOSING PARENTHESES IN EXPR THAN EXPECTED");

			auto expr = build_s_expr (tokens);
			Context repl_environment (nullptr);
			auto result = evaluate (get_object_data <cons_cell> (expr) -> car, repl_environment);

			std :: cout << print (result) << std :: endl;
		}
	} catch (std :: exception& e) {
		std :: cout << "Exception thrown: " << e .what () << std :: endl;
	};
	
	return EXIT_SUCCESS;
};