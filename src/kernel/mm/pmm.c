#include <msysx/mm/pmm.h>
#include <stdint.h>
#include <limine.h>
#include <msysx/requests.h>
#include <printk.h>
#include <stddef.h>
#include <string.h>
uint64_t available_mem;
uint8_t *bitmap;
uint64_t pages;
uint64_t bitmap_size;

extern uint8_t _kernel_start;
extern uint8_t _kernel_end;
int set_used(uint64_t addr)
{
	uint64_t page = addr / 4096;
	if (page >= pages)
		return 1;
	bitmap[page / 8] |= (1 << (page % 8));
	return 0;
}

int pmm_free(uint64_t addr)
{
	uint64_t page = addr / 4096;
	if (page >= pages)
		return 1;
	bitmap[page / 8] &= ~(1 << (page % 8));
	return 0;
}

void pmm_alloc_region(uint64_t addr, size_t len)
{
	uint64_t pages_in = len / PAGE_SIZE;
	for (uint64_t i = 0; i < pages_in; i++){
		set_used(addr + i * PAGE_SIZE);
	}
}

uintptr_t pmm_alloc()
{
	for (uintptr_t i = 0; i < pages; i++){
		if (!(bitmap[i / 8] & (1 << (i % 8)))){
			bitmap[i / 8] |= (1 << (i % 8));
			return i * PAGE_SIZE;
		}
	}
	return (uintptr_t)NULL;
}

void pmm_init()
{
	struct limine_memmap_response *memmap = memmap_request.response;

	struct limine_hhdm_response *hhdm = hhdm_request.response;

	for (uint64_t i = 0; i < memmap->entry_count; i++){
		if (memmap->entries[i]->type == LIMINE_MEMMAP_USABLE){
			available_mem += memmap->entries[i]->length;
		}
	}

	printk("PMM: Available Memory %lu MB\n", available_mem / (1024 * 1024));

	uint64_t ceiling = 0;
	for (uint64_t i = 0; i < memmap->entry_count; i++) {
		uint64_t top = 
			memmap->entries[i]->base + memmap->entries[i]->length;
		if (top > ceiling)
			ceiling = top;
	}

	pages = ceiling / PAGE_SIZE;
	bitmap_size = (pages + 7) / 8;
	
	/* Get the address for the bitmap */
	for (uint64_t i = 0; i < memmap->entry_count; i++){
		if (memmap->entries[i]->type == LIMINE_MEMMAP_USABLE
			&& memmap->entries[i]->base >= 0x1000
			&& memmap->entries[i]->length >= bitmap_size){
			bitmap = (uint8_t*)(memmap->entries[i]->base + hhdm->offset);
			break;
		}
	}
	printk("PMM: Bitmap address %lx \n", bitmap);
	memset(bitmap, 0xFF, bitmap_size);
	for (uint64_t i = 0; i < memmap->entry_count; i++){
		if (memmap->entries[i]->type == LIMINE_MEMMAP_USABLE){
			for (uint64_t j = 0; j < memmap->entries[i]->length; j+=
				PAGE_SIZE){
				pmm_free(memmap->entries[i]->base + j);
			}
		}
	}

	struct limine_executable_address_response *exec = 
		exe_address_request.response;

	pmm_alloc_region(exec->physical_base, &_kernel_end - &_kernel_start);
	pmm_alloc_region((uint64_t)bitmap - hhdm->offset,
                 (bitmap_size + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1));
	pmm_alloc_region(0x0, 0x1000);
}