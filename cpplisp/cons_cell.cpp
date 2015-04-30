#include "cons_cell.hpp"

std :: shared_ptr < object > cons (std :: shared_ptr < object > car, std :: shared_ptr < object > cdr) {
	return create_object (type :: CONS, std :: shared_ptr < const cons_cell > (new cons_cell { car, cdr }));
};