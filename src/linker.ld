/* linker.ld */
ENTRY(_start)
OUTPUT_FORMAT(elf64-x86-64)

SECTIONS {
  . = 1M;

  /* Le header Multiboot doit rester dans les 8 KiB initiaux */
  .multiboot : { KEEP(*(.multiboot)) }

  /* Code 32-bit du stub d'abord */
  .text32 : { *(.text32*) }

  /* Puis le code 64-bit du noyau */
  .text : { *(.text*) }

  .rodata : { *(.rodata*) }
  .data   : { *(.data*) }
  .bss    : { *(.bss*) *(COMMON) }
}
