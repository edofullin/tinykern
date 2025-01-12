/* This file is the first entrypoint for C */
#include "kconfig.h"

char early_stack[EARLY_STACK_SIZE * NCPU];

void centry() {
    /* we are in business */
    while(1) {}
}