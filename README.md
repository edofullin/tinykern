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

├── .devcontainer/      # Development container for vscode (includes toolchain and qemu)
├── kernel/             # Kernel code (C and Assembly)
├── scripts/            # Useful scripts
├── CMakeLists.txt      # Global CMakeLists
└── README.md

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