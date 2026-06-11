#include <stdint.h>
#include <interrupts/idt.h>
#include <printk.h>
#include <msysx/kernel.h>
#include <msysx/console.h>
const char *exception_messages[] = {
	"Division By Zero",
	"Debug",
	"Non Maskable Interrupt",
	"Breakpoint",
	"Into Detected Overflow",
	"Out of Bounds",
	"Invalid Opcode",
	"No Coprocessor",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Bad TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"Unknown Interrupt",
	"Coprocessor Fault",
	"Alignment Check",
	"Machine Check",
};

void isr_interrupt_handler(struct interrupt_frame *r)
{
	if (r->interrupt_no < 32)
	{
		/* DONT MESSAGE ME ABOUT WHY THIS IS SO MESSY
		 * as long as the interrupt msg is clean, i dont have a problem
		 * with it
		 */

		console_putc('\n');
		for (uint32_t i = 0; i < CELLS_X; i++)
			console_putc('=');
		
		for (uint32_t i = 0; i < (CELLS_X / 2 - (26 / 2)); i++)
			console_putc(' ');
		printk("ERROR, INTERRUPT REACHED\n");
		
		for (uint32_t i = 0; i < (CELLS_X / 2 - (26 / 2)); i++)
			console_putc(' ');
		printk("INTERRUPT: %s\n", r->interrupt_no < 19 ?
			 exception_messages[r->interrupt_no] : "Reserved");
		
		for (uint32_t i = 0; i < (CELLS_X / 2 - (26 / 2)); i++)
			console_putc(' ');
		printk("ERROR NUM: %lu \n", r->interrupt_error);
		
		for (uint32_t i = 0; i < CELLS_X; i++)
			console_putc('=');
		
		printk("RAX %lx, RCX %lx, RDX %lx, RBX %lx, RSI %lx \n",
			r->rax, r->rcx, r->rdx, r->rbx, r->rsi);
		printk("RDI %lx, RBP %lx, R8 %lx, R9 %lx, R10 %lx \n",
			r->rdi, r->rbp, r->r8, r->r9, r->r10);
		printk("R11 %lx, R12 %lx, R13 %lx, R14 %lx, R15 %lx \n",
			r->r11, r->r12, r->r13, r->r14, r->r15);
		printk("RIP %lx, CS %lx, RFLAGS %lx, USER_RSP %lx, SS %lx \n",
			r->rip, r->cs, r->rflags, r->user_rsp, r->ss);
		hcf();
	}
}