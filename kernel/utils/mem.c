#include "mem.h"

void memsetb(void *from, uint64 count, byte val) {
    byte *fromv = (byte *)from;
    for (uint64 i = 0; i < count; i++)
        fromv[i] = val;
}