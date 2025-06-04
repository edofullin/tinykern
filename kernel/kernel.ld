OUTPUT_ARCH("riscv")
ENTRY(_entry)



SECTIONS
{
  /*
   * ensure that entry.S / _entry is at 0x80000000,
   * where qemu's -kernel jumps.
   */
  . = 0x80000000;

  PROVIDE(KERNEL_BEGIN = .);

  .text : AT(0x80000000) {
    *(.text.startup)
    *(.text.*)
  }

  . = ALIGN(0x1000);
  PROVIDE(KERNEL_TEXT_END = .);

  .rodata : {
    . = ALIGN(16);
    *(.srodata .srodata.*)
    . = ALIGN(16);
    *(.rodata .rodata.*)
  }

  .data : {
    . = ALIGN(16);
    *(.sdata .sdata.*)
    . = ALIGN(16);
    *(.data .data.*)
  }

  .bss : {
    . = ALIGN(16);
    *(.sbss .sbss.*)
    . = ALIGN(16);
    *(.bss .bss.*)
  }

  PROVIDE(KERNEL_END = ALIGN(4096));
}