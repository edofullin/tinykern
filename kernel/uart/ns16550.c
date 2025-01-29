#include "ns16550.h"
#include "spinlock.h"

void ns166550_init(ns16550* dev, char* name, uint64 baseaddr) {

    dev->name = name;
    dev->baseaddr = baseaddr;
    
    spinlock_init(&dev->lock, "uartlock");
    spinlock_acquire(&dev->lock);

    // set DLAB mode to set speed
    wReg(dev, LCR, LCR_DLAB);

    // set speed of 38.4k (TODO check)
    wReg(dev, 0x0, 0x3);
    wReg(dev, 0x1, 0x0);

    // exists DLAB and set len to 8bits
    wReg(dev, LCR, LCR_LEN_8B);

    // enables fifo tx
    wReg(dev, FCR, FCR_FIFO_ENA | FCR_FIFO_RST);

    spinlock_release(&dev->lock);
}

void ns166550_transmit(ns16550* dev, char* data, uint64 len) {

    for(uint64 i = 0; i < len; ++i)
        ns166550_putc_sync(dev, data[i]);

}

void ns166550_putc_sync(ns16550* dev, char c) {
    spinlock_acquire(&dev->lock);

    while((rReg(dev, LSR) & LSR_THR_EMPTY) == 0)
        ;

    wReg(dev, THR, c);

    spinlock_release(&dev->lock);
}