#pragma once

#define CLINT_BASE_ADDR 0x2000000

#define CLINT_BASE_ADDR 0x2000000
#define CLINT_MTIMECMP_ADDR 0x2004000
#define CLINT_MTIME_ADDR 0x200BFF8

// Machine status (only in machine mode)
#define MSTATUS_MPP_MASK (3L << 11)
#define MSTATUS_MPP_M (3L << 11)
#define MSTATUS_MPP_S (1L << 11)
#define MSTATUS_MPP_U (0L << 11)
#define MSTATUS_MIE (1L << 3)    // machine-mode interrupt enable.
#define MSTATUS_SIE (1L << 1)    // machine-mode interrupt enable.

// Supervisor Interrupt Enable
#define SIE_SEIE (1L << 9) // external
#define SIE_STIE (1L << 5) // timer
#define SIE_SSIE (1L << 1) // software

// Supervisor status

#define SSTATUS_SPP (1L << 8)  // Previous mode, 1=Supervisor, 0=User
#define SSTATUS_SPIE (1L << 5) // Supervisor Previous Interrupt Enable
#define SSTATUS_UPIE (1L << 4) // User Previous Interrupt Enable
#define SSTATUS_SIE (1L << 1)  // Supervisor Interrupt Enable
#define SSTATUS_UIE (1L << 0)  // User Interrupt Enable

#define PMPCFG_RWX_NAPOT 0xf

// Machine Interrupt Enable
#define MIE_STIE (1L << 5)  // supervisor timer
#define MIE_MTIE (1L << 7)  // machine timer

// Machine Environment Configuration Register
#define MENVCFG_STCE (1L << 63L)

// Machine Counter-Enable Register
// The counter-enable register mcounteren is a 32-bit register that controls the availability of the hardware performance-monitoring counters to the next-lower privileged mode.

#define MCOUNTEREN_TIME (1 << 1L)

#define MIP_SSIP (1 << 1)