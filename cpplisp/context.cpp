#include "context.hpp"

#include <utility>

Context :: Context (const Context* outer_context) : outer_context (outer_context) { };

void Context :: add_symbol (const std :: string& symbol_name, const std :: shared_ptr <object>& symbol_value) {
	auto res = symbols .emplace (symbol_name, symbol_value);
	if (res .second == false)
		throw std :: runtime_error ("Failed to add new symbol to context: symbol '" + symbol_name + "' is already existing!");
};

std :: shared_ptr <object> Context :: lookup (const std :: string& symbol_name) const {
	auto symbol_iter = symbols .find (symbol_name);

	if (symbol_iter == symbols .end ()) {
		if (outer_context == nullptr)
			throw std :: runtime_error ("Failed to lookup symbol '" + symbol_name + "'!");
		
		return outer_context -> lookup (symbol_name);
	}

	return symbol_iter -> second;
}