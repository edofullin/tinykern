#pragma once

#include "types.h"
#include "uart/ns16550.h"

void uart_init();
void uart_transmit(char* device_name, char* data, uint64 len);
void uart_putc(char* device_name, char c);

ns16550* get_uart_devices();