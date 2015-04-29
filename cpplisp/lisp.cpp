#include <iostream>
#include <vector>
#include <memory>

#include "object.hpp"
#include "cons_cell.hpp"

#include "parser.hpp"

/*char *string_to_chars (std :: string str) {
	char *res = new char [str .length () + 1];
	strcpy (res, str .c_str ());
	return res;
};*/

object* interpret (cons_cell* cells);

int check_args_length (cons_cell* args, int min_length, int max_length) {
	cons_cell* arg = args;
	int arg_position = 0;

	while (arg != nullptr) {
		arg_position += 1;
		
		if (arg_position > max_length)
			return 1;
		
		arg = arg -> cdr;
	}
	
	if (arg_position < min_length)
		return -1;
	
	return 0;
}

object* quote (cons_cell* arguments) {
	if (check_args_length (arguments, 1, 1) != 0)
		throw std :: runtime_error ("Failed to 'quote', expected 1 parameter!"); // FIXME show error context on level up!

	return copy_object (arguments -> car); // FIXME classical shared_ptr!
}

object* car (cons_cell* arguments) {
	if (check_args_length (arguments, 1, 1) != 0)
		throw std :: runtime_error ("Failed to 'car', expected 1 parameter!"); // FIXME show error context on level up!

	object* arg = arguments -> car;

	object* interpreted_obj = interpret (get_object_data <cons_cell> (arg)); // FIXME check if arg is CONS!

	if (interpreted_obj -> type != type :: CONS)
		throw std :: runtime_error ("Failed to 'car', expected list as parameter!");

	cons_cell* list = get_object_data <cons_cell> (interpreted_obj);

	object* result = copy_object (list -> car);

	delete interpreted_obj;

	return result;
}

object* sum (cons_cell* arguments) {
	int result = 0;

	for (cons_cell* arg = arguments; arg != nullptr; arg = arg -> cdr) {
		object* arg_object = arg -> car;
		bool is_created_instance = false;

		if (arg_object -> type == type :: CONS) {
			arg_object = interpret (get_object_data <cons_cell> (arg_object));
			is_created_instance = true;
		}

		if (arg_object -> type == type :: INT)
			result += (*get_object_data <int> (arg_object));
		else
			throw std :: runtime_error ("MUL_SUPPORT_ONLY_INT_FOR_NOW!");
		
		if (is_created_instance)
			delete arg_object;
	};

	return create_object (type :: INT, new int (result));
}

object* mul (cons_cell* arguments) {
	int result = 1;

	for (cons_cell* arg = arguments; arg != nullptr; arg = arg -> cdr) {
		object* arg_object = arg -> car;
		bool is_created_instance = false;

		if (arg_object -> type == type :: CONS) {
			arg_object = interpret (get_object_data <cons_cell> (arg_object));
			is_created_instance = true;
		}

		if (arg_object -> type == type :: INT)
			result *= (*get_object_data <int> (arg_object));
		else
			throw std :: runtime_error ("MUL_SUPPORT_ONLY_INT_FOR_NOW!");
		
		if (is_created_instance)
			delete arg_object;
	};

	return create_object (type :: INT, new int (result));
}

object* interpret (cons_cell* cells) {
	if (cells == nullptr) // TODO test null-check
		return nullptr;
	
	object* command = cells -> car;

	if (command -> type != type :: SYMBOL)
		throw std :: runtime_error ("CAR_IS_NOT_SYMBOL IN " + print (cells));

	if ("quote" == *get_object_data <std :: string> (command))
		return quote (cells -> cdr);
	else
	if ("car" == *get_object_data <std :: string> (command))
		return car (cells -> cdr);
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
		auto tokens = tokenize ("(car (quote (A B C)))");
		cons_cell* expr = build_s_expr (tokens);
		//cons_cell* expr = cons (create_object (type :: SYMBOL, new std :: string ("quote")), orig);
		std :: cout << "parsed expression : " << print (expr -> car) << std :: endl;
		object* result = interpret (get_object_data <cons_cell> (expr -> car));
		std :: cout << "interpret result : " << print (result) << std :: endl;
		delete result;
		delete expr;
	} catch (std :: exception& e) {
		std :: cout << "Exception thrown: " << e .what () << std :: endl;
	};
	
	return EXIT_SUCCESS;
};
