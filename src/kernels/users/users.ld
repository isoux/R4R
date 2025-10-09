OUTPUT_FORMAT(elf32-i386)
ENTRY(setup_users)

PHDRS
{
  text PT_LOAD FLAGS(0x5);   /* R + X */
  rodata PT_LOAD FLAGS(0x4); /* R */
  data PT_LOAD FLAGS(0x6);   /* R + W */
}

SECTIONS
{
    . = 0x007af000;
    __devs_start = .;
    . = ALIGN(0x1000);

    __text_start = .;
    .text : ALIGN(0x1000)
    {
        *(.text.users_entry)
        *(.text*)
    } :text
    __text_end = .;

    . = ALIGN(0x1000);
    __rodata_start = .;
    .rodata : ALIGN(0x1000)
    {
        *(.rodata*)
    } :rodata
    __rodata_end = .;

    . = ALIGN(0x1000);
    __data_start = .;
    .data : ALIGN(0x1000)
    {
        *(.data*)
    } :data
    __data_end = .;

    . = ALIGN(0x1000);
    __bss_start = .;
    .bss : ALIGN(0x1000)
    {
        *(.bss*)
        *(COMMON)
    } :data
    __bss_end = .;
    
    /DISCARD/ : { *(.note .comment .eh_frame) *(.got.plt) *(.got) *(.plt) }
}
