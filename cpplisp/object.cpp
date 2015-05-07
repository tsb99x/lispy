#include "object.hpp"

#include <iostream>
#include <sstream>

/*object :: object (const object_type & type, const void * data) :
	 type (type),
	 data (data)
{ }

object :: ~object () noexcept {
	if (type == object_type :: INT)
		delete static_cast < const int * > (data);
	if (type == object_type :: SYMBOL)
		delete static_cast < const std :: string * > (data);
	if (type == object_type :: CONS)
		delete static_cast < const cons_cell * > (data);
}

const object_type & object :: get_type () {
	return type;
}*/

std :: shared_ptr < object > cons (std :: shared_ptr < object > car, std :: shared_ptr < object > cdr) {
	return create_object (type :: CONS, std :: shared_ptr < const cons_cell > (new cons_cell { car, cdr }));
};

bool is_atom (std :: shared_ptr <object> a) {
	return a == NIL || a -> type != type :: CONS;
}

bool is_cons (std :: shared_ptr <object> a) {
	return a -> type == type :: CONS;
};

std :: shared_ptr < object > car (std :: shared_ptr < object > pair) {
	return std :: static_pointer_cast < const cons_cell > (pair -> data) -> car;
}

std :: shared_ptr < object > cdr (std :: shared_ptr < object > pair) {
	return std :: static_pointer_cast < const cons_cell > (pair -> data) -> cdr;
}

std :: string print (std :: shared_ptr < object > object) {
	if (not object)
		throw std :: runtime_error ("Failed to print nil object!");
	
	switch (object -> type) {
		case type :: INT:
			return std :: to_string (*std :: static_pointer_cast < const int > (object -> data));
		break;
		case type :: SYMBOL:
			return *std :: static_pointer_cast < const std :: string > (object -> data);
		break;
		case type :: CONS: {
			auto cell = object;
			std :: string result = " ";

			while (cell != NIL) {
				result += print (car (cell)) + " ";

				if (not is_cons (cdr (cell))) {
					result += ". " + print (cdr (cell)) + " ";
					break;
				}
				
				cell = cdr (cell);
			};
			
			return "(" + result + ")";
		}
		default:
			throw std :: runtime_error ("PARSE_ERROR");
		break;
	}
};

std :: shared_ptr <object> NIL = cons (std :: shared_ptr <object> (), std :: shared_ptr <object> ());
