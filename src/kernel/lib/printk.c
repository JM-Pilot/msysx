/* MIT LICENSE (C) 2026 JM-Pilot for msysx */
#include <printk.h>
#include <stdarg.h>
#include <stdint.h>
#include <msysx/console.h>
#include <stdbool.h>
char* conv_itoc(char *buf, int64_t val, int size, int base, bool is_unsigned)
{
	if (base == 10){
		char tmp_buffer[21];
        	int i = 0;

        	if (is_unsigned) {
        		uint64_t uval = (uint64_t)val;
			do {
				tmp_buffer[i++] = uval % 10 + '0';
			} while ((uval /= 10) > 0);
		} else {
			int sign = val < 0;
			if (sign) val = -val;
			do {
				tmp_buffer[i++] = val % 10 + '0';
			} while ((val /= 10) > 0);
			if (sign) tmp_buffer[i++] = '-';
		}
		int j = 0;
		while (i--)
			buf[j++] = tmp_buffer[i];
		buf[j] = '\0';
	} else if (base == 16){
		char hex_val[] = "0123456789ABCDEF";
		char tmp_buffer[18]; 
		int i, j;
		for (i = 0; i < (size == 32 ? 8 : 16); i++) {
			tmp_buffer[i] = hex_val[val & 0x0F];
			val >>= 4;
		}
		tmp_buffer[i++] = 'x';
		tmp_buffer[i++] = '0';
		j = 0;
		while (i--)
			buf[j++] = tmp_buffer[i];
		buf[j] = '\0';
	} else if (base == 2){
		char tmp_buffer[66]; 
		int i, j;
		for (i = 0; i < (size == 32 ? size : 64); i++) {
			tmp_buffer[i] = '0' + (val & 0x01);
			val >>= 1;
		}
		tmp_buffer[i++] = 'b';
		tmp_buffer[i++] = '0';
		j = 0;
		while (i--)
			buf[j++] = tmp_buffer[i];
		buf[j] = '\0';
	}
	return buf;
}
void vsprintf(char *buf, const char *fmt, va_list args)
{
	int i = 0;
	while (*fmt) {
		if (*fmt == '%') {
			switch (*++fmt){
				case 'd': {
					char tmp[20];
					conv_itoc(tmp, 
						va_arg(args, int), 
						0, 10, false);
					for (int j = 0; tmp[j]; j++){
						buf[i++] = tmp[j];
					}
					break;
				}
					
				case 'x': {
					char tmp[18];
					conv_itoc(tmp, 
						va_arg(args, uint32_t), 
						32, 16, true);
					for (int j = 0; tmp[j]; j++){
						buf[i++] = tmp[j];
					}
					break;
				}
				case 'l':{
					*fmt++;
					if (*fmt == 'x'){
						char tmp[18];
						conv_itoc(tmp,
						va_arg(args, uint64_t),
						64, 16, true);
						for (int j = 0; tmp[j]; j++){
							buf[i++] = tmp[j];
						}
						break;
					} else if (*fmt == 'u'){
						char tmp[20];
							conv_itoc(tmp, 
							va_arg(args, uint64_t), 
							0, 10, true);
						for (int j = 0; tmp[j]; j++){
							buf[i++] = tmp[j];
						}
						break;
					}
					char tmp[20];
					conv_itoc(tmp, 
						va_arg(args, int64_t), 
						0, 10, false);
					for (int j = 0; tmp[j]; j++){
						buf[i++] = tmp[j];
					}
					break;
				}
				case 'u':{
					char tmp[20];
					conv_itoc(tmp, 
						va_arg(args, uint32_t), 
						0, 10, true);
					for (int j = 0; tmp[j]; j++){
						buf[i++] = tmp[j];
					}
					break;
				}
				case 'b': {
					char tmp[36];
					conv_itoc(tmp, 
						va_arg(args, uint32_t), 
						32, 2, true);
					for (int j = 0; tmp[j] != '\0'; j++){
						buf[i++] = tmp[j];
					}
					break;
				}
				
				case 'c': {
					buf[i++] = (char)va_arg(args, int);
					break;
				}
					
				case 's': {
					char *s = va_arg(args, char *);
					while (*s)
						buf[i++] = *s++;
					break;
				}
					
				case '%': {
					buf[i++] = '%';
					break;
				}
					
				default:
					buf[i++] = *fmt++;
					break;	
			}
		} else {
			buf[i++] = *fmt;
		}
		fmt++;
	}
	buf[i] = '\0';
}
void printk(const char *fmt, ...)
{
	va_list (args);
	va_start(args, fmt);
	char buffer[1024];
	vsprintf(buffer, fmt, args);
	va_end(args);
	console_puts(buffer);	
}
