OUTPUT_FORMAT("a.out-pdp11")
ENTRY(start)
phys = 00000000;
SECTIONS
{
    .text phys : AT(phys)
    {
        code = .;
        *(.text)
        *(.rodata)
        . = ALIGN(128);
        *(.data);
    }
}
