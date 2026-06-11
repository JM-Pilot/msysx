#include <stdint.h>
#include <limine.h>
#ifndef _MSYSX_REQUESTS_H
#define _MSYSX_REQUESTS_H

extern volatile uint64_t limine_requests_start_marker[];
extern volatile uint64_t limine_requests_end_marker[];
extern volatile uint64_t limine_base_revision[];
extern volatile struct limine_framebuffer_request framebuffer_request;

#endif