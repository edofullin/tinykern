/* This file is the first entrypoint for C */
#include "kconfig.h"
#include "riscv.h"

extern void kearly_init();
extern void mtrap();


void centry() {
    /* we are in business baby */
    /* entry.S assembly jumps here */

    // set previous mode to supervisor
    uint64 ms = r_mstatus();
    ms &= ~MSTATUS_MPP_MASK;
    w_mstatus(ms | MSTATUS_MPP_S);

    // set supervisor return address to function kearly_initialize
    w_mepc((uint64)kearly_init);

    // disable paging
    w_satp(0);

    // disable memory protection
    w_pmpaddr0(0x3fffffffffffffull);
    w_pmpcfg0(PMPCFG_RWX_NAPOT);

    // enable machine mode timer interrupts
    w_mie(MIE_MTIE);
    w_mtvec((uint64)mtrap);

    // delegate all interrupts and exceptions to supervisor mode and enable supervisor interrupts.
    // w_medeleg(0xffff);
    w_mideleg(0xffff);
    w_mstatus(r_mstatus() | MSTATUS_SIE);

    // setup timer
    w_mtimecmp(r_mtime() + INTR_PERIOD);

    // set processor id into tp register
    w_tp(r_mhartid());

    // jump into supervisor mode
    asm volatile("mret");
}