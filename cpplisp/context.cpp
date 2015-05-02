#include "context.hpp"

#include <utility>

#include <iostream>

Context :: Context (const Context* outer_context) : outer_context (outer_context) { };

void Context :: add_symbol (const std :: string& symbol_name, const std :: shared_ptr <object> symbol_value) {
	auto new_entry = std :: make_pair (symbol_name, symbol_value);
	std :: cout << "added: " << symbol_name << " with value: " << print (symbol_value) << std :: endl;
	symbols .insert (new_entry); // FIXME check for existance error
};

// erase example
/*std :: pair < std :: string, std :: shared_ptr <object> > new_entry (variable_name, variable_value);
auto existing_one = new_context .find (variable_name);
if (existing_one != new_context .end()) {
	new_context .erase (existing_one);
	std :: cout << "erased original value!" << std :: endl;
}
auto new_one = new_context .insert (new_entry);*/

std :: shared_ptr <object> Context :: lookup (const std :: string& symbol_name) const {
	auto symbol_iter = symbols .find (symbol_name);

	if (symbol_iter == symbols .end ()) {
		std :: cout << "can't find " << symbol_name << " in current context, trying parent" << std :: endl;
		
		if (outer_context == nullptr)
			throw std :: runtime_error ("Failed to lookup symbol '" + symbol_name + "'!");
		
		return outer_context -> lookup (symbol_name);
	}

	std :: cout << "found " << symbol_name << " with value " << print (symbol_iter -> second) << std :: endl;

	return symbol_iter -> second;
	
	/*auto symbol_value = context .find (*get_object_data <std :: string> (obj));
	if (symbol_value == context .end())
		throw std :: runtime_error ("FAILED TO EVALUATE SYMBOL " + *get_object_data <std :: string> (obj));
	std :: cout << "substituting " << print (symbol_value -> second) << " for " << symbol_value -> first << std :: endl;
	return symbol_value -> second;*/
}