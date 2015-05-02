#include "evaluate.hpp"

#include <iostream>

int check_args_length (std :: shared_ptr <object> args, int min_length, int max_length) { // FIXME put arguments to array?
	auto arg = args;
	int arg_position = 0;

	while (arg != NIL) {
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
	std :: cout << "evaluating quote with: " << print (arguments) << std :: endl;
	
	if (check_args_length (arguments, 1, 1) != 0)
		throw std :: runtime_error ("Failed to 'quote', expected 1 parameter!"); // FIXME show error context on level up!

	return get_object_data <cons_cell> (arguments) -> car;
}

std :: shared_ptr < object > car (std :: shared_ptr <object> arguments, const Context& context) {
	std :: cout << "evaluating car with: " << print (arguments) << std :: endl;
	
	if (check_args_length (arguments, 1, 1) != 0)
		throw std :: runtime_error ("Failed to 'car', expected 1 parameter!"); // FIXME show error context on level up!

	auto evaluated_obj = evaluate (get_object_data <cons_cell> (arguments) -> car, context);

	if (evaluated_obj -> type != type :: CONS)
		throw std :: runtime_error ("Failed to 'car', expected list as parameter!");

	auto list = get_object_data < cons_cell > (evaluated_obj);
	
	return list -> car;
}

std :: shared_ptr < object > cdr (std :: shared_ptr <object> arguments, const Context& context) {
	std :: cout << "evaluating cdr with: " << print (arguments) << std :: endl;
	
	if (check_args_length (arguments, 1, 1) != 0)
		throw std :: runtime_error ("Failed to 'cdr', expected 1 parameter!"); // FIXME show error context on level up!

	auto evaluated_obj = evaluate (get_object_data <cons_cell> (arguments) -> car, context);

	if (evaluated_obj -> type != type :: CONS)
		throw std :: runtime_error ("Failed to 'cdr', expected list as parameter!");
	
	return get_object_data < cons_cell > (evaluated_obj) -> cdr;
}

std :: shared_ptr < object > cons (std :: shared_ptr <object> arguments, const Context& context) {
	std :: cout << "evaluating cons with: " << print (arguments) << std :: endl;
	
	if (check_args_length (arguments, 2, 2) != 0)
		throw std :: runtime_error ("Failed to 'cons', expected 2 parameters!"); // FIXME show error context on level up!

	auto e1 = evaluate (get_object_data <cons_cell> (arguments) -> car, context);
	auto e2 = evaluate (get_object_data <cons_cell> (get_object_data <cons_cell> (arguments) -> cdr) -> car, context);

	return cons (e1, e2);
}

bool is_equal (std :: shared_ptr <object> a, std :: shared_ptr <object> b) {
	if (a == b)
		return true;

	if (a -> type != b -> type)
		return false;

	if (a -> type == type :: CONS)
		return is_equal (get_object_data <cons_cell> (a) -> car, get_object_data <cons_cell> (b) -> car)
			and is_equal (get_object_data <cons_cell> (a) -> cdr, get_object_data <cons_cell> (b) -> cdr);

	if (a -> type == type :: INT)
		return *get_object_data < int > (a) == *get_object_data < int > (b);

	if (a -> type == type :: SYMBOL)
		return *get_object_data < std :: string > (a) == *get_object_data < std :: string > (b);

	throw std :: runtime_error ("FAILED TO CHECK EQUALITY OF UNKNOWN TYPE!");
}

std :: shared_ptr < object > equal (std :: shared_ptr <object> arguments, const Context& context) {
	std :: cout << "evaluating equal with: " << print (arguments) << std :: endl;

	if (check_args_length (arguments, 2, 2) != 0)
		throw std :: runtime_error ("Failed to 'equal', expected 2 parameters!"); // FIXME show error context on level up!

	auto e1 = evaluate (get_object_data <cons_cell> (arguments) -> car, context);
	auto e2 = evaluate (get_object_data <cons_cell> (get_object_data <cons_cell> (arguments) -> cdr) -> car, context);

	return is_equal (e1, e2) ? create_object (type :: SYMBOL, std :: shared_ptr <std :: string> (new std :: string ("T")))
									 : NIL;
}

std :: shared_ptr < object > atom (std :: shared_ptr <object> arguments, const Context& context) {
	std :: cout << "evaluating atom with: " << print (arguments) << std :: endl;
	
	if (check_args_length (arguments, 1, 1) != 0)
		throw std :: runtime_error ("Failed to 'atom', expected 1 parameters!"); // FIXME show error context on level up!

	auto e = evaluate (get_object_data <cons_cell> (arguments) -> car, context);

	return is_atom (e) ? create_object (type :: SYMBOL, std :: shared_ptr <std :: string> (new std :: string ("T")))
							 : NIL;
}

std :: shared_ptr <object> cond (std :: shared_ptr <object> arguments, const Context& context) { // TODO check for structure pass (COND (p e) ... (p e))
	std :: cout << "evaluating cond with: " << print (arguments) << std :: endl;
	
	auto cond_obj = arguments;
	
	while (cond_obj != NIL) {
		auto list = get_object_data <cons_cell> (cond_obj) -> car;
		
		if (list -> type != CONS)
			throw std :: runtime_error ("Failed to COND, condition clause must be a list");
		
		auto cond = evaluate (get_object_data <cons_cell> (list) -> car, context);

		if (cond != NIL)
			return evaluate (get_object_data <cons_cell> (get_object_data <cons_cell> (list) -> cdr) -> car, context);
		
		cond_obj = get_object_data <cons_cell> (cond_obj) -> cdr;
	}
	
	return NIL;
}

std :: shared_ptr <object> lambda (std :: shared_ptr <object> lambda_args, std :: shared_ptr <object> arguments, const Context& outer_context) {
	std :: cout << "evaluating lambda with: " << print (lambda_args) << ", " << print (arguments) << std :: endl;

	Context new_context (&outer_context);
	
	if (check_args_length (lambda_args, 2, 2) != 0)
		throw std :: runtime_error ("Failed to 'lambda', expected 2 parameters!"); // FIXME show error context on level up!

	// FIXME check if lambda_list is really a list of atom symbols!
	auto lambda_list = get_object_data <cons_cell> (lambda_args) -> car;
	auto lambda_expr = get_object_data <cons_cell> (get_object_data <cons_cell> (lambda_args) -> cdr) -> car;
	
	while (lambda_list != NIL and arguments != NIL) {
		auto variable_name = *get_object_data <std :: string> (get_object_data <cons_cell> (lambda_list) -> car);
		auto variable_value = evaluate (get_object_data <cons_cell> (arguments) -> car, outer_context);

		/*std :: pair < std :: string, std :: shared_ptr <object> > new_entry (variable_name, variable_value);
		auto existing_one = new_context .find (variable_name);
		if (existing_one != new_context .end()) {
			new_context .erase (existing_one);
			std :: cout << "erased original value!" << std :: endl;
		}
		auto new_one = new_context .insert (new_entry);
		std :: cout << "inserted " << variable_name << " with value of " << print ((new_one . first) -> second) << std :: endl;*/
		
		new_context .add_symbol (variable_name, variable_value);
		
		lambda_list = get_object_data <cons_cell> (lambda_list) -> cdr;
		arguments = get_object_data <cons_cell> (arguments) -> cdr;
	}
	
	if (lambda_list != arguments)
		throw std :: runtime_error ("FAILED_TO_APPLY_LAMBDA: wrong number of arguments supplied.");

	return evaluate (lambda_expr, new_context);
}

std :: shared_ptr <object> label (std :: shared_ptr <object> this_label, std :: shared_ptr <object> label_args, std :: shared_ptr <object> lambda_args, const Context& outer_context) {
	std :: cout << "evaluating label: " << print (this_label) << std :: endl;

	if (check_args_length (label_args, 2, 2) != 0)
		throw std :: runtime_error ("Failed to 'label', expected 2 parameters!"); // FIXME show error context on level up!
	
	auto lambda_name = get_object_data <cons_cell> (label_args) -> car; // FIXME check for atom symbol
	auto lambda_cons = get_object_data <cons_cell> (get_object_data <cons_cell> (label_args) -> cdr) -> car;
	
	Context new_context (&outer_context);
	auto lambda_name_str = *get_object_data <std :: string> (lambda_name);
	new_context .add_symbol (lambda_name_str, get_object_data <cons_cell> (this_label) -> car);
	
	return lambda (get_object_data <cons_cell> (lambda_cons) -> cdr, lambda_args, new_context);
}

std :: shared_ptr < object > evaluate (std :: shared_ptr < object > obj, const Context& context) {
	if (obj == NIL)
		throw std :: runtime_error ("PASSED_EMPTY_FORM_TO_EVAL");

	if (obj -> type == type :: SYMBOL)
		return context .lookup (*get_object_data <std :: string> (obj));

	if (obj -> type != type :: CONS)
		return obj;
	
	auto list = get_object_data <cons_cell> (obj);
	auto function = list -> car;

	if (function != NIL and function -> type == type :: CONS) { // check for lambda or label
		auto car = get_object_data <cons_cell> (function) -> car;
		if (car != NIL and car -> type == type :: SYMBOL) {
			if (*get_object_data <std :: string> (car) == "lambda")
				return lambda (get_object_data <cons_cell> (function) -> cdr, list -> cdr, context);
		
			if (*get_object_data <std :: string> (car) == "label")
				return label (obj, get_object_data <cons_cell> (function) -> cdr, list -> cdr, context);
		}
	}

	if (function -> type != type :: SYMBOL)
		throw std :: runtime_error (print (function) + " IS_NOT_FUNCTION_SYMBOL_IN " + print (obj));

	if ("quote" == *get_object_data <std :: string> (function))
		return quote (list -> cdr);

	if ("car" == *get_object_data <std :: string> (function))
		return car (list -> cdr, context);

	if ("cdr" == *get_object_data <std :: string> (function))
		return cdr (list -> cdr, context);

	if ("cons" == *get_object_data <std :: string> (function))
		return cons (list -> cdr, context);

	if ("equal" == *get_object_data <std :: string> (function))
		return equal (list -> cdr, context);
	
	if ("atom" == *get_object_data <std :: string> (function))
		return atom (list -> cdr, context);
	
	if ("cond" == *get_object_data <std :: string> (function))
		return cond (list -> cdr, context);
	
	auto symbol_value = context .lookup (*get_object_data <std :: string> (function));

	return evaluate (cons (symbol_value, list -> cdr), context);

	throw std :: runtime_error ("UNKNOWN_FUNCTION: " + *get_object_data <std :: string> (function));
};
