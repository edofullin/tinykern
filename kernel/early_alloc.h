#pragma once
#include "types.h"

#define EARLY_ALLOC_SIZE (1ull << 20)

// Pages allocated by the early allocator are not supposed to be freed.
// The early allocator should only be used until paging is ready and the main allocator is set up. 



void kearly_alloc_init();

void* kearly_alloc_get_addr_space();
void* kearly_alloc_page();

uint64 kearly_alloc_get_num_alloc_pages();