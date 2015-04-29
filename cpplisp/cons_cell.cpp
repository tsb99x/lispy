#include "cons_cell.hpp"

#include <iostream>
#include <sstream>

cons_cell :: ~cons_cell () {
	if (car != nullptr) {
		//std :: cout << "deleting CAR: " << print (car) << std :: endl;
		delete car;
	}
	if (cdr != nullptr) {
		//std :: cout << "deleting CDR: " << print (cdr) << std :: endl;
		delete cdr;
	}
};

cons_cell* cons (object* obj, cons_cell* rest_cells) {
	return new cons_cell { obj, rest_cells };
};

std :: string print (cons_cell* cells) {
	if (cells == nullptr)
		throw std :: runtime_error ("NULLPTR");
	
	cons_cell* cell = cells;
	std :: string result = "";

	while (cell != nullptr) {
		result += " " + print (cell -> car) + " ";

		cell = cell -> cdr;
	};

	return "(" + result + ")";
};

std :: string to_dot_struct (cons_cell* cell) {
	if (cell == nullptr)
		return "";
	
	std :: stringstream ss;
	ss << "o" << cell << "[label=\"<car> car | <cdr> cdr\"];" << std :: endl;
	ss << "o" << cell << ":car -> o" << cell -> car << ";" << std :: endl << to_dot_struct (cell -> car);
	ss << "o" << cell << ":cdr -> o" << cell -> cdr << ";" << std :: endl << to_dot_struct (cell -> cdr);
	return ss .str ();
};