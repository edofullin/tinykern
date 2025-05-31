/**
 * initializes the kernel
 * centry function will jump here into supervisor mode
 */


#include "cpu.h"
#include "interrupt.h"
#include "early_alloc.h"
#include "kconfig.h"
#include "kio.h"
#include "riscv.h"
#include "uart/uart.h"
#include "vm.h"
#include "page_alloc.h"
#include "log.h"

extern void strap();
extern void switch_stack(void* new_stack); // defined in init.S

void init_kernel_stack();
void kinit();

extern pagetable k_pagetable;


void kearly_init() {

    if(cpuid() == 0) {
        uart_init();
        kio_init();
        KLOG_INFO("tinykern is booting");

        kearly_alloc_init();
    }

    // switch from early stack to a bigger stack
    // this function cannot return
    init_kernel_stack();
    // kinit();
}

void kinit() {

    if(cpuid() == 0) {
        kvm_init();
        kvm_mmu_enable();

        kalloc_init();

        int_init();
        int_init_hart();
    }

    KLOG_INFO("boot completed");
    
    while(1) ;
}

void init_kernel_stack() {
    // called for every cpu
    void* new_stack = kearly_alloc_page() + PAGE_SIZE - 1; // stack grows downwards

    KLOG_INFO("init: setting up kernel stack at addr %x", new_stack);

    asm volatile(
        "mv sp, %[new_stack]\n"
        "jr %[j_kinit]\n"
        :
        :   [new_stack] "r"(new_stack),
            [j_kinit] "r"(kinit)
    );

    // NEVER RETURNS
}