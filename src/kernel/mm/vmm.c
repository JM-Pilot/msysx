#include <stdint.h>
#include <msysx/vmm.h>
#include <msysx/pmm.h>
#include <msysx/requests.h>
#include <stddef.h>
#include <printk.h>
#include <string.h>
#include <msysx/kernel.h>

#define PTE_PRESENT (1ULL << 0)
#define PTE_WRITABLE (1ULL << 1)
#define PTE_NX (1ULL << 63)
#define ADDR_MASK 0xFFFFFFFFFF000ULL

extern char _kernel_start[];
extern char _kernel_end[];

uint64_t virt_to_phys(uint64_t *addr)
{
	return (uint64_t)addr-hhdm_request.response->offset;
}

void *phys_to_virt(uint64_t phys_addr)
{
	return (void *)(phys_addr + hhdm_request.response->offset);
}

uint64_t *get_next_table(uint64_t *table, size_t index, uint64_t flags)
{
	if (!(table[index] & PTE_PRESENT)){
		uint64_t new_page = (uint64_t)pmm_alloc();
		if (new_page == 0){
			printk("ERROR! IN GET_NEXT_TABLE,");
			printk("ALLOCATE NEW_PAGE FAILED\n");
			return 0;
		}
		uint64_t *np_virt_addr = (uint64_t*)phys_to_virt(new_page);
		memset(np_virt_addr, 0, 4096);

		table[index] = (new_page & ADDR_MASK) | flags;
		return (uint64_t*)phys_to_virt(table[index] & ADDR_MASK);
	}
	return (uint64_t*)phys_to_virt(table[index] & ADDR_MASK);
}

void vmm_map_page(
	uint64_t *pml4, uint64_t phys_addr, 
	uint64_t virt_addr, uint64_t flags
){
	
	uint64_t pml4_idx = (virt_addr >> 39) & 0x1FF;
	uint64_t pmpt_idx = (virt_addr >> 30) & 0x1FF;
	uint64_t pd_idx = (virt_addr >> 21) & 0x1FF;
	uint64_t pt_idx = (virt_addr >> 12) & 0x1FF;

	uint64_t *pmpt = get_next_table(pml4,pml4_idx, PTE_PRESENT | 0x02);
	if (!pmpt)
		return;
	uint64_t *pd = get_next_table(pmpt, pmpt_idx, PTE_PRESENT | 0x02);
	if (!pd)
		return;
	uint64_t *pt = get_next_table(pd, pd_idx, PTE_PRESENT | 0x02);
	if (!pt)
		return;
	if (pt[pt_idx] & PTE_PRESENT)
		printk("VMM_MAP_PAGE: remapping page\n");
	pt[pt_idx] = (phys_addr & ADDR_MASK) | flags;
	asm volatile("invlpg (%0)" :: "r"(virt_addr) : "memory");
}

uint64_t *get_table(uint64_t *table, size_t index)
{
	if (!(table[index] & PTE_PRESENT))
		return NULL;
	return (uint64_t*)phys_to_virt(table[index] & ADDR_MASK);
}

void vmm_unmap_page(uint64_t *pml4, uint64_t virt_addr)
{	
	uint64_t pml4_idx = (virt_addr >> 39) & 0x1FF;
	uint64_t pmpt_idx = (virt_addr >> 30) & 0x1FF;
	uint64_t pd_idx = (virt_addr >> 21) & 0x1FF;
	uint64_t pt_idx = (virt_addr >> 12) & 0x1FF;

	uint64_t *pmpt = get_table(pml4, pml4_idx);
	if (!pmpt)
		return;
	uint64_t *pd = get_table(pmpt, pmpt_idx);
	if (!pd)
		return;
	uint64_t *pt = get_table(pd, pd_idx);
	if (!pt)
		return;

	if (!(pt[pt_idx] & PTE_PRESENT))
		return;
	pt[pt_idx] = 0;
	asm volatile("invlpg (%0)" :: "r"(virt_addr) : "memory");
}

void init_kernel_tables()
{
	uint64_t virt_base = exe_address_request.response->virtual_base;
	uint64_t phys_base = exe_address_request.response->physical_base;
	uint64_t pml4_phys = (uint64_t)pmm_alloc();
	uint64_t *pml4 = (uint64_t*)phys_to_virt(pml4_phys);
	memset(pml4, 0, 4096);

	for (uint64_t addr = (uint64_t)_text_start; 
		addr < (uint64_t)_text_end; 
		addr += 0x1000
	) {
    		uint64_t phys = addr - virt_base + phys_base;
    		vmm_map_page(pml4, phys, addr, PTE_PRESENT);
	}

	for (uint64_t addr = (uint64_t)_rodata_start; 
		addr < (uint64_t)_rodata_end; 
		addr += 0x1000
	) {
    		uint64_t phys = addr - virt_base + phys_base;
    		vmm_map_page(pml4, phys, addr, PTE_PRESENT | PTE_NX);
	}

	for (uint64_t addr = (uint64_t)_data_start; 
		addr < (uint64_t)_data_end; 
		addr += 0x1000
	) {
		uint64_t phys = addr - virt_base + phys_base;
		vmm_map_page(pml4, phys, addr, PTE_PRESENT | 
			PTE_WRITABLE | PTE_NX);
	}
	for (uint64_t i = 0; i < memmap_request.response->entry_count; i++) {
		struct limine_memmap_entry *entry = 
			memmap_request.response->entries[i];
		if (entry->type == 1) continue;

		for (uint64_t phys = entry->base; 
			phys < entry->base + entry->length; 
			phys += 0x1000
		) {
			vmm_map_page(
				pml4, phys, 
				phys + hhdm_request.response->offset, 
				PTE_PRESENT | PTE_WRITABLE | PTE_NX
			);
		}
	}
	printk("hhdm map done\n");
	printk("Moving pml4 to cr3\n");
	asm volatile("mov %0, %%cr3" :: "r"(pml4_phys) : "memory");
	printk("Done moving pml4 to cr3\n");
}