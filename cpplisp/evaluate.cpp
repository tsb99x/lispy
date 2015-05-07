#include "evaluate.hpp"

// FIXME put arguments to array?
// FIXME return arguments count
// FIXME throw exception here
int check_args_length (std :: shared_ptr <object> args, int min_length, int max_length) {
	auto arg = args;
	int arg_position = 0;

	while (arg != NIL) {
		arg_position += 1;

		if (arg_position > max_length)
			return 1;
		
		arg = cdr (arg);
	}
	
	if (arg_position < min_length)
		return -1;
	
	return 0;
}

/**
	Rule #1 (QUOTE e).
	Example:

		(quote A)

		=> A
*/

std :: shared_ptr < object > quote (const std :: shared_ptr < object >& arguments) {
	if (check_args_length (arguments, 1, 1) != 0)
		throw std :: runtime_error ("Failed to 'quote', expected 1 parameter!"); // FIXME show error context on level up!

	return car (arguments);
}

/**
	Rule #2 (CAR e).
	Example:

		(car (quote (A B C)))

		=> A
*/

std :: shared_ptr < object > car (const std :: shared_ptr < object >& arguments, Context& context) {
	if (check_args_length (arguments, 1, 1) != 0)
		throw std :: runtime_error ("Failed to 'car', expected 1 parameter!"); // FIXME show error context on level up!

	auto evaluated_obj = evaluate (car (arguments), context);

	if (not is_cons (evaluated_obj))
		throw std :: runtime_error ("Failed to 'car', expected list as parameter!");

	return car (evaluated_obj);
}

/**
	Rule #3 (CDR e).
	Example:

		(cdr (quote (A B C)))

		=> (B C)
*/

std :: shared_ptr < object > cdr (const std :: shared_ptr < object >& arguments, Context& context) {	
	if (check_args_length (arguments, 1, 1) != 0)
		throw std :: runtime_error ("Failed to 'cdr', expected 1 parameter!"); // FIXME show error context on level up!

	auto evaluated_obj = evaluate (car (arguments), context);

	if (not is_cons (evaluated_obj))
		throw std :: runtime_error ("Failed to 'cdr', expected list as parameter!");

	return cdr (evaluated_obj);
}

/**
	Rule #4 (CONS e1 e2).
	Example:

		(cons
			(quote A)
			(quote (B C)))

		=> (A B C)
*/

std :: shared_ptr < object > cons (const std :: shared_ptr < object >& arguments, Context& context) {	
	if (check_args_length (arguments, 2, 2) != 0)
		throw std :: runtime_error ("Failed to 'cons', expected 2 parameters!"); // FIXME show error context on level up!

	auto e1 = evaluate (car (arguments), context);
	auto e2 = evaluate (car (cdr (arguments)), context);

	return cons (e1, e2);
}

bool is_equal (const std :: shared_ptr < object >& a, const std :: shared_ptr < object >& b) {
	if (a == b)
		return true;

	if (a -> type != b -> type)
		return false;

	if (a -> type == type :: CONS)
		return is_equal (car (a), car (b))
			and is_equal (cdr (a), cdr (b));

	if (a -> type == type :: INT)
		return *std :: static_pointer_cast < const int > (a -> data) == *std :: static_pointer_cast < const int > (b -> data);

	if (a -> type == type :: SYMBOL)
		return *std :: static_pointer_cast < const std :: string > (a -> data) == *std :: static_pointer_cast < const std :: string > (b -> data);

	throw std :: runtime_error ("FAILED TO CHECK EQUALITY OF UNKNOWN TYPE!");
}

/**
	Rule #5 (EQUAL e1 e2).
	Example:

		(equal
			(car (quote (A B)))
			(quote A))

		=> T
*/

std :: shared_ptr < object > equal (const std :: shared_ptr < object >& arguments, Context& context) {
	if (check_args_length (arguments, 2, 2) != 0)
		throw std :: runtime_error ("Failed to 'equal', expected 2 parameters!"); // FIXME show error context on level up!

	auto e1 = evaluate (car (arguments), context);
	auto e2 = evaluate (car (cdr (arguments)), context);

	return is_equal (e1, e2) ? create_object (type :: SYMBOL, std :: make_shared <std :: string> ("T"))
									 : NIL;
}

/**
	Rule #6 (ATOM e).
	Example:

		(atom A)

		=> T
*/

std :: shared_ptr < object > atom (const std :: shared_ptr < object >& arguments, Context& context) {	
	if (check_args_length (arguments, 1, 1) != 0)
		throw std :: runtime_error ("Failed to 'atom', expected 1 parameters!"); // FIXME show error context on level up!

	auto e = evaluate (car (arguments), context);

	return is_atom (e) ? create_object (type :: SYMBOL, std :: make_shared <std :: string> ("T"))
							 : NIL;
}

/**
	Rule #7 (COND (p1 e1) ... (pn en)).
	Example:

		(cond
			((atom (quote A)) (quote B))
			((quote T) (quote C)))

		=> B
*/

std :: shared_ptr <object> cond (const std :: shared_ptr < object >& arguments, Context& context) { // TODO check for structure pass (COND (p e) ... (p e))

	auto cond_obj = arguments;
	
	while (cond_obj != NIL) {
		auto list = car (cond_obj);
		
		if (list -> type != type :: CONS)
			throw std :: runtime_error ("Failed to COND, condition clause must be a list");
		
		auto cond = evaluate (car (list), context);

		if (cond != NIL)
			return evaluate (car (cdr (list)), context);
		
		cond_obj = cdr (cond_obj);
	}
	
	return NIL;
}

/**
	Rule #9 ((LAMBDA (v1 ... vn) e) e1 ... en).
	Example:

		((lambda (X Y)
			(cond (car X) Y))
		 (quote (A B))
		 (cdr (quote (C D))))

		=> (A D)
*/

std :: shared_ptr <object> lambda (const std :: shared_ptr < object >& lambda_args, std :: shared_ptr <object> arguments, Context& outer_context) {
	Context new_context (&outer_context);
	
	if (check_args_length (lambda_args, 2, 2) != 0)
		throw std :: runtime_error ("Failed to 'lambda', expected 2 parameters!"); // FIXME show error context on level up!

	// FIXME check if lambda_list is really a list of atom symbols!
	auto lambda_list = car (lambda_args);
	auto lambda_expr = car (cdr (lambda_args));
	
	while (lambda_list != NIL and arguments != NIL) {
		auto variable_name = *std :: static_pointer_cast < const std :: string > (car (lambda_list) -> data);
		auto variable_value = evaluate (car (arguments), outer_context);
		
		new_context .add_symbol (variable_name, variable_value);
		
		lambda_list = cdr (lambda_list);
		arguments = cdr (arguments);
	}
	
	if (lambda_list != arguments)
		throw std :: runtime_error ("FAILED_TO_APPLY_LAMBDA: wrong number of arguments supplied.");

	return evaluate (lambda_expr, new_context);
}

/**
	Rule #10 ((LABEL f (LAMBDA (v1 ... vn) e)) e1 ... en).
	Example:

		((label ff
			(lambda (x)
				(cond
					((atom x) x)
					((quote t) (ff (car x))))))
		(quote ((A B) C)))

		=> A
*/

std :: shared_ptr <object> label (const std :: shared_ptr < object > this_label, const std :: shared_ptr < object >& label_args, const std :: shared_ptr < object >& lambda_args, Context& outer_context) {
	if (check_args_length (label_args, 2, 2) != 0)
		throw std :: runtime_error ("Failed to 'label', expected 2 parameters!"); // FIXME show error context on level up!
	
	auto lambda_name = car (label_args); // FIXME check for atom symbol
	auto lambda_cons = car (cdr (label_args));
	
	Context new_context (&outer_context);
	auto lambda_name_str = *std :: static_pointer_cast < const std :: string > (lambda_name -> data);
	new_context .add_symbol (lambda_name_str, car (this_label));

	return lambda (cdr (lambda_cons), lambda_args, new_context);
}

/**
	In general sense, `(defun f (p1 ... pn) e)` is equal to `f = (label f (lambda (p1 ... pn) e))`.
*/

std :: shared_ptr <object> defun (const std :: shared_ptr < object > args, Context& context) {
	// FIXME check if args is function!
	
	auto func_name = car (args);
	auto func_parameters_and_expr = cdr (args);
	
	auto lambda_symbol = create_object (type :: SYMBOL, std :: make_shared <std::string> ("lambda"));
	auto label_symbol = create_object (type :: SYMBOL, std :: make_shared <std::string> ("label"));

	auto new_lambda = cons (lambda_symbol, func_parameters_and_expr);
	auto new_label = cons (label_symbol, cons (func_name, cons (new_lambda, NIL)));

	context .add_symbol (*std :: static_pointer_cast < const std :: string > (func_name -> data), new_label);
	
	return func_name;
}

std :: shared_ptr < object > evaluate (const std :: shared_ptr < object >& obj, Context& context) {
	if (obj == NIL)
		throw std :: runtime_error ("PASSED_EMPTY_FORM_TO_EVAL");

	/**
		Rule #8, atom value.
	*/

	if (obj -> type == type :: SYMBOL)
		return context .lookup (*std :: static_pointer_cast < const std :: string > (obj -> data));

	if (obj -> type != type :: CONS)
		return obj;

	auto function = car (obj);

	if (function != NIL and function -> type == type :: CONS) { // check for lambda or label
		auto first = car (function);
		if (first != NIL and first -> type == type :: SYMBOL) {
			if (*std :: static_pointer_cast < const std :: string > (first -> data) == "lambda")
				return lambda (cdr (function), cdr (obj), context);

			if (*std :: static_pointer_cast < const std :: string > (first -> data) == "label")
				return label (obj, cdr (function), cdr (obj), context);
		}
	}

	if (function -> type != type :: SYMBOL)
		throw std :: runtime_error (print (function) + " IS_NOT_FUNCTION_SYMBOL_IN " + print (obj));

	auto func_name = *std :: static_pointer_cast < const std :: string > (function -> data);

	if (func_name == "quote") // FIXME define elementary operators as permanent?
		return quote (cdr (obj));

	if (func_name == "car")
		return car (cdr (obj), context);

	if (func_name == "cdr")
		return cdr (cdr (obj), context);

	if (func_name == "cons")
		return cons (cdr (obj), context);

	if (func_name == "equal")
		return equal (cdr (obj), context);
	
	if (func_name == "atom")
		return atom (cdr (obj), context);
	
	if (func_name == "cond")
		return cond (cdr (obj), context);
	
	if (func_name == "defun")
		return defun (cdr (obj), context);
	
	auto symbol_value = context .lookup (func_name);

	return evaluate (cons (symbol_value, cdr (obj)), context);
};
