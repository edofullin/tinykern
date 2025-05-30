#include "panic.h"
#include "interrupt.h"
#include "types.h"
#include "kio.h"
#include <stdarg.h>

void panic(char* fmt, ...) {
    va_list args;

    int_off();

    va_start(args, fmt);

    kprintf("PANIC: ");
    kfprintf(fmt, args);
    kprintf("\n");

    va_end(args);


    while(TRUE)
        ;
}