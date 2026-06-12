#include <stdint.h>
#ifndef _ARCH_X86_64_ASM_H
#define _ARCH_X86_64_ASM_H

/* BYTE */
static inline void outb(uint16_t port, uint8_t val)
{
	asm volatile ("outb %0, %1" 
		: 
		: "a"(val), "Nd"(port) 
		: "memory");
}

static inline uint8_t inb(uint16_t port)
{
	uint8_t ret;
	asm volatile ("inb %1, %0"
			: "=a"(ret)
			: "Nd"(port)
			: "memory");
	return ret;
}

/* WORD */
static inline void outw(uint16_t port, uint16_t val)
{
	asm volatile ("outw %0, %1" 
		: 
		: "a"(val), "Nd"(port) 
		: "memory");
}

static inline uint16_t inw(uint16_t port)
{
	uint16_t ret;
	asm volatile ("inw %1, %0"
			: "=a"(ret)
			: "Nd"(port)
			: "memory");
	return ret;
}
/* LONG */
static inline void outl(uint16_t port, uint32_t val)
{
	asm volatile ("outl %0, %1" 
		: 
		: "a"(val), "Nd"(port) 
		: "memory");
}
static inline uint32_t inl(uint16_t port)
{
	uint32_t ret;
	asm volatile ("inl %1, %0"
			: "=a"(ret)
			: "Nd"(port)
			: "memory");
	return ret;
}

/* QUAD */
static inline void outq(uint16_t port, uint64_t val)
{
	asm volatile ("outq %0, %1" 
		: 
		: "a"(val), "Nd"(port) 
		: "memory");
}
static inline uint64_t inq(uint16_t port)
{
	uint64_t ret;
	asm volatile ("inq %1, %0"
			: "=a"(ret)
			: "Nd"(port)
			: "memory");
	return ret;
}



#endif