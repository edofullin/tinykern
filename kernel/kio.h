#pragma once

#include "types.h"
#include <stdarg.h>

uint32 KLOG_INFO(char* fmt, ...);
uint32 kfprintf(char* fmt, va_list);

void kio_init();