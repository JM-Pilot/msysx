#include <stdint.h>
#include <stddef.h>
#ifndef _MSYSX_MM_VMM_H
#define _MSYSX_MM_VMM_H

#define PTE_PRESENT (1ULL << 0)
#define PTE_WRITABLE (1ULL << 1)
#define PTE_NX (1ULL << 63)
#define ADDR_MASK 0xFFFFFFFFFF000ULL

void vmm_map_page(
	uint64_t *pml4, uint64_t phys_addr, 
	uint64_t virt_addr, uint64_t flags
);

uint64_t virt_to_phys(uint64_t *addr);
void *phys_to_virt(uint64_t phys_addr);
uint64_t *get_next_table(uint64_t *table, size_t index, uint64_t flags);
void vmm_unmap_page(uint64_t *pml4, uint64_t virt_addr);

#endif