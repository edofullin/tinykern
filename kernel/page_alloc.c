#include "page_alloc.h"
#include "kconfig.h"
#include "kio.h"
#include "panic.h"
#include "types.h"
#include "utils/addr.h"
#include "utils/mem.h"
#include "vm.h"
#include "addressing.h"
#include "early_alloc.h"
#include "utils/bitops.h"

bool page_alloc_ready = FALSE;

extern char KERNEL_END[];

static uint64 phyalloc_start;
static uint64 phyalloc_size; // number of pages

// each bit represents 1 page. 512 elements * 64bit = 32768 pages
// 32768 * 4096 = 134217728 (128 MiB)
uint64 order_1_usage[512];

bool is_page_free_index(uint32 idx) {
    uint32 bitmap = order_1_usage[idx / 64];
    uint32 bitnum = 64 - (idx % 64); // bit 0 is the one from the left

    return !(bitmap & (1 << bitnum));
}

void set_page_status_index(uint32 idx, bool allocated) {
    uint32 bitmap = order_1_usage[idx / 64];
    uint32 bitnum = 64 - (idx % 64); // bit 0 is the one from the left

    order_1_usage[idx / 64] = BIT_SET(bitmap, bitnum, allocated ? 1 : 0);
}

void* get_page_addr_index(uint32 index) {
    return (void*)(PA_DRAM_START + index * PAGE_SIZE);
}

void set_kernel_pages_used() {
    uint64 it = (uint64)PA_DRAM_START;
    uint32 i = 0;

    while(it <= ALIGN_PAGE_UP((uint64)KERNEL_END + EARLY_ALLOC_SIZE)) {
        set_page_status_index(i, TRUE);

        it += PAGE_SIZE;
        i++;
    }
}

void* get_first_free_page() {
    for(int i = 0; i < 512; ++i) {
        if(is_page_free_index(i))
            return get_page_addr_index(i);
    }

    return 0x0;
}

void kalloc_init() {

    // KERNEL_END is aligned on a page boundary (see kernel.ld)
    phyalloc_start = PA_DRAM_START;
    phyalloc_size = (PA_DRAM_END - PA_DRAM_START) >> 12;

    memsetb(order_1_usage, 512 * sizeof(uint64), 0);

    kprintf("page_alloc: initializing from %p size %d pages\n", phyalloc_start, phyalloc_size);

    // set pages containing .text and .data as already in use
    set_kernel_pages_used();

    kprintf("page_alloc: first free page at addr %p\n", get_first_free_page());

    page_alloc_ready = TRUE;
    return;
}

void* kalloc_pages(uint32 npages) {

    if(!page_alloc_ready) {
        panic("page allocator not ready");
    }

    

    return NULL;
}