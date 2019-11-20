OUTPUT_FORMAT("binary");
OUTPUT_ARCH(i386:x86-64);
ENTRY(start_kernel);

MEMORY
{
    HEAD(rwx) : ORIGIN = 0x00000000, LENGTH = 900k
    BODY(rwx) : ORIGIN = 0x00110000, LENGTH = 128m
}

_heap_size = 0x4000000;  /* heap size = 64MiB */

SECTIONS
{
    .body : {
        *(.text)
        *(.rodata*)
        *(.data*)
        . = ALIGN(0x1000);
        __heap_start = .;
        . += _heap_size;
        __heap_end = .;
        __bss_start = .;
        *(.bss)
        __bss_end = .;
    } > BODY

    /DISCARD/ : {*(.eh_frame)}
}

