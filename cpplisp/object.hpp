#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>

/*enum class object_type { // add string
	INT,
	SYMBOL,
	CONS
};

class object {

	const object_type type;
	const void *      data;

public:

	object (const object_type & type, const void * data); // constructor
	~object () noexcept; // destructor
	
	const object_type & get_type ();
	template < typename T > const T * get_data ();

};*/

enum class type { // add string
	INT,
	SYMBOL,
	CONS
};

struct object {
	const type type;
	const std :: shared_ptr < const void > data;
};

struct cons_cell {
	std :: shared_ptr < object > car; // copy address register, head, first // FIXME const!
	std :: shared_ptr < object > cdr; // copy decrement register, tail, rest
};

std :: shared_ptr < object > cons (std :: shared_ptr < object >, std :: shared_ptr < object >);

bool is_atom (std :: shared_ptr <object>);
bool is_cons (std :: shared_ptr <object>);

std :: shared_ptr < object > car (std :: shared_ptr <object>);
std :: shared_ptr < object > cdr (std :: shared_ptr <object>);

std :: string print (std :: shared_ptr < object >);

// FIXME type as template parameter
template < typename T >
std :: shared_ptr < object > create_object (type type, std :: shared_ptr < T > data) {
	return std :: shared_ptr < object > (new object { type, std :: static_pointer_cast < const void > (data) });
};

/*template < typename T >
std :: shared_ptr < const T > get_object_data (std :: shared_ptr < object > object) { // FIXME check for null!
	return std :: static_pointer_cast < const T > (object -> data);
};*/

extern std :: shared_ptr <object> NIL;

#endif
