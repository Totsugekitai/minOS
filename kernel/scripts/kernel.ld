/*OUTPUT_FORMAT("binary");*/
OUTPUT_FORMAT("elf64-x86-64");

ENTRY(start_kernel);

MEMORY
{
    EXEC(r) : ORIGIN = 0x00000000, LENGTH = 1m
    RAM(wx) : ORIGIN = 0x00110000, LENGTH = 1m
}

SECTIONS
{
    .header : {
        QUAD(__bss_start)
        QUAD(__bss_end - __bss_start)
    } > EXEC

    .body : {
        *(.text)

        *(.rodata)

        *(.data)

        __bss_start = .;
        *(.bss)
        __bss_end = .;

        *(.eh_frame)
    } > RAM AT> EXEC
}

