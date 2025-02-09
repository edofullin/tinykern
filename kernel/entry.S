#include "kconfig.h"

.section .text.startup
.global _entry
_entry:
    la sp, early_stack # load stack address (see centry.c)
    csrr a1, mhartid # get processor number
    addi a1, a1, 1 # stack grows downward
    li a2, EARLY_STACK_SIZE # load stack size
    mul a1, a1, a2 # calculate offset
    add sp, sp, a1 # set sp = early_stack + cpuid * 1024
    j centry # jump into C
spin:
    # should never reach
    j spin