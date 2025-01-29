/* This file is the first entrypoint for C */
#include "kconfig.h"
#include "riscv.h"

extern void kearly_init();
void timer_early_init();

__attribute__ ((aligned(16)))
char early_stack[EARLY_STACK_SIZE * NCPU];

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

    // delegate all interrupts and exceptions to supervisor mode.
    w_medeleg(0xffff);
    w_mideleg(0xffff);
    w_sie(r_sie() | SIE_SEIE | SIE_STIE | SIE_SSIE);

    // disable memory protection
    w_pmpaddr0(0x3fffffffffffffull);
    w_pmpcfg0(PMPCFG_RWX_NAPOT);

    // initialize early timer
    timer_early_init();

    // set processor id into tp register
    w_tp(r_mhartid());

    // jump into supervisor mode
    asm volatile("mret");
}

void timer_early_init() {
    
    // enable supervisor mode timer interrupts
    w_mie(r_mie() | MIE_STIE);

    // enable sstc entension (timecmp w/o jumping to machine mode)
    w_menvcfg(r_menvcfg() | MENVCFG_STCE);

    // allow access to time from S mode
    w_mcounteren(r_mcounteren() | MCOUNTEREN_TIME);

    // setup timer
    w_stimecmp(r_time() + 10000);
}