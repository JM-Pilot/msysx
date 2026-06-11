/* MIT LICENSE (C) 2026 JM-Pilot for msysx */
#include <stdint.h>
#include <video/fb.h>
#include <video/psf.h>

struct psf2_font *font_main;
extern uint8_t _binary_misc_fonts_default_psfu_start[];

void psf_init()
{
	font_main = (struct psf2_font*)
		&_binary_misc_fonts_default_psfu_start;
	if (font_main->magic != PSF2_FONT_MAGIC)
	{
		for (;;);
	}
}
void psf_render_char(
	uint8_t c, uint32_t x, 
	uint32_t y, uint32_t fg, 
	uint32_t bg
){
	uint8_t *glyph = 
		(uint8_t*)&_binary_misc_fonts_default_psfu_start+
		font_main->header_size + c * font_main->bytes_per_glyph;
	
	uint32_t bpg_line = (font_main->width + 7) / 8;
	for (uint32_t gy = 0; gy < font_main->height; gy++){
		uint8_t* current_ln = glyph + (bpg_line * gy);
		for (uint32_t gx = 0; gx < font_main->width; gx++){
			uint8_t byte = current_ln[gx / 8];
			if (byte & (0x80 >> (gx % 8))) {
				fb_plot_pix(x + gx, y + gy, fg);
			} else {
				fb_plot_pix(x + gx, y + gy, bg);
			}
		}
	}
}