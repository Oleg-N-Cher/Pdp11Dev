OUTPUT_FORMAT("a.out-pdp11")
ENTRY(start)
SECTIONS
{
. = 00001000;
.text :
{
*(.text)
*(.rodata)
*(.data)
}
.data : { *(.data) }
.bss : { *(.bss) }
}
