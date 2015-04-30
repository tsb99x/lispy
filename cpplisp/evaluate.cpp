#include "evaluate.hpp"

int check_args_length (std :: shared_ptr < const cons_cell > args, int min_length, int max_length) {
	auto arg = args;
	int arg_position = 0;

	while (arg) {
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

	auto arg = arguments -> car;
	auto interpreted_obj = interpret (get_object_data <cons_cell> (arg)); // FIXME check if arg is CONS!

	if (interpreted_obj -> type != type :: CONS)
		throw std :: runtime_error ("Failed to 'car', expected list as parameter!");

	auto list = get_object_data < cons_cell > (interpreted_obj);
	
	return list -> car;
}

std :: shared_ptr < object > cdr (std :: shared_ptr < const cons_cell > arguments) {
	if (check_args_length (arguments, 1, 1) != 0)
		throw std :: runtime_error ("Failed to 'cdr', expected 1 parameter!"); // FIXME show error context on level up!

	auto arg = arguments -> car;
	auto interpreted_obj = interpret (get_object_data <cons_cell> (arg)); // FIXME check if arg is CONS!

	if (interpreted_obj -> type != type :: CONS)
		throw std :: runtime_error ("Failed to 'cdr', expected list as parameter!");

	auto list = get_object_data < cons_cell > (interpreted_obj);
	
	return create_object (type :: CONS, list -> cdr);
}

std :: shared_ptr < object > cons (std :: shared_ptr < const cons_cell > arguments) {
	if (check_args_length (arguments, 2, 2) != 0)
		throw std :: runtime_error ("Failed to 'cons', expected 2 parameters!"); // FIXME show error context on level up!

	auto arg = arguments -> car;
	auto e1 = interpret (get_object_data <cons_cell> (arg)); // FIXME check if arg is CONS!
	
	auto arg2 = arguments -> cdr -> car;
	auto interpreted_obj_2 = interpret (get_object_data <cons_cell> (arg2)); // FIXME check if arg is CONS!

	if (interpreted_obj_2 -> type != type :: CONS)
		throw std :: runtime_error ("Failed to 'cons', expected list as second parameter!");

	auto e2 = get_object_data < cons_cell > (interpreted_obj_2);
	
	return create_object (type :: CONS, cons (e1, e2));
}

std :: shared_ptr < object > equal (std :: shared_ptr < const cons_cell > arguments) {
	if (check_args_length (arguments, 2, 2) != 0)
		throw std :: runtime_error ("Failed to 'equal', expected 2 parameters!"); // FIXME show error context on level up!

	auto arg = arguments -> car;
	auto e1 = interpret (get_object_data <cons_cell> (arg)); // FIXME check if arg is CONS!
	
	auto arg2 = arguments -> cdr -> car;
	auto interpreted_obj_2 = interpret (get_object_data <cons_cell> (arg2)); // FIXME check if arg is CONS!

	if (interpreted_obj_2 -> type != type :: CONS)
		throw std :: runtime_error ("Failed to 'cons', expected list as second parameter!");

	auto e2 = get_object_data < cons_cell > (interpreted_obj_2);
	
	return create_object (type :: CONS, cons (e1, e2));
}

std :: shared_ptr < object > sum (std :: shared_ptr <const cons_cell> arguments) {
	int result = 0;

	for (auto arg = arguments; arg; arg = arg -> cdr) {
		auto arg_object = arg -> car;

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

	for (auto arg = arguments; arg; arg = arg -> cdr) {
		auto arg_object = arg -> car;

		if (arg_object -> type == type :: CONS)
			arg_object = interpret (get_object_data <cons_cell> (arg_object));

		if (arg_object -> type == type :: INT)
			result *= (*get_object_data <int> (arg_object));
		else
			throw std :: runtime_error ("MUL_SUPPORT_ONLY_INT_FOR_NOW!");
	};

	return create_object (type :: INT, std :: shared_ptr <int> (new int (result)));
}

std :: shared_ptr < object > evaluate (std :: shared_ptr < const cons_cell > cells) { // FIXME evaluate, not interpret!
	if (not cells) // FIXME is real situation?
		return std :: shared_ptr < object > ();

	auto command = cells -> car;

	if (command -> type != type :: SYMBOL)
		throw std :: runtime_error ("CAR_IS_NOT_SYMBOL IN " + print (cells));

	if ("quote" == *get_object_data <std :: string> (command))
		return quote (cells -> cdr);
	else
	if ("car" == *get_object_data <std :: string> (command))
		return car (cells -> cdr);
	else
	if ("cdr" == *get_object_data <std :: string> (command))
		return cdr (cells -> cdr);
	else
	if ("cons" == *get_object_data <std :: string> (command))
		return cons (cells -> cdr);
	else
	if ("equal" == *get_object_data <std :: string> (command))
		return equal (cells -> cdr);
	else
	if ("sum" == *get_object_data <std :: string> (command)) // FIXME not only sum!!
		return sum (cells -> cdr);
	else
	if ("mul" == *get_object_data <std :: string> (command))
		return mul (cells -> cdr);
	else
		throw std :: runtime_error ("UNKNOWN_COMMAND: " + *get_object_data <std :: string> (command));
};
