#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>

enum type {
	INT,
	SYMBOL,
	CONS
};

struct object {
	const type type;
	const std :: shared_ptr < const void > data;
};

std :: string print (std :: shared_ptr < object >);

template < typename T >
std :: shared_ptr < object > create_object (type type, std :: shared_ptr < T > data) {
	return std :: shared_ptr < object > (new object { type, std :: static_pointer_cast < const void > (data) });
};

template < typename T >
std :: shared_ptr < const T > get_object_data (std :: shared_ptr < object > object) {
	return std :: static_pointer_cast < const T > (object -> data);
};

#endif