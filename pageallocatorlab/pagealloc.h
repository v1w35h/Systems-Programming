#pragma once
#include <stdbool.h>
#include <stdlib.h>

// Allocator functions
bool page_init(size_t pages);
void page_deinit(void);

// Allocation functions
void *page_alloc(size_t pages);
void  page_free(void *addr);

// Status functions
size_t pages_taken(void);
size_t pages_free(void);