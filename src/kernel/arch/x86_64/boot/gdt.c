#include <stdint.h>
#include <boot/gdt.h>

extern void gdt_load();
struct gdt_entry gdt[3];
struct gdt_desc gdtr;
void gdt_set_desc(
	int n,
	uint32_t base, 
	uint32_t limit, 
	uint8_t access, 
	uint8_t flags
){
	gdt[n].base_low = base & 0xFFFF;
	gdt[n].base_mid = (base >> 16) & 0xFF;
	gdt[n].base_high = (base >> 24) & 0xFF;
	
	gdt[n].limit_low = limit & 0xFFFF;
	gdt[n].access_byte = access;
	gdt[n].flags = (flags << 4) | ((limit >> 16) & 0x0F);
}

void gdt_init()
{
	gdtr.size = sizeof(gdt) - 1;
	gdtr.offset = (uint64_t)&gdt;
	gdt_set_desc(0, 0, 0, 0, 0);
	gdt_set_desc(1, 0, 0xFFFFF, 0x9A, 0xA);
	gdt_set_desc(2, 0, 0xFFFFF, 0x92, 0xC);
	gdt_load();
}