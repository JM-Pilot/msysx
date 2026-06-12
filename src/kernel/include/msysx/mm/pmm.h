#include <stdint.h>
#ifndef _MSYSX_MM_PMM_H
#define _MSYSX_MM_PMM_H

#define PAGE_SIZE 4096

void pmm_init();
int pmm_free(uint64_t addr);
uintptr_t pmm_alloc();
#endif