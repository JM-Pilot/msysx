#include <stdint.h>
#include <limine.h>
#ifndef _MSYSX_REQUESTS_H
#define _MSYSX_REQUESTS_H

extern volatile uint64_t limine_requests_start_marker[];
extern volatile uint64_t limine_requests_end_marker[];
extern volatile uint64_t limine_base_revision[];
extern volatile struct limine_framebuffer_request framebuffer_request;
extern volatile struct limine_memmap_request memmap_request;
extern volatile struct limine_hhdm_request hhdm_request;
extern volatile struct limine_executable_address_request exe_address_request;

#endif