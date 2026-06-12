#include <stdint.h>
#include <msysx/mm/heap.h>
#ifndef _MSYSX_KERNEL_H
#define _MSYSX_KERNEL_H

#define KERNEL_LOG_ERROR 1
#define KERNEL_LOG_DEBUG 2
#define KERNEL_LOG_WARNING 3
extern uint64_t *kernel_pml4;

extern heap_bm_t kernel_heap;

extern char _kernel_end[];
extern char _kernel_start[];
extern char _text_end[];
extern char _text_start[];
extern char _rodata_end[];
extern char _rodata_start[];
extern char _data_end[];
extern char _data_start[];

void init_kernel_tables();

void hcf();
#endif