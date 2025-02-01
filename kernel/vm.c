#include "kconfig.h"
#include "panic.h"
#include "types.h"
#include "vm.h"
#include "kio.h"
#include "kalloc.h"

#include "utils/mem.h"

#define IS_PTE_VALID(pte) ((pte & ~(PTE_V)) > 0)
#define PTE_PPN(pte) (pte >> 10)

// addresses provided by the linker
extern char KERNEL_BEGIN[];
extern char KERNEL_TEXT_END[];

pagetable k_pagetable;

uint64* vm_get_setup_pte(pagetable pt, uint64 va, bool setup) {
    va >>= 12; // we can safely ignore the offset to get the PTE
    
    uint64 pte, pdi;
    pagetable current_pt = pt;

    for(ushort i = 2; i > 0; --i) {
        pdi = (va >> (9*i)) & ((1 << 9) - 1);
        pte = current_pt[pdi];

        if(IS_PTE_VALID(pte)) {
            current_pt = (uint64*)PTE_PPN(pte);
            continue;
        }

        // check if we are allowed to create new page tables
        if(!setup)
            panic("segmentation fault\n");
        
        pagetable new_pt = kmallocp();
        current_pt[pdi] = (uint64)new_pt;
        current_pt[pdi] <<= 10;

        current_pt[pdi] |= PTE_V;
        current_pt = new_pt;
    }

    pdi = (va) & ((1 << 9) - 1); // L0

    return &current_pt[pdi];
}

/*
 * maps size bytes of virtual memory starting from va 
 * to addresses starting from pa in physical memory
 */
void vmmap(pagetable pt, uint64 va, uint64 pa, uint64 size, uint32 flags) {
    if(size == 0)
        panic("vmmap: cannot map 0 bytes\n");

    if(va % PAGE_SIZE != 0)
        panic("vmmap: va %p not page aligned\n", va);

    if(pa % PAGE_SIZE != 0)
        panic("vmmap: pa %p not page aligned\n", pa);

    if(size % PAGE_SIZE != 0) {
        kprintf("vmmap: size %d from va %p not page size, ceiling it to page size\n");
        size = ((size >> PAGE_SIZE_BITS) + 1) << PAGE_SIZE_BITS;
    }

    uint64 curr_va = va, curr_pa = pa;
    uint64 va_last = va + size - PAGE_SIZE;
    uint64* pte;

    do {
        pte = vm_get_setup_pte(pt, va, TRUE); // address of PTE, creating every intermediate necessary PT

        if(!pte)
            panic("vmmap: segfault\n");

        // PTE will point to the physical address
        *pte = curr_pa << 10;
        *pte = *pte | PTE_V | flags;


        curr_va += PAGE_SIZE;
        curr_pa += PAGE_SIZE;
    } while(curr_va < va_last);
}


void kvm_init() {
    kprintf("initializing kernel vm\n");

    k_pagetable = kmallocp();
    memsetb(k_pagetable, PAGE_SIZE, 0);

    vmmap(k_pagetable, (uint64)KERNEL_BEGIN, (uint64)KERNEL_BEGIN, KERNEL_TEXT_END-KERNEL_BEGIN, PTE_R | PTE_X);
}

