#include "uart.h"
#include "kconfig.h"
#include "panic.h"

#include "ns16550.h"
#include "utils/string.h" // IWYU pragma: keep

ns16550 dev_uarts[NUARTS];

void uart_init() {
    // we should read devices from the dtb... hardcoded for now

    ns166550_init(&dev_uarts[0], "uart0", 0x10000000L);
}

void uart_putc(char* device_name, char c) {

    ns16550* dev = NULL;

    for(uint32 i = 0; i < NUARTS; ++i) {
        if(!strcmp(device_name, dev_uarts[i].name))
            dev = &dev_uarts[i];
    }

    if(!dev) {
        panic("device not found");
    }

    ns166550_putc_sync(dev, c);
}

void uart_transmit(char* device_name, char* data, uint64 len) {

    ns16550* dev = NULL;

    for(uint32 i = 0; i < NUARTS; ++i) {
        if(!strcmp(device_name, dev_uarts[i].name))
            dev = &dev_uarts[i];
    }

    if(!dev) {
        panic("device not found");
    }

    ns166550_transmit(dev, data, len);
}
