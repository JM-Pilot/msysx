#include <stdint.h>
#include <interrupts/idt.h>
#include <boot/gdt.h>
#include <printk.h>

struct idt_entry idt[256];
extern uint64_t isr_stub_table[32];
struct idt_desc idtr;
void idt_set_entry(
	int n,
	uint64_t offset,
	uint16_t segment,
	uint8_t flags
){
	idt[n].offset_low = offset & 0xFFFF;
	idt[n].offset_mid = (offset >> 16) & 0xFFFF;
	idt[n].offset_high = (offset >> 32) & 0xFFFFFFFF;

	idt[n].ist = 0;
	idt[n].flags = flags;
	idt[n].segment_selector = segment;
	idt[n].rsvd = 0;
}

void idt_init()
{
	idtr.size = sizeof(idt) - 1;
	idtr.offset = (uint64_t)&idt;

	for (int i = 0; i < 32; i++){
		idt_set_entry(i, isr_stub_table[i], KERNEL_CODE_SELECTOR, 0x8E);
	}

	asm volatile ("lidt %0" :
		: "m"(idtr));
}