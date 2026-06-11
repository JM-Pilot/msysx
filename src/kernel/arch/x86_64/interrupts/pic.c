#include <stdint.h>
#include <asm.h>
#include <interrupts/idt.h>
#include <boot/gdt.h>
extern uint64_t irq_stub_table[16];

void pic_remap()
{
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);
}

void pic_init()
{
	pic_remap();
	for (int i = 0; i < 16; i++)
		idt_set_entry(32 + i, 
			irq_stub_table[i], KERNEL_CODE_SELECTOR, 0x8E);
}