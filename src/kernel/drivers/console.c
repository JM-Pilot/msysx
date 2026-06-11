#include <msysx/console.h>
#include <stdint.h>
#include <video/psf.h>
#include <video/fb.h>
#include <string.h>

struct console console_main;
void console_init()
{
	console_main.bg = 0;
	console_main.fg = 0xFFFFFF;
	console_main.cursor_x = 0;
	console_main.cursor_y = 0;
	console_main.prev_cursor_x = 0;
	console_main.prev_cursor_y = 0;
}

void console_draw_cursor()
{
	for (uint32_t y = 0; y < font_main->height; y++){
        	for (uint32_t x = 0; x < font_main->width; x++){
            		uint64_t px = console_main.cursor_x * 
				font_main->width + x;
            		uint64_t py = console_main.cursor_y * 
				font_main->height + y;

            		fb_plot_pix(px, py, 0xFFFFFF ^ fb_get_pix_col(px, py));
        	}
    	}
}


void scroll_up()
{
	uint8_t *addr = (uint8_t*)fb_main->address;
	uint32_t line = fb_main->pitch * font_main->height;
	memmove(addr, addr + line, fb_main->pitch * fb_main->height - line);
	memset(addr + fb_main->pitch * fb_main->height - line, 0, line);
	console_main.cursor_y--;
}

void insert_nl()
{
	console_main.cursor_x = 0;
	if (++console_main.cursor_y >= CELLS_Y){
		scroll_up();
		console_main.cursor_y--;
	}
	console_draw_cursor();
}
void console_putc(char c)
{
	console_draw_cursor();
	switch (c) {
		case '\n':
			insert_nl();
			return;
		case '\r':
			console_main.cursor_x = 0;
			console_draw_cursor();
			return;
		case '\b':
			if (console_main.cursor_x > 0){
				console_main.cursor_x--;
			}
			psf_render_char(' ',
				console_main.cursor_x * font_main->width, 
				console_main.cursor_y * font_main->height,
				console_main.fg, console_main.bg);
			console_draw_cursor();
			return;
	}
	psf_render_char(c,
		console_main.cursor_x * font_main->width, 
		console_main.cursor_y * font_main->height,
		console_main.fg, console_main.bg);


	console_main.prev_cursor_x = console_main.cursor_x;
	console_main.prev_cursor_y = console_main.cursor_y;

	if (++console_main.cursor_x >= CELLS_X)
		insert_nl();

	console_draw_cursor();
}
void console_puts(char *s)
{
	while (*s)
		console_putc(*s++);
}