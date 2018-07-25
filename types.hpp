#ifndef TYPES_HPP
#define TYPES_HPP

typedef signed char int8_t;
typedef unsigned char uint8_t;

typedef signed short int16_t;
typedef unsigned char uint16_t;

typedef signed int int32_t;
typedef unsigned int uint32_t;

typedef signed long long int64_t;
typedef unsigned long long uint64_t;

enum error_t
{
	OK = 0,
	DIMENSION_ERROR = 1,
	DETERMINANT_ERROR = 2,
	IDENTITY_ERROR = 3,
	CANT_FIND = 4,
	OTHER_ERROR
};

enum task_t
{
	FIND_VOLTAGE_SOURCE,
	FIND_OTHER_ELEMENT
};

#endif // TYPES

