add-symbol-file kernel/kImage 0x80000000
b kearly_init
target remote | qemu-system-riscv64 -gdb stdio -cpu rv64 -machine virt -bios kernel/kImage -smp 1 -m 128M