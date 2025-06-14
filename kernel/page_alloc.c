#include "page_alloc.h"
#include "kconfig.h"
#include "kio.h"
#include "log.h"
#include "panic.h"
#include "spinlock.h"
#include "types.h"
#include "utils/addr.h"
#include "utils/mem.h"
#include "vm.h"
#include "addressing.h"
#include "early_alloc.h"
#include "utils/bitops.h"

bool page_alloc_ready = FALSE;

// defined by kernel.ld
extern char KERNEL_END[];

static uint64 phyalloc_start;
static uint64 phyalloc_size; // number of pages

static spinlock pages_alloc_lock;

// each bit represents 1 page. 512 elements * 64bit = 32768 pages
// 32768 * 4096 = 134217728 (128 MiB)
static uint64 order_1_usage[512];

static bool is_page_free_index(uint32 idx) {
    uint64 bitmap = order_1_usage[idx / 64];
    uint32 bitnum = 63 - (idx % 64); // bit 0 is the one from the left

    return !(bitmap & (1ull << bitnum));
}

static void set_page_status_index(uint32 idx, bool allocated) {
    uint32 word_num = idx / 64;
    uint64 bitmap = order_1_usage[word_num];
    uint32 bitnum = 63 - (idx % 64); // bit 0 is the one from the left

    if(allocated)
        order_1_usage[word_num] = BIT_SET(bitmap, bitnum);
    else
        order_1_usage[word_num] = BIT_RESET(bitmap, bitnum);

}

static void* get_page_addr_index(uint32 index) {
    return (void*)(PA_DRAM_START + index * PAGE_SIZE);
}

static uint32 get_free_block_size_index(uint32 idx, uint32 max) {
    uint32 ret = 0;

    while(ret < max && 
            ((idx + ret) < phyalloc_size) && 
            is_page_free_index(idx + ret)
    ) {
        ret++;
    }

    return ret;
}

static void set_kernel_pages_used() {
    uint64 it = (uint64)PA_DRAM_START;
    uint32 i = 0;

    while(it <= ALIGN_PAGE_UP((uint64)KERNEL_END + EARLY_ALLOC_SIZE)) {
        set_page_status_index(i, TRUE);

        it += PAGE_SIZE;
        i++;
    }
}

static void* get_first_free_page() {
    for(int i = 0; i < 512; ++i) {
        if(is_page_free_index(i))
            return get_page_addr_index(i);
    }

    return 0x0;
}

// PUBLIC

void kalloc_init() {

    if(!is_kvm_mmu_enabled()) {
        panic("page_alloc requires kvm to be available");
    }

    // KERNEL_END is aligned on a page boundary (see kernel.ld)
    phyalloc_size = (PA_DRAM_END - PA_DRAM_START) >> 12;
    spinlock_init(&pages_alloc_lock, "page_allocator");
    spinlock_acquire(&pages_alloc_lock);

    memsetb(order_1_usage, 512 * sizeof(uint64), 0);

    // set pages containing .text and .data as already in use
    set_kernel_pages_used();
    
    phyalloc_start = (uint64)get_first_free_page();
    
    KLOG_INFO("page_alloc: initializing from %x size %d pages total size %d MiB", phyalloc_start, phyalloc_size, phyalloc_size >> (20-12));
    KLOG_INFO("page_alloc: first free page at addr %x", phyalloc_start);

    page_alloc_ready = TRUE;
    spinlock_release(&pages_alloc_lock);
    return;
}

uint64 kalloc_pages_pa(uint32 npages) {
    uint64 pa = NULL;
    spinlock_acquire(&pages_alloc_lock);

    if(!page_alloc_ready) {
        panic("page allocator not ready");
    }

    for(uint32 i = 0; i < phyalloc_size; ++i) {
        uint32 free_block_size = get_free_block_size_index(i, npages);

        if(free_block_size >= npages) {
            pa = (uint64)get_page_addr_index(i);

            KLOG_INFO("page_alloc: allocated %d pages from PA %x", npages, pa);
            
            for(uint32 j = 0; j < npages; ++j) {
                set_page_status_index(i + j, TRUE);
            }
            
            goto finish;
        }
    }
    
finish:
    
    spinlock_release(&pages_alloc_lock);
    return pa;
}

void* kalloc_pages(uint32 npages) {
   uint64 pa = kalloc_pages_pa(npages);

   // Kernel pages start from VA_KERNEL_HEAP_START, 
   // however the first pages containing kernel .text and .data are ignored
   void* va = (void*)((pa - phyalloc_start) + VA_KERNEL_HEAP_START);

   vmmap_kern_internal((uint64)va, pa, npages * PAGE_SIZE, PTE_R | PTE_W);

   return va;
}