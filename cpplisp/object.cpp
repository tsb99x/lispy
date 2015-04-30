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

std :: string print (std :: shared_ptr < object > object) {
	if (not object)
		throw std :: runtime_error ("Failed to print nil object!");
	
	switch (object -> type) {
		case INT:
			return std :: to_string (*get_object_data <int> (object));
		break;
		case SYMBOL:
			return *get_object_data <std :: string> (object);
		break;
		case CONS: {
			auto cell = object;
			std :: string result = " ";

			while (cell) {
				result += print (get_object_data <cons_cell> (cell) -> car) + " ";

				cell = get_object_data <cons_cell> (cell) -> cdr;
			};
			
			return "(" + result + ")";
		}
		default:
			throw std :: runtime_error ("PARSE_ERROR");
		break;
	}
};