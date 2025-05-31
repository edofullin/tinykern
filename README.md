# Tinykern RISC-V

*A minimalist educational kernel to learn OS development.*

TinyKernel is a barebones operating system kernel developed as a student project. Its goal is to demonstrate the fundamental concepts of OS design — booting, memory management, task switching, and basic I/O.

For now only working on the `virt` machine via QEMU.

---

## Status

| Feature                   | Status         | Notes                      |
|---------------------------|----------------|----------------------------|
| Early boot into C         | ✅ Implemented |                          |
| Memory paging (basic)     | ✅ Implemented |                          |
| UART simple driver        | ✅ Implemented |                          |
| Timer interrupt           | ✅ Implemented |                          |
| Pages allocator (basic)   | ✅ Implemented |                          |
| Kernel heap allocator     | 🔄 In progress |                          |
| Task creation             | 🔲 Planned     |                          |
| Task switching            | 🔲 Planned     |                          |
| System calls (basic)      | 🔲 Planned     |                          |
| Synchronization primitives| 🔲 Planned     |                          |
| Simple file system        | 🔲 Planned     |                          |

And then I'll see.

---

## Project Structure

```
├── .devcontainer/      # Development container for vscode (includes toolchain and qemu)
├── kernel/             # Kernel code (C and Assembly)
├── scripts/            # Useful scripts
├── CMakeLists.txt      # Global CMakeLists
└── README.md
```

## Getting Started

### Requirements

- GCC toolchain cross-compiler targeting `rv64ig`
- `qemu` (or any x86 emulator)
- `cmake`
- Linux (recommended) or WSL

### Build & Run

```bash
# Configure
mkdir build; cd build;

cmake -DCMAKE_TOOLCHAIN_FILE=../scripts/riscv64_debug.cmake ..

# Build the kernel
make

# Run with QEMU
make qemu
continue # in qemu console

# Attach GDB
riscv64-unknown-linux-gnu-gdb --command ../scripts/qemu.gdb
```

## Current progress

Bootlog:

```
tinykern is booting
early_alloc: initializing from 0x80005000 to 0x80105000
init: setting up kernel stack at addr 0x80005fff
kvm: initializing virtual memory
vmmap: pages from 0x80000000 to 0x80002000 (total 2) mapped from 0x80000000 flags 10
vmmap: pages from 0x80002000 to 0x80005000 (total 3) mapped from 0x80002000 flags 6
vmmap: pages from 0x80005000 to 0x80105000 (total 256) mapped from 0x80005000 flags 6
vmmap: pages from 0x2000000 to 0x2010000 (total 16) mapped from 0x2000000 flags 6
vmmap: pages from 0x10000000 to 0x10001000 (total 1) mapped from 0x10000000 flags 6
kvm: turning on paging for cpu0
kvm: paging enabled for cpu0
page_alloc: initializing from 0x0 size 32768 pages total size 128 MiB
page_alloc: first free page at addr 0x80106000
interrupt: initializing global
interrupt: initializing hart 0
boot completed
TODO hart 0
TODO hart 0
TODO hart 0
```