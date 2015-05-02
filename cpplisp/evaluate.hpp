#ifndef EVALUATE_HPP
#define EVALUATE_HPP

#include "cons_cell.hpp"
#include "context.hpp"

std :: shared_ptr < object > evaluate (std :: shared_ptr < object >, const Context&);

#endif