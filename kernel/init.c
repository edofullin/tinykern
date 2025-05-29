/**
 * initializes the kernel
 * centry function will jump here into supervisor mode
 */


#include "cpu.h"
#include "interrupt.h"
#include "early_alloc.h"
#include "kio.h"
#include "riscv.h"
#include "uart/uart.h"
#include "vm.h"
#include "page_alloc.h"

extern void strap();

void kearly_init() {
    if(cpuid() == 0) {
        uart_init();
        kio_init();
        kprintf("tinykern is booting\n");

        kearly_alloc_init();
        kvm_init();
        kvm_mmu_enable();

        kalloc_init();

        int_init();
        int_init_hart();
    } else {


    }

    kalloc_pages(1);
    kalloc_pages(2);
    kprintf("boot completed\n");
    while(1) {}
}