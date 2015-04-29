#include <iostream>
#include <vector>
#include <memory>

#include "object.hpp"
#include "cons_cell.hpp"

#include "parser.hpp"

std :: shared_ptr < object > interpret (std :: shared_ptr < const cons_cell > cells);

int check_args_length (std :: shared_ptr < const cons_cell > args, int min_length, int max_length) {
	std :: shared_ptr < const cons_cell > arg = args;
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

std :: shared_ptr < object > quote (std :: shared_ptr < const cons_cell > arguments) {
	if (check_args_length (arguments, 1, 1) != 0)
		throw std :: runtime_error ("Failed to 'quote', expected 1 parameter!"); // FIXME show error context on level up!

	return arguments -> car;
}

std :: shared_ptr < object > car (std :: shared_ptr < const cons_cell > arguments) {
	if (check_args_length (arguments, 1, 1) != 0)
		throw std :: runtime_error ("Failed to 'car', expected 1 parameter!"); // FIXME show error context on level up!

	std :: shared_ptr < object > arg = arguments -> car;

	std :: shared_ptr < object > interpreted_obj = interpret (get_object_data <cons_cell> (arg)); // FIXME check if arg is CONS!

	if (interpreted_obj -> type != type :: CONS)
		throw std :: runtime_error ("Failed to 'car', expected list as parameter!");

	auto list = get_object_data < cons_cell > (interpreted_obj);
	
	return list -> car;
}

std :: shared_ptr < object > sum (std :: shared_ptr <const cons_cell> arguments) {
	int result = 0;

	for (std :: shared_ptr <const cons_cell> arg = arguments; arg != nullptr; arg = arg -> cdr) {
		std :: shared_ptr <object> arg_object = arg -> car;

		if (arg_object -> type == type :: CONS)
			arg_object = interpret (get_object_data <cons_cell> (arg_object));

		if (arg_object -> type == type :: INT)
			result += (*get_object_data <int> (arg_object));
		else
			throw std :: runtime_error ("MUL_SUPPORT_ONLY_INT_FOR_NOW!");
	};

	return create_object (type :: INT, std :: shared_ptr <int> (new int (result)));
}

std :: shared_ptr < object > mul (std :: shared_ptr <const cons_cell> arguments) {
	int result = 1;

	for (std :: shared_ptr <const cons_cell> arg = arguments; arg != nullptr; arg = arg -> cdr) {
		std :: shared_ptr <object> arg_object = arg -> car;

		if (arg_object -> type == type :: CONS)
			arg_object = interpret (get_object_data <cons_cell> (arg_object));

		if (arg_object -> type == type :: INT)
			result *= (*get_object_data <int> (arg_object));
		else
			throw std :: runtime_error ("MUL_SUPPORT_ONLY_INT_FOR_NOW!");
	};

	return create_object (type :: INT, std :: shared_ptr <int> (new int (result)));
}

std :: shared_ptr < object > interpret (std :: shared_ptr < const cons_cell > cells) {
	if (cells == nullptr) // TODO test null-check
		return nullptr;

	std :: shared_ptr <object> command = cells -> car;

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
		auto expr = build_s_expr (tokens);
		auto result = interpret (get_object_data <cons_cell> (expr -> car));

		std :: cout << "parsed expression : " << print (expr -> car) << std :: endl;
		std :: cout << "interpret result : " << print (result) << std :: endl;
	} catch (std :: exception& e) {
		std :: cout << "Exception thrown: " << e .what () << std :: endl;
	};
	
	return EXIT_SUCCESS;
};
