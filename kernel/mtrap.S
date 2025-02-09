#include "riscvconst.h"
#include "kconfig.h"

.section .text
.global mtrap
.align 4
mtrap:
    # save registers
    addi sp, sp, -64
    sd t0, 0x0(sp)
    sd t1, 0x8(sp)
    sd t2, 0x10(sp)
    sd t3, 0x18(sp)
    sd t4, 0x20(sp)
    sd t5, 0x28(sp)
    sd t6, 0x30(sp)
    sd a0, 0x38(sp)

    # check cause of interrupt
    csrr t0, mcause
    bgez t0, mtrap_exc # exception

    # update mtimecmp
    csrr t0, mhartid # get core id
    sll t0, t0, 3 # multiply by 8
    li t1, CLINT_MTIMECMP_ADDR # mtimecmp base addr
    add t0, t0, t1 # calculate hart addr

    li t1, CLINT_MTIME_ADDR # load mtime addr
    ld t1, 0(t1) # get mtime

    li t2, INTR_PERIOD
    add t1, t1, t2 # calculate mtime + INTR_PERIOD
    sd t1, 0(t0) # store new time in mtimecmp

    # set supervisor flag
    li t0, 0x02
    csrs mip, t0 # set flag for supervisor

    # restore the register and unwind frame
    ld t0, 0x0(sp)
    ld t1, 0x8(sp)
    ld t2, 0x10(sp)
    ld t3, 0x18(sp)
    ld t4, 0x20(sp)
    ld t5, 0x28(sp)
    ld t6, 0x30(sp)
    ld a0, 0x38(sp)
    addi sp, sp, 64

    mret

# exception handler
mtrap_exc:
    # TODO not implemented
    j mtrap_exc