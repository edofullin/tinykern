/**
 * initializes the kernel
 * centry function will jump here into supervisor mode
 */

#include "riscv.h"
#include "kconfig.h"

#include "cpu.h"
#include "uart/ns16550.h"

extern ns16550 dev_uarts[];

void kearly_init() {
    if(cpuid() == 0) {
        ns166550_init(&dev_uarts[0], "uart0", 0x10000000L);
        ns166550_transmit(&dev_uarts[0], "tinykern is booting...\n", sizeof("tinykern is booting...\n"));
    }

    while(1) {}
}

