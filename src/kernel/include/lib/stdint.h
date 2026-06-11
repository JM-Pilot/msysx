#ifndef STDINT_H
#define STDINT_H

#define INT64_MAX (__INT64_C(9223372036854775807))

typedef unsigned char 		uint8_t;
typedef unsigned short 		uint16_t;
typedef unsigned int 		uint32_t;
typedef unsigned long int 	uint64_t;

typedef signed char 		int8_t;
typedef signed short 		int16_t;
typedef signed int 		int32_t;
typedef signed long int 	int64_t;

#if defined(__x86_64__)
	typedef unsigned long int uintptr_t;
#elif defined(__i386__)
	typedef unsigned int uintptr_t;	
#endif

#endif