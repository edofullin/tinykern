#pragma once

#include "types.h"

void uart_init();
void uart_transmit(char* device_name, char* data, uint64 len);