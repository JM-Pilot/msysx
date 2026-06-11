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
#include <msysx/pmm.h>
#include <msysx/requests.h>
#include <uart/serial.h>
#include <autoconf.h>
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
	if (memmap_request.response == NULL || 
	memmap_request.response->entry_count < 1)
		hcf();
	if (hhdm_request.response == NULL)
		hcf();
	if (exe_address_request.response == NULL)
		hcf();
}
void init_main()
{
	asm volatile ("cli");
	check_limine_requests();
	fb_init();
	psf_init();
	
	console_init();
	
#if defined(CONFIG_ENABLE_UART)
	if (serial_init() == 0){
		serial_puts("\e[1;1H\e[2J");
	}
#endif

	console_puts("CONSOLE Initialized\n");
	
	printk("RESOLUTION: %dx%dx%d\n", 
		fb_main->width, fb_main->height, fb_main->bpp);
	gdt_init();
	printk("GDT Initialized\n");
	idt_init();
	printk("IDT Initialized\n");
	pic_init();
	printk("PIC Remaped\n");
	printk("IRQ Loaded\n");

	pmm_init();
	printk("PMM Initialized\n");
	asm volatile ("sti");
	

	while (1){
		console_draw_cursor();

		/* I might change this delay to a pit controlled 
		 * nah im lazy, ima do this later
		 */
		for (int i = 0; i < 99999999; i++); 
	}
}

