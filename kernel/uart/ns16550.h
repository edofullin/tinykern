#pragma once

#include "types.h"
#include "spinlock.h"

#define Reg(dev, reg) ((volatile unsigned char*)(dev->baseaddr + (reg)))
#define rReg(dev, reg) (*Reg(dev, reg))
#define wReg(dev, reg, val) ((*Reg(dev, reg)) = (val))

#define LCR 0x3
#define LCR_DLAB (0x1 << 7)
#define LCR_LEN_8B (0x3)

#define FCR 0x2
#define FCR_FIFO_ENA (0x1)
#define FCR_FIFO_RST (0x3 << 1)

#define LSR 0x5
#define LSR_THR_EMPTY (1 << 5)

#define THR 0x0

typedef struct ns16550 {
    char* name;
    uint64 baseaddr;

    spinlock lock;
} ns16550;

void ns166550_init(ns16550* dev, char* name, uint64 baseaddr);
void ns166550_transmit(ns16550* dev, char* data, uint64 len);

void ns166550_putc_sync(ns16550* dev, char c);



