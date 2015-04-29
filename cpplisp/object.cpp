#include "object.hpp"

#include <iostream>
#include <sstream>

#include "cons_cell.hpp"

object :: ~object() {
	switch (type) {
		case INT:
			//std :: cout << "deleting INT: " << *get_object_data <int> (this) << std :: endl;
			delete get_object_data <int> (this);
		break;
		case SYMBOL:
			//std :: cout << "deleting SYMBOL: " << *get_object_data <std :: string> (this) << std :: endl;
			delete get_object_data <std :: string> (this);
		break;
		case CONS:
			//std :: cout << "deleting CONS_CELL" << std :: endl;
			delete get_object_data <cons_cell> (this);
		break;
		default:
			throw std :: runtime_error ("UNKNOWN_OBJECT_TYPE");
		break;
	};
};

object* create_object (type type, void* data) {
	return new object { type, data };
};

std :: string print (object* object) {
	if (object == nullptr)
		throw "NULLPTR";
	
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

std :: string to_dot_struct (object* object) {
	if (object == nullptr)
		return "";
	
	std :: stringstream ss;
	ss << "o" << object << "[label=\"<type> " << object -> type << " | <data> data\"];" << std :: endl;
	ss << "o" << object << ":data -> o" << object -> data << ";" << std :: endl;
	if (object -> type == CONS)
		ss << to_dot_struct (get_object_data <cons_cell> (object));
	else
		ss << "o" << object -> data << "[label=\"" << print (object) << "\"];" << std :: endl;
	return ss .str ();
};