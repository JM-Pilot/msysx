#include <stdint.h>
#include <uart/serial.h>
#include <asm.h>
#include <stdbool.h>
#include <autoconf.h>
#include <printk.h>

bool com1_default = false;

/* All depends on COM1 */
int serial_init()
{
	com1_default = CONFIG_ENABLE_SERIAL_COM1;
	if (com1_default == 1){
		outb(COM1 + 1, 0x00);
		outb(COM1 + 1, 0x00);
		outb(COM1 + 3, 0x80);
		outb(COM1 + 0, 0x03);
		outb(COM1 + 1, 0x00);
		outb(COM1 + 3, 0x03);
		outb(COM1 + 2, 0xC7);
		outb(COM1 + 4, 0x0B);
		outb(COM1 + 4, 0x1E);
		outb(COM1 + 0, 0xAE);

		if(inb(COM1 + 0) != 0xAE) {
      			return 1;
   		}
		outb(COM1 + 4, 0x0F);
   		return 0;
	}
	
	printk("SERIAL_INIT: /dev/ttyS0 not yet implemented\n");
	/* I dont have anything to output yet */
	return 255;
}

int serial_received() {
	if (com1_default)
   		return inb(COM1 + 5) & 1;
	return 0;
}

char read_serial() {
   	while (serial_received() == 0);
	if (com1_default)
   		return inb(COM1);
	return 0;
}

int is_transmit_empty() {
	if (com1_default) 
   		return inb(COM1 + 5) & 0x20;
	return 0;
}


void serial_putc(char c)
{
	while (is_transmit_empty() == 0);
	if (com1_default)
   		outb(COM1, c);
}
void serial_puts(const char *s)
{
	while (*s)
		serial_putc(*s++);
}