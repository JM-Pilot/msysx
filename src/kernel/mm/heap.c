/*
	2014 Leonard Kevin McGuire Jr (www.kmcg3413.net) (kmcg3413@gmail.com)
	2016 Clément Gallet (provided bug fixes)
	2026 JM-Pilot (github) (simplified the code and made it work on this kernel)
*/
#include <msysx/kernel.h>
#include <msysx/mm/heap.h>
void heap_init(heap_bm_t *heap){
	heap->fblock = 0;
}
int heap_add_block(heap_bm_t *heap, uintptr_t addr, uint64_t size, uint64_t blk_size){
	heap_block_t 	*b; 
	uint64_t 	blk_count;
	uint64_t	x;
	uint8_t		*bm;
	
	b = (heap_block_t*)addr;
	b->size = size - sizeof(heap_block_t);
	b->blk_size = blk_size;
	
	b->next = heap->fblock;
	heap->fblock = b;

	blk_count = b->size / b->blk_size;
	bm = (uint8_t*)&b[1];

	/* clear bitmap */
	for (x = 0; x < blk_count; ++x) {
			bm[x] = 0;
	}
	/* reserve room for bitmap */
	blk_count = (blk_count / blk_size) * blk_size < blk_count ? blk_count / blk_size + 1 : blk_count / blk_size;
	for (x = 0; x < blk_count; ++x) {
			bm[x] = 5;
	}
	
	b->lfb = blk_count - 1;
	
	b->used = blk_count;
	
	return 1;
}
uint8_t heap_get_next_id(uint8_t a, uint8_t b){
	uint8_t	c;	
	for (c = a + 1; c == b || c == 0; ++c);
	return c;
}
void *heap_alloc(heap_bm_t *heap, uint64_t size){
	heap_block_t	*b;
	uint8_t		*bm;
	uint64_t	bcnt;
	uint64_t	x, y, z;
	uint64_t	bneed;
	uint8_t		nid;

	/* iterate blocks */
	for (b = heap->fblock; b; b = b->next) {
		/* check if block has enough room */
		if (b->size - (b->used * b->blk_size) >= size) {
			
			bcnt = b->size / b->blk_size;		
			bneed = (size / b->blk_size) * b->blk_size < size ? size / b->blk_size + 1 : size / b->blk_size;
			bm = (uint8_t*)&b[1];
			/*						      | Error Here*/
			/* for (x = (b->lfb + 1 >= bcnt ? 0 : b->lfb + 1); x < b->lfb; ++x) */
			for (x = (b->lfb + 1 >= bcnt ? 0 : b->lfb + 1); x != b->lfb; ++x) {
				/* just wrap around */
				if (x >= bcnt) {
					x = 0;
				}		

				if (bm[x] == 0) {	
					/* count free blocks */
					for (y = 0; bm[x + y] == 0 && y < bneed && (x + y) < bcnt; ++y);
					
					/* we have enough, now allocate them */
					if (y == bneed) {
						/* find ID that does not match left or right */
						nid = heap_get_next_id(bm[x - 1], bm[x + y]);
						
						/* allocate by setting id */
						for (z = 0; z < y; ++z) {
							bm[x + z] = nid;
						}
						
						/* optimization */
						b->lfb = (x + bneed) - 2;
						
						/* count used blocks NOT bytes */
						b->used += y;
						
						return (void*)(x * b->blk_size + (uintptr_t)&b[1]);
					}
					
					/* x will be incremented by one ONCE more in our FOR loop */
					x += (y - 1);
					continue;
				}
			}
		}
	}
	
	return 0;
}
void heap_free(heap_bm_t *heap, void *ptr){
	heap_block_t	*b;	
	uintptr_t	ptroff;
	uint64_t	bi, x;
	uint8_t		*bm;
	uint8_t		id;
	uint64_t	max;
	
	for (b = heap->fblock; b; b = b->next) {
		if ((uintptr_t)ptr > (uintptr_t)b && (uintptr_t)ptr < (uintptr_t)b + sizeof(heap_block_t) + b->size) {
			/* found block */
			ptroff = (uintptr_t)ptr - (uintptr_t)&b[1];  /* get offset to get block */
			/* block offset in BM */
			bi = ptroff / b->blk_size;
			/* .. */
			bm = (uint8_t*)&b[1];
			/* clear allocation */
			id = bm[bi];
			/* oddly.. GCC did not optimize this */
			max = b->size / b->blk_size;
			for (x = bi; x < max && bm[x] == id; ++x) {
				bm[x] = 0;
			}
			/* update free block count */
			b->used -= x - bi;
			return;
		}
	}
	
	/* this error needs to be raised or reported somehow */
	return;
}