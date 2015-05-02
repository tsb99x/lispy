#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <unordered_map>

#include "object.hpp"

class Context { // FIXME delete assign constructor, move constructor

	const Context* outer_context;
	std :: unordered_map < std :: string, std :: shared_ptr < object >> symbols;
	
public:

	Context () = delete;                               // default constructor
	Context (const Context* outer_context);            // constructor
	Context (const Context&) = delete;                 // copy constructor
	Context (Context&&) noexcept = delete;             // move constructor
	~Context () noexcept = default;                    // destructor

	Context& operator = (const Context&) = delete;     // copy assignment operator
	Context& operator = (Context&&) noexcept = delete; // move assignment operator

	void add_symbol (const std :: string& symbol_name, const std :: shared_ptr <object> symbol_value);
	std :: shared_ptr <object> lookup (const std :: string& symbol_name) const;
	
};

#endif