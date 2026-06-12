#include <stdint.h>
#ifndef KMALLOC_H
#define KMALLOC_H


void *kmalloc(uint32_t size);
void kfree(void *ptr);

#endif