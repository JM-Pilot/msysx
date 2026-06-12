#include <stdint.h>
#ifndef _MSYSX_KERNEL_H
#define _MSYSX_KERNEL_H

extern uint64_t kernel_pml4[512];
extern uint64_t kernel_pdpt[512];
extern uint64_t kernel_pd[512];
extern uint64_t kernel_pt[512];

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