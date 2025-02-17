/**
 * initializes the kernel
 * centry function will jump here into supervisor mode
 */


#include "cpu.h"
#include "interrupt.h"
#include "kalloc.h"
#include "kio.h"
#include "riscv.h"
#include "uart/uart.h"
#include "vm.h"

extern void strap();

void kearly_init() {
    if(cpuid() == 0) {
        uart_init();
        kio_init();
        kprintf("tinykern is booting\n");

        kalloc_init();
        kvm_init();
        kvm_mmu_enable();

        int_init();
        int_init_hart();
    } else {


    }

    kprintf("boot completed\n");
    while(1) {}
}