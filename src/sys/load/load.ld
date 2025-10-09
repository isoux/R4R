OUTPUT_FORMAT(elf32-i386)
ENTRY(start)

PHDRS
{
  text PT_LOAD FLAGS(0x5);   /* R + X */
  rodata PT_LOAD FLAGS(0x4); /* R */
  data PT_LOAD FLAGS(0x6);   /* R + W */
}

SECTIONS
{
  . = 0x100000;

  .text : ALIGN(4K)
  {
    *(.multiboot)
    *(.text)
  } :text

  .rodata : ALIGN(4K)
  {
    *(.rodata*)
  } :rodata

  .data : ALIGN(4K)
  {
    *(.data)
  } :data

  .bss : ALIGN(4K)
  {
    *(.bss)
    *(COMMON)
  } :data
  
  /* init = 0x200000; */
}
