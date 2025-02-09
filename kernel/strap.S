#include "riscv_const.h"

.section .text
.global strap
.global int_strap2
.align 4
strap:
    addi sp, sp, -0xf8
    sd x1, 0x00(sp) // ra
    sd x2, 0x08(sp) // sp
    sd x3, 0x10(sp) // gp
    sd x4, 0x18(sp) // tp
    sd x5, 0x20(sp) // t0
    sd x6, 0x28(sp) // t1
    sd x7, 0x30(sp) // t2
    sd x8, 0x38(sp) // s0 fp
    sd x9, 0x40(sp) // s1
    sd x10, 0x48(sp) // a0
    sd x11, 0x50(sp) // a1
    sd x12, 0x58(sp) // a2
    sd x13, 0x60(sp) // a3
    sd x14, 0x68(sp) // a4
    sd x15, 0x70(sp) // a5
    sd x16, 0x78(sp) // a6
    sd x17, 0x80(sp) // a7
    sd x18, 0x88(sp) // s2
    sd x19, 0x90(sp) // s3
    sd x20, 0x98(sp) // s4
    sd x21, 0xa0(sp) // s5
    sd x22, 0xa8(sp) // s6
    sd x23, 0xb0(sp) // s7
    sd x24, 0xb8(sp) // s8
    sd x25, 0xc0(sp) // s9
    sd x26, 0xc8(sp) // s10
    sd x27, 0xd0(sp) // s11
    sd x28, 0xd8(sp) // t3
    sd x29, 0xe0(sp) // t4
    sd x30, 0xe8(sp) // t5
    sd x31, 0xf0(sp) // t6


    call int_strap2


    li t0, MIP_SSIP
    csrc sip, t0 # clear flag for supervisor
    
    ld x1, 0x00(sp) // ra
    ld x2, 0x08(sp) // sp
    ld x3, 0x10(sp) // gp
    ld x4, 0x18(sp) // tp
    ld x5, 0x20(sp) // t0
    ld x6, 0x28(sp) // t1
    ld x7, 0x30(sp) // t2
    ld x8, 0x38(sp) // s0 fp
    ld x9, 0x40(sp) // s1
    ld x10, 0x48(sp) // a0
    ld x11, 0x50(sp) // a1
    ld x12, 0x58(sp) // a2
    ld x13, 0x60(sp) // a3
    ld x14, 0x68(sp) // a4
    ld x15, 0x70(sp) // a5
    ld x16, 0x78(sp) // a6
    ld x17, 0x80(sp) // a7
    ld x18, 0x88(sp) // s2
    ld x19, 0x90(sp) // s3
    ld x20, 0x98(sp) // s4
    ld x21, 0xa0(sp) // s5
    ld x22, 0xa8(sp) // s6
    ld x23, 0xb0(sp) // s7
    ld x24, 0xb8(sp) // s8
    ld x25, 0xc0(sp) // s9
    ld x26, 0xc8(sp) // s10
    ld x27, 0xd0(sp) // s11
    ld x28, 0xd8(sp) // t3
    ld x29, 0xe0(sp) // t4
    ld x30, 0xe8(sp) // t5
    ld x31, 0xf0(sp) // t6
    addi sp, sp, 0xf8

    sret