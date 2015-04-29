#ifndef CONS_CELL_HPP
#define CONS_CELL_HPP

#include "object.hpp"

struct cons_cell { // FIXME cons only for navigation, not interpretation
	object* car; // copy address register, head, first
	cons_cell* cdr; // copy decrement register, tail, rest

	~cons_cell ();
};

cons_cell* cons (object*, cons_cell*);
std :: string print (cons_cell*);
std :: string to_dot_struct (cons_cell*);

#endif