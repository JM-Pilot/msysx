#include <stdint.h>
#include <video/fb.h>
#include <video/psf.h>
#ifndef _MSYSX_CONSOLE_H
#define _MSYSX_CONSOLE_H

#define CELLS_X (fb_main->width / font_main->width)
#define CELLS_Y (fb_main->height / font_main->height)

struct console {
	uint32_t cursor_x, cursor_y;
	uint32_t prev_cursor_x, prev_cursor_y;
	uint32_t fg, bg;
};

extern struct console console_main;
void console_init();
void console_putc(char c);
void console_puts(char *s);
void console_draw_cursor();
void console_set_col(uint32_t fg, uint32_t bg);
#endif