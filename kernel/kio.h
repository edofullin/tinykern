#pragma once

#include "types.h"
#include <stdarg.h>

uint32 kprintf(char* fmt, ...);
uint32 kfprintf(char* fmt, va_list);

void kio_init();