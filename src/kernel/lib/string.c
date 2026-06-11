#include <string.h>
#include <stddef.h>
#include <stdint.h>
void *memset(void *dest, int val, size_t size)
{
	uint8_t *d = dest;
	while (size--)
		*d++ = val;
	return dest;
}

void *memset16(void *dest, int val, size_t size)
{
	uint16_t *d = dest;
	while (size--)
		*d++ = val;
	return dest;
}

void *memmove(void *dest, void *src, size_t len)
{
	uint8_t *d = dest;
	uint8_t *s = src;
	if (d < s){
		while (len--)
			*d++ = *s++;
	} else {
		uint8_t *lasts = s + (len - 1);
		uint8_t *lastd = d + (len - 1);
		while (len--)
			*lastd-- = *lasts--;
	}
	return dest;
}
void *memmove16(void *dest, void *src, size_t len)
{
	uint16_t *d = dest;
	uint16_t *s = src;
	if (d < s){
		while (len--)
			*d++ = *s++;
	} else {
		uint16_t *lasts = s + (len - 1);
		uint16_t *lastd = d + (len - 1);
		while (len--)
			*lastd-- = *lasts--;
	}
	return dest;
}