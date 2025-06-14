#pragma once

// PHYSICAL ADDR
#define PA_DRAM_START 0x80000000
#define PA_DRAM_END (PA_DRAM_START + (1ull << 27)) // SIZE 128MiB

extern char KERNEL_BEGIN[];
extern char KERNEL_END[];

// VIRTUAL ADDR
#define VA_KERNEL_START (0xffffffff - (1ull << 10)) // LEAVE 1 PAGE AT THE BEGINNING
#define VA_KERNEL_END (VA_KERNEL_START - (1ull << 12)) // SIZE 4 kiB
#define VA_KERNEL_SIZE (VA_KERNEL_START - VA_KERNEL_END)





