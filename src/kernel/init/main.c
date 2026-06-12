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
#include <msysx/requests.h>
#include <uart/serial.h>
#include <autoconf.h>
#include <msysx/kernel.h>
#include <string.h>
#include <msysx/mm/pmm.h>
#include <msysx/mm/vmm.h>
#include <msysx/mm/heap.h>
uint64_t *kernel_pml4 __attribute__((aligned(4096)));
heap_bm_t kernel_heap;
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
	
	console_set_col(0x00FF00, 0x000000);
#if defined(CONFIG_ENABLE_UART)
	if (serial_init() == 0){
		serial_puts("\e[1;1H\e[2J");
		serial_puts("Serial IO Initialized\n");
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

	init_kernel_tables();
	printk("VMM Initialized\n");

	heap_init(&kernel_heap);
	printk("HEAP Initialized\n");

	console_set_col(0x3D98D1, 0x000000);
	printk("Welcome to MSYSX v0.1\n");
	console_set_col(0xFFFFFF, 0x000000);
	asm volatile ("sti");

	while (1){
		console_draw_cursor();

		/* I might change this delay to a pit controlled 
		 * nah im lazy, ima do this later
		 */
		for (int i = 0; i < 99999999; i++); 
	}
}

