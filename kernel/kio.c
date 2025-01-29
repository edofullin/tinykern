#include "kio.h"
#include "uart/uart.h"

#include "utils/string.h"

#define UART_LOG "uart0"

uint32 kprintf(char* fmt, ...) {
    uint64 len = strlen(fmt);

    uart_transmit(UART_LOG, fmt, len);
    return 0;
}