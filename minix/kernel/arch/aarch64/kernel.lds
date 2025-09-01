OUTPUT_ARCH("aarch64")
ENTRY(MINIX)

SECTIONS
{
    /* Place kernel at a reasonable physical base; virtual mapping TBD */
    . = 0x00080000;

    .startup : { *(.startup) }
    .text    : { *(.text*) }
    _etext = .;
    .rodata  : { *(.rodata*) }
    .data    : { *(.data*) }
    _edata = .;
    .bss     : { *(.bss*) *(COMMON) }
    _end = .;

    /DISCARD/ : { *(.eh_frame*) *(.note.GNU-stack) }
}

