#include <stdarg.h>
#include <stdint.h>
#ifndef _MSYSX_PRINTK_H
#define _MSYSX_PRINTK_H

void vsprintf(char *buf, const char *fmt, va_list args);
void printk(const char *fmt, ...);

void lprintk(uint32_t level, const char *fmt, ...);

#endif