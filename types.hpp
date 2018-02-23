#ifndef TYPES_HPP
#define TYPES_HPP

typedef signed char int8_t;
typedef unsigned char uint8_t;

enum error_t
{
	OK = 0,
	DIMENSION_ERROR = 1,
	DETERMINANT_ERROR = 2,
	IDENTITY_ERROR = 3,
	OTHER_ERROR
};

#endif // TYPES

