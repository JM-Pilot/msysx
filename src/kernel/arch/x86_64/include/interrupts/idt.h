#include <stdint.h>
#ifndef _ARCH_X86_64_IDT_H
#define _ARCH_X86_64_IDT_H

struct idt_desc {
	uint16_t size;
	uint64_t offset;
}__attribute__((packed));

struct idt_entry {
	uint16_t offset_low;
	uint16_t segment_selector;
	uint8_t ist;
	uint8_t flags;
	uint16_t offset_mid;
	uint32_t offset_high;
	uint32_t rsvd;	/* i name reserved as "rsvd" because im lazy as hell */
}__attribute__((packed));

struct interrupt_frame {
	uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
	uint64_t rbp, rdi, rsi, rbx, rdx, rcx, rax;
	uint64_t interrupt_no, interrupt_error;
	uint64_t rip, cs, rflags, user_rsp, ss;
};

void idt_set_entry(
	int n,
	uint64_t offset,
	uint16_t segment,
	uint8_t flags
);

void idt_init();

#endif