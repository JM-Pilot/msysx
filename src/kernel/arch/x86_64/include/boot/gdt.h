#include <stdint.h>
#ifndef _ARCH_X86_64_GDT_H
#define _ARCH_X86_64_GDT_H

#define KERNEL_CODE_SELECTOR 0x8
#define KERNEL_DATA_SELECTOR 0x10
struct gdt_desc {
	uint16_t size;
	uint64_t offset;
}__attribute__((packed));

struct gdt_entry {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t access_byte;
	uint8_t flags;
	uint8_t base_high;
}__attribute__((packed));

void gdt_set_desc(
	int n,
	uint32_t base, 
	uint32_t limit, 
	uint8_t access, 
	uint8_t flags
);

void gdt_init();
#endif