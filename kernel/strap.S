.section .text
.global strap
.align 4
strap:
    addi sp, sp, -64
    sd t0, 0x0(sp)
    sd a0, 0x8(sp)

    li t0, 0x02
    csrc sip, t0 # clear flag for supervisor
    
    ld t0, 0x0(sp)
    ld a0, 0x8(sp)
    addi sp, sp, 64

    sret