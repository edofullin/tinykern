#include "early_alloc.h"
#include "kio.h"
#include "log.h"
#include "panic.h"
#include "kconfig.h"
#include "spinlock.h"

// kreg_end set to the beginning of next page after the kernel space in kernel.ld
extern char KERNEL_END[];

void* next_free_addr = NULL;

void kearly_alloc_init() {
    next_free_addr = KERNEL_END;


    KLOG_INFO("early_alloc: initializing from %p to %p", KERNEL_END, (uint64)KERNEL_END + EARLY_ALLOC_SIZE);
}

void* kearly_alloc_page() {
    void* ret = next_free_addr;

    if(ret + PAGE_SIZE > (void*)KERNEL_END + (EARLY_ALLOC_SIZE)) {
        panic("early allocator out of memory");
    }

    next_free_addr += PAGE_SIZE;

    return ret;
}

void* kearly_alloc_get_addr_space() {
    return KERNEL_END;
}


uint64 kearly_alloc_get_num_alloc_pages() {
    return (next_free_addr - (void*)KERNEL_END) >> 12;
}



