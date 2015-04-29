#include <iostream>
#include <vector>
#include <memory>

#include "object.hpp"
#include "cons_cell.hpp"

/*char *string_to_chars (std :: string str) {
	char *res = new char [str .length () + 1];
	strcpy (res, str .c_str ());
	return res;
};*/

std :: unique_ptr <object> interpret (cons_cell* cells);

std :: unique_ptr <object> sum (cons_cell* arguments) {
	cons_cell* arg = arguments;
	std :: vector <int> args; // FIXME no vector?

	while (arg != nullptr) {
		switch (arg -> car -> type) {
			case INT: {
				args .push_back (
					*get_object_data <int> (arg -> car));
				break;
			}
			case CONS: {
				args .push_back (
					*get_object_data <int> (
						interpret (
							get_object_data <cons_cell> (arg -> car)) .get ()));
				break;
			}
			default: {
				throw std :: runtime_error ("SUM_SUPPORT_ONLY_INT_FOR_NOW!");
				break;
			}
		};

		arg = arg -> cdr;
	};

	int res = 0;

	for (auto& it : args) res += it;

	return std :: unique_ptr <object> (create_object (type :: INT, new int (res)));
}

std :: unique_ptr <object> mul (cons_cell* arguments) {
	int result = 1;

	for (cons_cell* arg = arguments; arg != nullptr; arg = arg -> cdr) {
		object* arg_object = arg -> car;

		if (arg_object -> type == type :: CONS) {
			auto expr_res = interpret (get_object_data <cons_cell> (arg_object));
			arg_object = expr_res .get ();
			
			result *= (*get_object_data <int> (arg_object));
		} else

		if (arg_object -> type == type :: INT)
			result *= (*get_object_data <int> (arg_object));
		else
			throw std :: runtime_error ("MUL_SUPPORT_ONLY_INT_FOR_NOW!"); 
	};

	/*while (arg != nullptr) {
		switch (arg -> car -> type) {
			case INT: {
				args .push_back (
					*get_object_data <int> (arg -> car));
				break;
			}
			case CONS: {
				args .push_back (
					*get_object_data <int> (
						interpret (
							get_object_data <cons_cell> (arg -> car)) .get ()));
				break;
			}
			default: {
				throw std :: runtime_error ("SUM_SUPPORT_ONLY_INT_FOR_NOW!");
				break;
			}
		};

		arg = arg -> cdr;
	};*/

	return std :: unique_ptr <object> (create_object (type :: INT, new int (result)));
}

std :: unique_ptr <object> interpret (cons_cell* cells) {
	if (cells == nullptr) // TODO test null-check
		return nullptr;
	
	object* command = cells -> car;

	if (command -> type != type :: SYMBOL)
		throw std :: runtime_error ("CAR_IS_NOT_SYMBOL IN " + print (cells));

	if ("sum" == *get_object_data <std :: string> (command)) // FIXME not only sum!!
		return sum (cells -> cdr);
	else
	if ("mul" == *get_object_data <std :: string> (command))
		return mul (cells -> cdr);
	else
		throw std :: runtime_error ("UNKNOWN_COMMAND: " + *get_object_data <std :: string> (command));
};

int main () {
	try {
		object* one = create_object (type :: INT, new int (1));
		object* anotherOne = create_object (type :: INT, new int (1));
		object* two = create_object (type :: INT, new int (2));
		object* anotherTwo = create_object (type :: INT, new int (2));
		object* sum = create_object (type :: SYMBOL, new std :: string ("sum"));
		object* anotherSum = create_object (type :: SYMBOL, new std :: string ("sum"));
		object* mul = create_object (type :: SYMBOL, new std :: string ("mul"));
		
		cons_cell* test = cons (create_object (type :: CONS, cons (anotherSum, cons (anotherTwo, cons (anotherOne, nullptr)))), nullptr);
		test = cons (two, test);
		test = cons (mul, test);
		//test = cons (cell_type :: CONS, test, nullptr);

		std :: cout << "structure : " << print (test) << std :: endl;
		//std :: cout << to_dot_struct (test) << std :: endl;
		std :: cout << "result : " << print (interpret (test) .get ()) << std :: endl;

		delete test;
	} catch (std :: exception& e) {
		std :: cout << "Exception thrown: " << e .what () << std :: endl;
	};
	
	return EXIT_SUCCESS;
};