#include <stdint.h>
#ifndef _DRIVERS_VIDEO_PSF_H
#define _DRIVERS_VIDEO_PSF_H

#define PSF2_FONT_MAGIC 0x864ab572

struct psf2_font {
	uint32_t magic;
	uint32_t version;
	uint32_t header_size;
	uint32_t flags;
	uint32_t glyph_count;
	uint32_t bytes_per_glyph;
	uint32_t height;
	uint32_t width;
};

extern struct psf2_font *font_main;

void psf_init();
void psf_render_char(
	uint8_t c, uint32_t x, 
	uint32_t y, uint32_t fg, 
	uint32_t bg);
#endif