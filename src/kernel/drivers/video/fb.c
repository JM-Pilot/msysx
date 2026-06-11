#include <video/fb.h>
#include <stdint.h>
#include <limine.h>
#include <msysx/requests.h>
struct limine_framebuffer *fb_main;
uint32_t *fb_ptr;
void fb_init()
{
	fb_main = framebuffer_request.response->framebuffers[0];
	fb_ptr = fb_main->address;
}
void fb_plot_pix(uint32_t x, uint32_t y, uint32_t color)
{
	fb_ptr[y * (fb_main->pitch / 4) + x] = color;
}

uint32_t fb_get_pix_col(uint32_t x, uint32_t y)
{
	return fb_ptr[y * (fb_main->pitch / 4) + x];
}