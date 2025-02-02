/**
 * initializes the kernel
 * centry function will jump here into supervisor mode
 */


#include "cpu.h"
#include "kalloc.h"
#include "kio.h"
#include "panic.h"
#include "uart/uart.h"
#include "vm.h"

void kearly_init() {
    if(cpuid() == 0) {
        uart_init();
        kio_init();
        kprintf("tinykern is booting\n");

        kalloc_init();
        kvm_init();
        kvm_enable_paging();
    } else {


    }

    panic("boot complete in %d seconds\n", 10);
    while(1) {}
}

