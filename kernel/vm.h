#pragma once

#include "types.h"

#define PTE_V (1)       // valid
#define PTE_R (1 << 1)  // readable
#define PTE_W (1 << 2)  // writable
#define PTE_X (1 << 3)  // executable
#define PTE_U (1 << 4)  // user
#define PTE_G (1 << 5)  // global
#define PTE_A (1 << 6)  // accessed
#define PTE_D (1 << 7)  // dirty
// bits 8 and 9 available for use

/*
 * array of 512 page table entries
 */
typedef uint64* pagetable;

void kvm_init();
void vmmap(pagetable pt, uint64 va, uint64 pa, uint64 size, uint32 flags);
uint64 vm_translate_pa(pagetable pt, uint64 va);

void kvm_enable_paging();

// internal
uint64* vm_get_setup_pte(pagetable pt, uint64 va, bool setup);
