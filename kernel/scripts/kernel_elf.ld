OUTPUT_ARCH(i386:x86-64);
ENTRY(start_kernel);

MEMORY
{
    HEAD(rwx) : ORIGIN = 0x00000000, LENGTH = 900k
    BODY(rwx) : ORIGIN = 0x00110000, LENGTH = 1m
}

SECTIONS
{
    .body : {
        *(.text)
        *(.rodata*)
        *(.data*)
        __bss_start = .;
        *(.bss)
        __bss_end = .;
    } > BODY

    /DISCARD/ : {*(.eh_frame)}
}

