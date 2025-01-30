#include "console.h"
#include "kconfig.h"
#include "uart/uart.h"

void console_printc(char c) {
    uart_putc(KERNEL_OUT_UART, c);
}