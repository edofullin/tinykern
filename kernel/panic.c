#include "panic.h"
#include "types.h"
#include "kio.h"
#include <stdarg.h>

void panic(char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    KLOG_INFO("PANIC: ");
    kfprintf(fmt, args);

    va_end(args);


    while(TRUE)
        ;
}