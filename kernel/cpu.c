#include "cpu.h"

uint32 cpuid() {
    uint64 ret;
    asm volatile("mv %0, tp" : "=r" (ret));
    return ret;
}
