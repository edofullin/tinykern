#pragma once

#define NCPU 1
#define NUARTS 2

#define KERNEL_OUT_UART "uart0"

#define EARLY_STACK_SIZE 1024
#define PAGE_SIZE 4096
#define PAGE_SIZE_BITS 12
#define KERNEL_DATA_SIZE 4096*128

#define INTR_PERIOD 1000000