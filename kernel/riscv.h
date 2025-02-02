#pragma once

#include "types.h"

// return core number from machine mode
static inline uint64 r_mhartid() {
    uint64 x;
    asm volatile("csrr %0, mhartid" : "=r" (x));
    return x;
}

// Machine status (only in machine mode)
#define MSTATUS_MPP_MASK (3L << 11)
#define MSTATUS_MPP_M (3L << 11)
#define MSTATUS_MPP_S (1L << 11)
#define MSTATUS_MPP_U (0L << 11)
#define MSTATUS_MIE (1L << 3)    // machine-mode interrupt enable.

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

// Supervisor Interrupt Enable
#define SIE_SEIE (1L << 9) // external
#define SIE_STIE (1L << 5) // timer
#define SIE_SSIE (1L << 1) // software


static inline uint64 r_sie() {
  uint64 x;
  asm volatile("csrr %0, sie" : "=r" (x) );
  return x;
}

static inline void w_sie(uint64 x) {
  asm volatile("csrw sie, %0" : : "r" (x));
}

// Supervisor status

#define SSTATUS_SPP (1L << 8)  // Previous mode, 1=Supervisor, 0=User
#define SSTATUS_SPIE (1L << 5) // Supervisor Previous Interrupt Enable
#define SSTATUS_UPIE (1L << 4) // User Previous Interrupt Enable
#define SSTATUS_SIE (1L << 1)  // Supervisor Interrupt Enable
#define SSTATUS_UIE (1L << 0)  // User Interrupt Enable

static inline uint64 r_sstatus() {
    uint64 x;
    asm volatile("csrr %0, sstatus" : "=r" (x));
    return x;
}

static inline void w_sstatus(uint64 s) {
    asm volatile("csrr sstatus, %0" : : "r" (s));
}

// Supervisor Address Translation and Protection (satp) Register
// Set to 0 to disable paging, otherwise set to top level page addr
static inline void w_satp(uint64 val) {
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

#define PMPCFG_RWX_NAPOT 0xf

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

// Machine Interrupt Enable
#define MIE_STIE (1L << 5)  // supervisor timer
static inline uint64 r_mie() {
  uint64 x;
  asm volatile("csrr %0, mie" : "=r" (x) );
  return x;
}

static inline void w_mie(uint64 x) {
  asm volatile("csrw mie, %0" : : "r" (x));
}

// Machine Environment Configuration Register
#define MENVCFG_STCE (1L << 63L)
static inline uint64 r_menvcfg() {
  uint64 x;
  asm volatile("csrr %0, menvcfg" : "=r" (x) );
  return x;
}

static inline void w_menvcfg(uint64 x) {
  asm volatile("csrw menvcfg, %0" : : "r" (x));
}

// Machine Counter-Enable Register
// The counter-enable register mcounteren is a 32-bit register that controls the availability of the hardware performance-monitoring counters to the next-lower privileged mode.

#define MCOUNTEREN_TIME (1 << 1L)

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
