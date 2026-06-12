/*
	2014 Leonard Kevin McGuire Jr (www.kmcg3413.net) (kmcg3413@gmail.com)
	2016 Clément Gallet (provided bug fixes)
	2026 JM-Pilot (github) (simplified the code and made it work on this kernel)
*/
#include <stdint.h>
#ifndef _MSYSX_MM_HEAP_H
#define _MSYSX_MM_HEAP_H

typedef struct heap_block{ 
	struct heap_block *next;
	uint64_t size;
	uint64_t used;
	uint64_t blk_size;
	uint64_t lfb;
} heap_block_t;

typedef struct heap_bm {
	heap_block_t *fblock;
} heap_bm_t;

void heap_init(heap_bm_t *heap);
int heap_add_block(heap_bm_t *heap, uintptr_t addr, uint64_t size, uint64_t blk_size);
uint8_t heap_get_next_id(uint8_t a, uint8_t b);
void *heap_alloc(heap_bm_t *heap, uint64_t size);
void heap_free(heap_bm_t *heap, void *ptr);

#endif