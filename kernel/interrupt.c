#include "interrupt.h"
#include "kconfig.h"
#include "kio.h"
#include "panic.h"
#include "riscv_const.h"
#include "spinlock.h"
#include "types.h"
#include "riscv.h"
#include "utils/mem.h"

extern void strap();

// interrupt stack (> 0 int off)
uint16 int_stack = 0;
spinlock lk;

// strap.S will jump here
void int_strap2() {
    
    // TODO
    kprintf("TODO\n");
    
    return;
}

void int_init() {
    int_stack = 0;
    spinlock_init(&lk, "int_stack");
}

void int_init_hart() {
    w_sie(SIE_SSIE | SIE_SEIE | SIE_STIE);
    w_stvec((uint64)strap);
}

void int_off() {
    spinlock_acquire(&lk);
    int_stack++;
    w_sie(r_sie() & ~(SIE_SSIE | SIE_SEIE | SIE_STIE));
    spinlock_release(&lk);
}

void int_on() {
    spinlock_acquire(&lk);
    if(int_stack-- == 0) {
        spinlock_release(&lk);
        panic("interrupts already enabled");
    } else {
        w_sie(SIE_SSIE | SIE_SEIE | SIE_STIE);
        spinlock_release(&lk);
    }
}

bool int_status() {
    return int_stack > 0 ? FALSE : TRUE;
}