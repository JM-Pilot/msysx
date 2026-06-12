#include <stdint.h>
#ifndef _MSYSX_VMM_H
#define _MSYSX_VMM_H


void vmm_map_page(
	uint64_t *pml4, uint64_t phys_addr, 
	uint64_t virt_addr, uint64_t flags
);

void vmm_unmap_page(uint64_t *pml4, uint64_t virt_addr);

#endif