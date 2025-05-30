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
#include "log.h"

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
extern char KERNEL_END[];
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

        // KLOG_INFO("va %p level %d pdi %d pte %p valid %d", va, i, pdi, pte, IS_PTE_VALID(pte));

        if(IS_PTE_VALID(pte)) {
            current_pt = CONV_PTE_PTA(pte);
            continue;
        }

        // check if we are allowed to create new page tables
        if(!setup)
            panic("segmentation fault");
        
        pagetable new_pt;

        if(page_alloc_ready) {
            new_pt = (pagetable)kalloc_pages_pa(1);
            vmmap_kern_internal((uint64)new_pt, (uint64)new_pt, PAGE_SIZE, PTE_R | PTE_W);
        } else {
            new_pt = kearly_alloc_page();
        }

        // pagetable new_pt = kearly_alloc_page();
        current_pt[pdi] = CONV_PTA_PTE(new_pt);
        current_pt[pdi] |= PTE_V;
        
        current_pt = new_pt;
    }
    
    // KLOG_INFO("pt %x va %p level %d pdi %d pte %p valid %d", current_pt, va, 0, pdi, pte, IS_PTE_VALID(pte));
    
    pdi = (va) & ((1 << 9) - 1); // L0
    pte = current_pt[pdi];

    return &current_pt[pdi];
}

void vmmap_internal(pagetable pt, uint64 va, uint64 pa, uint64 size, uint32 flags) {
    if(size == 0)
        panic("vmmap: cannot map 0 bytes");

    if(va % PAGE_SIZE != 0)
        panic("vmmap: va %p not page aligned", va);

    if(pa % PAGE_SIZE != 0)
        panic("vmmap: pa %p not page aligned", pa);

    if(size % PAGE_SIZE != 0) {
        KLOG_INFO("vmmap: size %d from va %p not page size, ceiling it to page size");
        size = ((size >> PAGE_SIZE_BITS) + 1) << PAGE_SIZE_BITS;
    }

    uint64 curr_va = va, curr_pa = pa;
    uint64 va_last = va + size - PAGE_SIZE;
    uint64* pte;



    do {
        pte = vm_get_setup_pte(pt, curr_va, TRUE); // address of PTE, creating every intermediate necessary PT

        if(!pte)
            panic("vmmap: segfault");

        // PTE will point to the physical address
        *pte = 0;
        *pte = (curr_pa >> 12) << 10; // remove offset then make space for flags
        *pte = *pte | flags | PTE_V;


        curr_va += PAGE_SIZE;
        curr_pa += PAGE_SIZE;
    } while(curr_va <= va_last);

    KLOG_INFO("vmmap: pages from %p to %p (total %d) mapped from %p flags %d", va, va + size, size >> 12, pa, flags);
}


/*
 * maps size bytes of virtual memory starting from va 
 * to addresses starting from pa in physical memory
 */
void vmmap(pagetable pt, uint64 va, uint64 pa, uint64 size, uint32 flags) {
    spinlock_acquire(&k_pt_lock);

    vmmap_internal(pt, va, pa, size, flags);

    spinlock_release(&k_pt_lock);
}

void vmmap_kern_internal(uint64 va, uint64 pa, uint64 size, uint32 flags) {
    return vmmap_internal(k_pagetable, va, pa, size, flags);
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
    KLOG_INFO("kvm: initializing virtual memory");
    spinlock_init(&k_pt_lock, "kpt");

    k_pagetable = kearly_alloc_page();
    memsetb(k_pagetable, PAGE_SIZE, 0);

    // map kernel text
    vmmap(k_pagetable, (uint64)KERNEL_BEGIN, (uint64)KERNEL_BEGIN, KERNEL_TEXT_END-KERNEL_BEGIN, PTE_R | PTE_X);

    // map kernel data as rw
    vmmap(k_pagetable, (uint64)KERNEL_TEXT_END, (uint64)KERNEL_TEXT_END, KERNEL_END - KERNEL_TEXT_END, PTE_R | PTE_W);

    // map early_alloc as rw
    vmmap(k_pagetable, (uint64)KERNEL_END, (uint64)KERNEL_END, EARLY_ALLOC_SIZE, PTE_R | PTE_W);

    // map CLINT
    vmmap(k_pagetable, CLINT_BASE_ADDR, CLINT_BASE_ADDR, 16 * PAGE_SIZE, PTE_R | PTE_W);

    kvm_map_devices();
}

void kvm_mmu_enable() {

    KLOG_INFO("kvm: turning on paging for cpu%d", cpuid());
    register uint64 satp = MAKE_SATP((uint64)k_pagetable, 0x0, 0x8);

    sfence_vma();

    w_satp(
        satp
    );

    sfence_vma();
    KLOG_INFO("kvm: paging enabled for cpu%d", cpuid());
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

bool is_kvm_mmu_enabled() {
    return r_satp() ? 1 : 0;
}

void vm_debug_translate(pagetable pt, uint64 va) {
    va >>= 12; // we can safely ignore the offset to get the PTE
    
    uint64 pte, pdi;
    pagetable current_pt = pt;

    for(ushort i = 2; i > 0; --i) {
        pdi = (va >> (9*i)) & ((1 << 9) - 1);
        pte = current_pt[pdi];

        KLOG_INFO("va %p level %d pdi %d pte %x valid %d", va, i, pdi, CONV_PTE_PTA(pte), IS_PTE_VALID(pte));

        if(IS_PTE_VALID(pte)) {
            current_pt = CONV_PTE_PTA(pte);
            continue;
        }

        KLOG_INFO("va %x unmapped", va);
        return;
    }
    
    pdi = (va) & ((1 << 9) - 1); // L0
    pte = current_pt[pdi];

    KLOG_INFO("va %p level 0 pdi %d pte %x valid %d", va, pdi, CONV_PTE_PTA(pte), IS_PTE_VALID(pte));
}

