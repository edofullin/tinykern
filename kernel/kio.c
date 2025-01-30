#include "kio.h"
#include "console.h"
#include "spinlock.h"
#include "types.h"
#include "spinlock.h"
#include <stdarg.h>

#include "utils/string.h"

#define UART_LOG "uart0"

spinlock lock;

void kio_init() {
    lock.locked = FALSE;
    lock.name = "kio";
}

void _print_c(char c) {
    console_printc(c);
}

void _print_n(int64 i, uint32 base, bool sign) {

    char buff[16];
    uint64 num, j = 0;
    bool is_negative = FALSE;
    char digits[] = "0123456789abcdef";

    if(sign && i < 0) {
        num = -i;
        is_negative = TRUE;
    } else {
        num = i;
    }

    do {
        buff[j] = digits[num % base];
        num /= base;
        j++;
    } while(num != 0);

    if(is_negative)
        buff[j] = '-';

    for(;; j--) {
        _print_c(buff[j]);
        if(j == 0) break;
    }
}

void _print_p(void* ptr) {
    _print_c('0'); _print_c('x');

    _print_n((uint64)ptr, 16, FALSE);
}

void _print_s(char* s) {
    for(;*s;s++)
        _print_c(*s);
}

uint32 kprintf(char* fmt, ...) {
    va_list va;
    char c, c1;
    uint32 i = 0;

    spinlock_acquire(&lock);

    va_start(va, fmt);

    for(i = 0; (c = fmt[i]) != NULL; ++i) {
        
        // just print the character
        if(c != '%') {
            _print_c(c);
            continue;
        }

        // type of argument
        i++;
        c1 = fmt[i];

        switch(c1) {
            case 'l':
            case 'd':
                _print_n(va_arg(va, int64), 10, TRUE);
                break;
            case 'u':
                _print_n(va_arg(va, uint64), 10, TRUE);
            case 'p':
                _print_p((void*)va_arg(va, uint64));
                break;
            case 'c':
                _print_c(va_arg(va, char));
                break;
            case 's':
                _print_s(va_arg(va, char*));
                break;
            default:
                _print_s("???");
                break;
        }
    }

    spinlock_release(&lock);

    return i;
}