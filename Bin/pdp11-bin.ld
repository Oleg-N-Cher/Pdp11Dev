OUTPUT_FORMAT("a.out-pdp11")
ENTRY(start)
phys = 00001000;
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
