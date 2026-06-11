/* MIT LICENSE 2026 JM-Pilot */
#include <limine.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <video/fb.h>
#include <video/psf.h>
#include <msysx/console.h>
#include <printk.h>
#include <boot/gdt.h>
#include <interrupts/idt.h>
#include <interrupts/pic.h>
__attribute__((used, section(".limine_requests")))
volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(6);

__attribute__((used, section(".limine_requests")))
volatile struct limine_framebuffer_request framebuffer_request = {
	.id = LIMINE_FRAMEBUFFER_REQUEST_ID,
	.revision = 0
};

__attribute__((used, section(".limine_requests_start")))
volatile uint64_t limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
volatile uint64_t limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

void hcf()
{
	asm volatile ("cli");
	for (;;)
		asm volatile ("hlt");
}



void check_limine_requests()
{
	if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false)
		hcf();
	if (framebuffer_request.response == NULL || 
	framebuffer_request.response->framebuffer_count < 1) 
		hcf();
}
void init_main()
{
	asm volatile ("cli");
	check_limine_requests();
	fb_init();
	psf_init();
	console_init();
	console_puts("Hello World!\n");
	gdt_init();
	printk("GDT Initialized\n");
	idt_init();
	printk("IDT Initialized\n");
	pic_init();
	printk("PIC Remaped\n");
	printk("IRQ Loaded\n");

	asm volatile ("sti");

	while (1){
		console_draw_cursor();

		/* I might change this delay to a pit controlled 
		 * nah im lazy, ima do this later
		 */
		for (int i = 0; i < 99999999; i++); 
	}
}

