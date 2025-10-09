OUTPUT_FORMAT(elf32-i386)
ENTRY(setup_core)

PHDRS
{
  text PT_LOAD FLAGS(0x5);   /* R + X */
  rodata PT_LOAD FLAGS(0x4); /* R */
  data PT_LOAD FLAGS(0x6);   /* R + W */
}

SECTIONS
{
    . = 0x007df000;
    __core_start = .;
    . = ALIGN(0x1000);

    __text_start = .;
    .text : ALIGN(0x1000)
    {
        *(.text.core_entry)
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
    
    . = 0x007ef000;
    __idt_start = .;
    __core_end = .;
    . = . + 0x1000;
    __idt_end = .;

    . = 0x007f0000;
    __gdt_start = .;
    . = . + 0x10000;
    __gdt_end = .;
    
    
    /DISCARD/ : { *(.note .comment .eh_frame) *(.got.plt) *(.got) *(.plt) }
}