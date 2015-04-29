#include "object.hpp"

#include <iostream>
#include <sstream>

#include "cons_cell.hpp"

std :: string print (std :: shared_ptr < object > object) {
	if (not object)
		throw std :: runtime_error ("NULLPTR");
	
	switch (object -> type) {
		case INT:
			return std :: to_string (*get_object_data <int> (object));
		break;
		case SYMBOL:
			return *get_object_data <std :: string> (object);
		break;
		case CONS:
			return print (get_object_data <cons_cell> (object));
		break;
		default:
			throw std :: runtime_error ("PARSE_ERROR");
		break;
	}
};