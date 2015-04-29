#include "cons_cell.hpp"

#include <iostream>
#include <sstream>

std :: shared_ptr < const cons_cell > cons (std :: shared_ptr < object > obj, std :: shared_ptr < const cons_cell > rest_cells) {
	return std :: shared_ptr < const cons_cell > (new cons_cell { obj, rest_cells });
};

std :: string print (std :: shared_ptr < const cons_cell > cells) {
	if (not cells)
		throw std :: runtime_error ("NULLPTR");
	
	auto cell = cells;
	std :: string result = "";

	while (cell) {
		result += " " + print (cell -> car) + " ";

		cell = cell -> cdr;
	};

	return "(" + result + ")";
};