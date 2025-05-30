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
#include "log.h"

extern void strap();

void kearly_init() {
    if(cpuid() == 0) {
        uart_init();
        kio_init();
        KLOG_INFO("tinykern is booting");

        kearly_alloc_init();
        kvm_init();
        kvm_mmu_enable();

        kalloc_init();

        int_init();
        int_init_hart();
    } else {


    }

    int x = *(int*)kalloc_pages(3);

    KLOG_INFO("boot completed %d", x);
    while(1) {}
}