#pragma once

#include "types.h"
#include "riscv_const.h"

// return core number from machine mode
static inline uint64 r_mhartid() {
    uint64 x;
    asm volatile("csrr %0, mhartid" : "=r" (x));
    return x;
}

static inline uint64 r_mstatus() {
    uint64 x;
    asm volatile("csrr %0, mstatus" : "=r" (x));
    return x;
}

static inline uint64 w_mstatus(uint64 s) {
    uint64 x;
    asm volatile("csrw mstatus, %0" : : "r" (s));
    return x;
}


static inline uint64 r_sie() {
  uint64 x;
  asm volatile("csrr %0, sie" : "=r" (x) );
  return x;
}

static inline __attribute__((always_inline)) void w_sie(uint64 x) {
  asm volatile("csrw sie, %0" : : "r" (x));
}

static inline uint64 r_sstatus() {
    uint64 x;
    asm volatile("csrr %0, sstatus" : "=r" (x));
    return x;
}

static inline void w_sstatus(uint64 s) {
    asm volatile("csrw sstatus, %0" : : "r" (s));
}

// Supervisor Address Translation and Protection (satp) Register
// Set to 0 to disable paging, otherwise set to top level page addr
static inline __attribute__((always_inline)) void w_satp(uint64 val) {
    asm volatile("csrw satp, %0" : : "r" (val));
}

// Memory protection

static inline uint64 r_pmpaddr0() {
    uint64 x;
    asm volatile("csrr %0, pmpaddr0" : "=r" (x));
    return x;
}

static inline uint64 w_pmpaddr0(uint64 s) {
    uint64 x;
    asm volatile("csrw pmpaddr0, %0" : : "r" (s));
    return x;
}

static inline uint64 r_pmpcfg0() {
    uint64 x;
    asm volatile("csrr %0, pmpcfg0" : "=r" (x));
    return x;
}

static inline uint64 w_pmpcfg0(uint64 s) {
    uint64 x;
    asm volatile("csrw pmpcfg0, %0" : : "r" (s));
    return x;
}

// TP

static inline uint64 r_tp() {
    uint64 x;
    asm volatile("mv %0, tp" : "=r" (x));
    return x;
}

static inline uint64 w_tp(uint64 s) {
    uint64 x;
    asm volatile("mv tp, %0" : : "r" (s));
    return x;
}

// Machine exception program counter.
static inline void w_mepc(uint64 x)
{
  asm volatile("csrw mepc, %0" : : "r" (x));
}

static inline uint64 r_mie() {
  uint64 x;
  asm volatile("csrr %0, mie" : "=r" (x) );
  return x;
}

static inline void w_mie(uint64 x) {
  asm volatile("csrw mie, %0" : : "r" (x));
}


static inline uint64 r_menvcfg() {
  uint64 x;
  asm volatile("csrr %0, menvcfg" : "=r" (x) );
  return x;
}

static inline void w_menvcfg(uint64 x) {
  asm volatile("csrw menvcfg, %0" : : "r" (x));
}

static inline uint64 r_mcounteren() {
  uint64 x;
  asm volatile("csrr %0, mcounteren" : "=r" (x) );
  return x;
}

static inline void w_mcounteren(uint64 x) {
  asm volatile("csrw mcounteren, %0" : : "r" (x));
}

// Supervisor Timer Comparison Register
static inline uint64 r_stimecmp() {
  uint64 x;
  asm volatile("csrr %0, stimecmp" : "=r" (x) );
  return x;
}

static inline void w_stimecmp(uint64 x) {
  asm volatile("csrw stimecmp, %0" : : "r" (x));
}

static inline uint64 r_mtime() {
  return *(uint64*)(0x200BFF8);
}

static inline void w_mtimecmp(uint64 x) {
  uint64 addr = CLINT_BASE_ADDR + 0x4000 + 8 * r_mhartid();
  asm volatile("sd %0, 0(%1)" : : "r"(x), "r"(addr));
}

// machine-mode cycle counter
static inline uint64 r_time() {
  uint64 x;
  asm volatile("csrr %0, time" : "=r" (x) );
  return x;
}

// Machine Exception Delegation
static inline uint64 r_medeleg() {
  uint64 x;
  asm volatile("csrr %0, medeleg" : "=r" (x) );
  return x;
}

static inline void w_medeleg(uint64 x) {
  asm volatile("csrw medeleg, %0" : : "r" (x));
}

// Machine Interrupt Delegation
static inline uint64 r_mideleg() {
  uint64 x;
  asm volatile("csrr %0, mideleg" : "=r" (x) );
  return x;
}

static inline void w_mideleg(uint64 x) {
  asm volatile("csrw mideleg, %0" : : "r" (x));
}

// flush the TLB.
static inline void sfence_vma() {
  asm volatile("sfence.vma zero, zero");
}

static inline uint64 r_pc() {
  uint64 pc;
  asm volatile("auipc %0, 0" : "=r" (pc));
  return pc;
}

static inline __attribute__((always_inline)) void w_stvec(uint64 addr) {
  asm volatile ("csrw stvec, %0" : : "r" (addr));
}

static inline __attribute__((always_inline)) void w_mtvec(uint64 addr) {
  asm volatile ("csrw mtvec, %0" : : "r" (addr));
}

static inline uint64 r_scause() {
  uint64 mc;
  asm volatile("csrr %0, scause" : "=r" (mc));
  return mc;
}

static inline void c_sip_stip() {
  asm volatile("li t0, 0x10"
               "csrc sip, t0");
}

static inline __attribute__((always_inline)) void nop() {
  asm volatile("nop");
}