#include "spinlock.h"
#include "cpu.h"
#include "panic.h"

void spinlock_init(spinlock* lock, char* name) {

    lock->name = name;
    lock->locked = FALSE;
    lock->cpu = 0;

}

void spinlock_acquire(spinlock* lock) {

    // TODO disable interrupts

    if(lock->locked && lock->cpu == cpuid()) {
        panic("cpu cannot hold lock %s twice", lock->name);
        // no exec
    }

    while(__sync_lock_test_and_set(&lock->locked, TRUE) != 0)
        ;

    __sync_synchronize();

    lock->cpu = cpuid();
}

void spinlock_release(spinlock* lock) {

    if(!lock->locked) {
        panic("cannot release unlocked lock");
        // no exec
    }

    lock->cpu = 0;

    __sync_lock_release(&lock->locked);

    // TODO reenable interrupts
}