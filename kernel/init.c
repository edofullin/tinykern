/**
 * initializes the kernel
 * centry function will jump here into supervisor mode
 */


#include "cpu.h"
#include "kalloc.h"
#include "kio.h"
#include "panic.h"
#include "riscv.h"
#include "uart/uart.h"
#include "vm.h"

void init_interrupts();
extern void strap();

void kearly_init() {
    if(cpuid() == 0) {
        uart_init();
        kio_init();
        kprintf("tinykern is booting\n");

        kalloc_init();
        kvm_init();
        kvm_mmu_enable();

        init_interrupts();
    } else {


    }

    kprintf("boot completed\n");
    while(1) {}
}

void init_interrupts() {
    w_sie(SIE_STIE | SIE_SSIE);

    uint64 stvec_val = ((uint64)strap);

    w_stvec(stvec_val);
}