#ifndef PARSER_HPP
#define PARSER_HPP

#include "cons_cell.hpp"

#include <list>

std :: list <std :: string> tokenize (const std :: string& sourceCode);
const int parens_balance (const std :: list <std :: string>& tokens);
std :: shared_ptr < object > build_s_expr (std :: list <std :: string>& tokens);

#endif