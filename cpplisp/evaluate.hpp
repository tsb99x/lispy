#ifndef EVALUATE_HPP
#define EVALUATE_HPP

#include "cons_cell.hpp"

#include <unordered_map>

using context = std :: unordered_map < std :: string, std :: shared_ptr < object > >;
std :: shared_ptr < object > evaluate (std :: shared_ptr < object >, context&);

#endif