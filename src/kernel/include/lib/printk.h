#include <stdarg.h>

#ifndef _MSYSX_PRINTK_H
#define _MSYSX_PRINTK_H

void vsprintf(char *buf, const char *fmt, va_list args);
void printk(const char *fmt, ...);

#endif