#include <stdint.h>
#include <limine.h>
#ifndef _DRIVERS_VIDEO_FB_H
#define _DRIVERS_VIDEO_FB_H  


extern struct limine_framebuffer *fb_main;
void fb_init();
void fb_plot_pix(uint32_t x, uint32_t y, uint32_t color);
uint32_t fb_get_pix_col(uint32_t x, uint32_t y);
#endif