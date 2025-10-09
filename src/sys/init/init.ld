OUTPUT_FORMAT(elf32-i386)
ENTRY(init)

PHDRS
{
  text PT_LOAD FLAGS(0x5);   /* R + X */
  rodata PT_LOAD FLAGS(0x4); /* R */
  data PT_LOAD FLAGS(0x6);   /* R + W */
}

SECTIONS
{
    . = 0x200000;

    .text :
    {
        *(.text*)
    } :text

    .rodata :
    {
        *(.rodata*)
    } :rodata

    .data :
    {
        *(.data*)
    } :data

    .bss :
    {
        *(.bss*)
        *(COMMON)
    } :data

    /DISCARD/ : { *(.note .comment .eh_frame) }
}

