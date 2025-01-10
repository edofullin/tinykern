/* This file is the first entrypoint for C */
#include "kconfig.h"

char early_stack[1024 * NCPU];

void centry() {
    /* we are in business */
    while(1) {}
}