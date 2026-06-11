#ifndef _DRIVERS_UART_SERIAL_H
#define _DRIVERS_UART_SERIAL_H

#define COM1 0x3F8
/* DEFAULTS TO COM1, IF IN THE MENUCONFIG "ENABLE_SERIAL_COM1" IS FALSE
 * IT SWITCHES TO /dev/ttyS0 (not yet implemented)
 */
int serial_init();
void serial_putc(char c);
void serial_puts(const char *s);

#endif