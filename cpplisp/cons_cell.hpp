#ifndef CONS_CELL_HPP
#define CONS_CELL_HPP

#include "object.hpp"

struct cons_cell {
	std :: shared_ptr < object > car; // copy address register, head, first // FIXME const!
	std :: shared_ptr < object > cdr; // copy decrement register, tail, rest
};

std :: shared_ptr < object > cons (std :: shared_ptr < object >, std :: shared_ptr < object >);

#endif