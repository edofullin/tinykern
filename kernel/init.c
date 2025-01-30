/**
 * initializes the kernel
 * centry function will jump here into supervisor mode
 */

#include "console.h"
#include "riscv.h"
#include "kconfig.h"

#include "cpu.h"
#include "kio.h"
#include "uart/ns16550.h"
#include "uart/uart.h"

void kearly_init() {
    if(cpuid() == 0) {
        uart_init();
        kio_init();

        kprintf("tinykern is booting...\n");
    }

    while(1) {}
}

