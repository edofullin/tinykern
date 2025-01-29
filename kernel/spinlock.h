#pragma once

#include "types.h"

typedef struct spinlock {
    char* name;
    bool locked;
    uint32 cpu; // valid only if locked == TRUE
} spinlock;

void spinlock_init(spinlock* lock, char* name);
void spinlock_acquire(spinlock* lock);
void spinlock_release(spinlock* lock);