#include "early_alloc.h"
#include "kconfig.h"
#include "page_alloc.h"
#include "panic.h"
#include "riscv.h"
#include "spinlock.h"
#include "types.h"
#include "vm.h"
#include "kio.h"
#include "early_alloc.h"
#include "cpu.h"

#include "uart/ns16550.h"
#include "utils/mem.h"
#include "uart/uart.h"

#define MAKE_SATP(ppn, asid, mode) (((uint64)mode << 60) | ((uint64)asid << 44) | (((uint64)ppn) >> PAGE_SIZE_BITS))

#define IS_PTE_VALID(pte) ((pte & ~(PTE_V)) > 0)
#define PTE_PPN(pte) (pte >> 10)

#define CONV_PTE_PTA(pte) ((pagetable)((pte >> 10) << 12))
#define CONV_PTA_PTE(pta) (((((uint64)pta) >> 12) << 10))

// addresses provided by the linker
extern char KERNEL_BEGIN[];
extern char KERNEL_TEXT_END[];

extern bool page_alloc_ready;

pagetable k_pagetable;
spinlock k_pt_lock;

uint64* vm_get_setup_pte(pagetable pt, uint64 va, bool setup) {
    va >>= 12; // we can safely ignore the offset to get the PTE
    
    uint64 pte, pdi;
    pagetable current_pt = pt;

    for(ushort i = 2; i > 0; --i) {
        pdi = (va >> (9*i)) & ((1 << 9) - 1);
        pte = current_pt[pdi];

        // kprintf("va %p level %d pdi %d pte %p valid %d\n", va, i, pdi, pte, IS_PTE_VALID(pte));

        if(IS_PTE_VALID(pte)) {
            current_pt = CONV_PTE_PTA(pte);
            continue;
        }

        // check if we are allowed to create new page tables
        if(!setup)
            panic("segmentation fault\n");
        
        pagetable new_pt = page_alloc_ready ? kalloc_pages(1) : kearly_alloc_page();
        current_pt[pdi] = CONV_PTA_PTE(new_pt);
        current_pt[pdi] |= PTE_V;
        
        current_pt = new_pt;
    }

    pdi = (va) & ((1 << 9) - 1); // L0
    pte = current_pt[pdi];
    // kprintf("va %p level %d pdi %d pte %p valid %d\n", va, 0, pdi, pte, IS_PTE_VALID(pte));

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

    spinlock_acquire(&k_pt_lock);

    do {
        pte = vm_get_setup_pte(pt, curr_va, TRUE); // address of PTE, creating every intermediate necessary PT

        if(!pte)
            panic("vmmap: segfault\n");

        // PTE will point to the physical address
        *pte = 0;
        *pte = (curr_pa >> 12) << 10; // remove offset then make space for flags
        *pte = *pte | flags | PTE_V;


        curr_va += PAGE_SIZE;
        curr_pa += PAGE_SIZE;
    } while(curr_va <= va_last);

    spinlock_release(&k_pt_lock);
    kprintf("vmmap: pages %p to %p mapped from %p flags %d\n", va, va + size, pa, flags);
}

void vmmap_kern(uint64 va, uint64 pa, uint64 size, uint32 flags) {
    return vmmap(k_pagetable, va, pa, size, flags);
}

void kvm_map_devices() {
    ns16550* uarts = get_uart_devices();

    // map uarts
    for(uint64 i = 0; i < NUARTS; ++i) {
        if(uarts[i].baseaddr)
            vmmap(k_pagetable, 
                    uarts[i].baseaddr,
                    uarts[i].baseaddr,
                    PAGE_SIZE,
                    PTE_R | PTE_W
            );
    }
}

void kvm_init() {
    kprintf("kvm: initializing virtual memory\n");
    spinlock_init(&k_pt_lock, "kpt");

    k_pagetable = kearly_alloc_page();
    memsetb(k_pagetable, PAGE_SIZE, 0);

    // map kernel text
    vmmap(k_pagetable, (uint64)KERNEL_BEGIN, (uint64)KERNEL_BEGIN, KERNEL_TEXT_END-KERNEL_BEGIN, PTE_R | PTE_X);

    // map kernel data as rw
    vmmap(k_pagetable, (uint64)KERNEL_TEXT_END, (uint64)KERNEL_TEXT_END, KERNEL_DATA_SIZE, PTE_R | PTE_W);

    // map CLINT
    vmmap(k_pagetable, CLINT_BASE_ADDR, CLINT_BASE_ADDR, 16 * PAGE_SIZE, PTE_R | PTE_W);
    
    kvm_map_devices();
}

void kvm_mmu_enable() {

    kprintf("kvm: turning on paging for cpu%d\n", cpuid());
    register uint64 satp = MAKE_SATP((uint64)k_pagetable, 0x0, 0x8);

    sfence_vma();

    w_satp(
        satp
    );

    sfence_vma();
    kprintf("kvm: paging enabled for cpu%d\n", cpuid());
}

uint64 vm_translate_pa(pagetable pt, uint64 va) {
    uint64* pte = vm_get_setup_pte(pt, va, FALSE);
    uint64 offset = va & ((1 << 12) - 1);


    if(!pte) {
        panic("vm_translate invalid pte ptr");
    }

    if(!IS_PTE_VALID(*pte)) {
        panic("invalid pte");
    }

    uint64 ppn = *pte >> 10;

    return (ppn << 12) | offset;

}

