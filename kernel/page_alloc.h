#pragma once

#include "types.h"

#define VA_KERNEL_HEAP_START   0xA0000000

// Initializes the page allocator
void kalloc_init();

// Allocates kernel memory for npages pages
void* kalloc_pages(uint32 npages);

// Allocates PHYSICAL memory for npages pages
// Memory is not mapped
uint64 kalloc_pages_pa(uint32 npages);