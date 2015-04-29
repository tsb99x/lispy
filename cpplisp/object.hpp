#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>

enum type {
	INT,
	SYMBOL,
	CONS
};

struct object {
	type type;
	void* data;
	
	~object ();
};

object* create_object (type, void*);
std :: string print (object*);
std :: string to_dot_struct (object*);

template < typename T >
T* get_object_data (object* object) {
	return static_cast < T* > (object -> data);
};

#endif