/**
 * initializes the kernel
 * centry function will jump here into supervisor mode
 */

#include "riscv.h"
#include "kconfig.h"

#include "cpu.h"
#include "kio.h"
#include "uart/ns16550.h"
#include "uart/uart.h"

extern ns16550 dev_uarts[];

void kearly_init() {
    if(cpuid() == 0) {
        uart_init();
        kprintf("tinykern is booting...");
    }

    while(1) {}
}

