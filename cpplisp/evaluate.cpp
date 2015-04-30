#include "evaluate.hpp"

int check_args_length (std :: shared_ptr <object> args, int min_length, int max_length) {
	auto arg = args;
	int arg_position = 0;

	while (arg) {
		arg_position += 1;
		
		if (arg_position > max_length)
			return 1;
		
		arg = get_object_data <cons_cell> (arg) -> cdr;
	}
	
	if (arg_position < min_length)
		return -1;
	
	return 0;
}

std :: shared_ptr < object > quote (std :: shared_ptr <object> arguments) {	
	if (check_args_length (arguments, 1, 1) != 0)
		throw std :: runtime_error ("Failed to 'quote', expected 1 parameter!"); // FIXME show error context on level up!

	return get_object_data <cons_cell> (arguments) -> car;
}

std :: shared_ptr < object > car (std :: shared_ptr <object> arguments) {
	if (check_args_length (arguments, 1, 1) != 0)
		throw std :: runtime_error ("Failed to 'car', expected 1 parameter!"); // FIXME show error context on level up!

	auto evaluated_obj = evaluate (get_object_data <cons_cell> (arguments) -> car);

	if (evaluated_obj -> type != type :: CONS)
		throw std :: runtime_error ("Failed to 'car', expected list as parameter!");

	auto list = get_object_data < cons_cell > (evaluated_obj);
	
	return list -> car;
}

std :: shared_ptr < object > cdr (std :: shared_ptr <object> arguments) {
	if (check_args_length (arguments, 1, 1) != 0)
		throw std :: runtime_error ("Failed to 'cdr', expected 1 parameter!"); // FIXME show error context on level up!

	auto evaluated_obj = evaluate (get_object_data <cons_cell> (arguments) -> car);

	if (evaluated_obj -> type != type :: CONS)
		throw std :: runtime_error ("Failed to 'cdr', expected list as parameter!");
	
	return get_object_data < cons_cell > (evaluated_obj) -> cdr;
}

std :: shared_ptr < object > cons (std :: shared_ptr <object> arguments) {
	if (check_args_length (arguments, 2, 2) != 0)
		throw std :: runtime_error ("Failed to 'cons', expected 2 parameters!"); // FIXME show error context on level up!

	auto e1 = evaluate (get_object_data <cons_cell> (arguments) -> car);
	auto e2 = evaluate (get_object_data <cons_cell> (get_object_data <cons_cell> (arguments) -> cdr) -> car);
	
	return cons (e1, e2);
}

std :: shared_ptr < object > equal (std :: shared_ptr <object> arguments) {
	if (check_args_length (arguments, 2, 2) != 0)
		throw std :: runtime_error ("Failed to 'equal', expected 2 parameters!"); // FIXME show error context on level up!

	auto e1 = evaluate (get_object_data <cons_cell> (arguments) -> car);
	auto e2 = evaluate (get_object_data <cons_cell> (arguments) -> cdr);
	
	return create_object (type :: CONS, cons (e1, e2));
}

/*std :: shared_ptr < object > sum (std :: shared_ptr <object> arguments) {
	int result = 0;

	for (auto arg = arguments; arg; arg = arg -> cdr) {
		auto arg_object = arg -> car;

		if (arg_object -> type == type :: CONS)
			arg_object = evaluate (get_object_data <cons_cell> (arg_object));

		if (arg_object -> type == type :: INT)
			result += (*get_object_data <int> (arg_object));
		else
			throw std :: runtime_error ("MUL_SUPPORT_ONLY_INT_FOR_NOW!");
	};

	return create_object (type :: INT, std :: shared_ptr <int> (new int (result)));
}*/

/*std :: shared_ptr < object > mul (std :: shared_ptr <object> arguments) {
	int result = 1;

	for (auto arg = arguments; arg; arg = arg -> cdr) {
		auto arg_object = arg -> car;

		if (arg_object -> type == type :: CONS)
			arg_object = evaluate (get_object_data <cons_cell> (arg_object));

		if (arg_object -> type == type :: INT)
			result *= (*get_object_data <int> (arg_object));
		else
			throw std :: runtime_error ("MUL_SUPPORT_ONLY_INT_FOR_NOW!");
	};

	return create_object (type :: INT, std :: shared_ptr <int> (new int (result)));
}*/

std :: shared_ptr < object > evaluate (std :: shared_ptr < object > obj) { // FIXME evaluate, not interpret!	
	if (not obj)
		throw std :: runtime_error ("PASSED_EMPTY_OBJECT_TO_EVAL");

	if (obj -> type != type :: CONS)
		return obj;
	
	auto list = get_object_data <cons_cell> (obj);
	auto command = list -> car;

	if (command -> type != type :: SYMBOL)
		throw std :: runtime_error ("CAR_IS_NOT_SYMBOL IN " + print (obj));

	if ("quote" == *get_object_data <std :: string> (command))
		return quote (list -> cdr);
	else
	if ("car" == *get_object_data <std :: string> (command))
		return car (list -> cdr);
	else
	if ("cdr" == *get_object_data <std :: string> (command))
		return cdr (list -> cdr);
	else
	if ("cons" == *get_object_data <std :: string> (command))
		return cons (list -> cdr);
	else
	if ("equal" == *get_object_data <std :: string> (command))
		return equal (list -> cdr);
	else
	/*if ("sum" == *get_object_data <std :: string> (command)) // FIXME not only sum!!
		return sum (list -> cdr);
	else
	if ("mul" == *get_object_data <std :: string> (command))
		return mul (list -> cdr);
	else*/
		throw std :: runtime_error ("UNKNOWN_COMMAND: " + *get_object_data <std :: string> (command));
};
