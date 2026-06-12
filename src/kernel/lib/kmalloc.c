#include <stdint.h>
#include <kmalloc.h>
#include <msysx/mm/heap.h>
#include <msysx/kernel.h>

void *kmalloc(uint32_t size) { 
	return heap_alloc(&kernel_heap, size); 
}
void kfree(void *ptr) { 
	heap_free(&kernel_heap, ptr);
}
