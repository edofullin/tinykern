#include "kalloc.h"
#include "kio.h"
#include "panic.h"
#include "kconfig.h"
#include "spinlock.h"

// kreg_end set to the beginning of next page after the kernel space in kernel.ld
extern char KERNEL_END[];

void* alloc_last;

struct node {
    struct node* next;
};

spinlock ka_lock;

/*
 * list of free pages
 * every **free** page has the first 8B set to addr of next free page
 * last (free) page contains NULL
 */
struct node* freelist;

void* get_first_free_page() {
    struct node* curr = freelist;

    if(!curr)
        panic("kalloc: get_first_free_page invoked with a empty freelist\n");

    while(curr->next)
        curr = curr->next;

    return curr;
}

void kalloc_init() {
    spinlock_init(&ka_lock, "ka_lock");
    spinlock_acquire(&ka_lock);

    kprintf("kalloc initializing at address %p\n", KERNEL_END);
    freelist = NULL;
    alloc_last = KERNEL_END;

    spinlock_release(&ka_lock);
}

// allocated a new page
// TODO > 1 page should be done in a smarter way
void* kmallocp() {
    spinlock_acquire(&ka_lock);

    if(freelist) {
        void* page = get_first_free_page();
        spinlock_release(&ka_lock);
        return page;
    }

    void* curr = alloc_last;
    alloc_last += PAGE_SIZE;
    
    spinlock_release(&ka_lock);
    return curr;
}

void kmfree(void* ptr) {
    spinlock_acquire(&ka_lock);
    struct node* curr = (struct node*)freelist;
    struct node* freed = (struct node*)ptr;

    freed->next = curr;
    freelist = freed;

    spinlock_release(&ka_lock);
}