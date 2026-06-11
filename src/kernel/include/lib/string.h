#include <stddef.h>
#ifndef STRING_H
#define STRING_H

void *memset(void *dest, int val, size_t size);
void *memset16(void *dest, int val, size_t size);

void *memmove(void *dest, void *src, size_t len);
void *memmove16(void *dest, void *src, size_t len);

#endif