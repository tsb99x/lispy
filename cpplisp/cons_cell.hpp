#ifndef CONS_CELL_HPP
#define CONS_CELL_HPP

#include "object.hpp"

struct cons_cell { // FIXME cons only for navigation, not interpretation
	std :: shared_ptr < object > car; // copy address register, head, first // FIXME const!
	std :: shared_ptr < const cons_cell > cdr; // copy decrement register, tail, rest
};

std :: shared_ptr < const cons_cell > cons (std :: shared_ptr < object >, std :: shared_ptr < const cons_cell >);
std :: string print (std :: shared_ptr < const cons_cell >);

#endif