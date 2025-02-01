#include "mem.h"

void memset(void *from, uint64 count, uint64 val) {
    uint64 *fromv = (uint64 *)from;
    for (; count >= 0; count--)
        fromv[count] = val;
}

void memsetb(void *from, uint64 count, byte val) {
    byte *fromv = (byte *)from;
    for (uint64 i = 0; i < count; i++)
        fromv[i] = val;
}