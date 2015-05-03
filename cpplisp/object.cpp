#include "object.hpp"

#include <iostream>
#include <sstream>

#include "cons_cell.hpp"

template < >
std :: shared_ptr < const cons_cell > get_object_data (std :: shared_ptr < object > object) {	
	if (object -> type != type :: CONS)
		throw std :: runtime_error ("Failed to get CONS from non-cons object!");
	
	return std :: static_pointer_cast < const cons_cell > (object -> data);
};

bool is_atom (std :: shared_ptr <object> a) {
	return a == NIL || a -> type != type :: CONS;
}

bool is_cons (std :: shared_ptr <object> a) {
	return a -> type == type :: CONS;
};

/*std :: shared_ptr < object > car (std :: shared_ptr < object > list) {
	return get_object_data < cons_cell > (list);
}*/

std :: string print (std :: shared_ptr < object > object) {
	if (not object)
		throw std :: runtime_error ("Failed to print nil object!");
	
	switch (object -> type) {
		case type :: INT:
			return std :: to_string (*get_object_data <int> (object));
		break;
		case type :: SYMBOL:
			return *get_object_data <std :: string> (object);
		break;
		case type :: CONS: {
			auto cell = object;
			std :: string result = " ";

			while (cell != NIL) {
				result += print (get_object_data <cons_cell> (cell) -> car) + " ";
				
				if (get_object_data <cons_cell> (cell) -> cdr -> type != type :: CONS) {
					result += ". " + print (get_object_data <cons_cell> (cell) -> cdr) + " ";
					break;
				}
				
				cell = get_object_data <cons_cell> (cell) -> cdr;
			};
			
			return "(" + result + ")";
		}
		default:
			throw std :: runtime_error ("PARSE_ERROR");
		break;
	}
};

std :: shared_ptr <object> NIL = cons (std :: shared_ptr <object> (), std :: shared_ptr <object> ());
