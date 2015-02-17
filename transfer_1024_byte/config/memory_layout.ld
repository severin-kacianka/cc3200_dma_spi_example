HEAP_SIZE = 0x00010000;

MEMORY
{
    SRAM (rwx) : ORIGIN = 0x20004000, LENGTH = 0x0002C000
}

SECTIONS
{
    .text :
    {
        _text = .;
        KEEP(*(.intvecs))
        *(.text*)
        *(.rodata*)
	*(.ARM.extab* .gnu.linkonce.armextab.*)
	. = ALIGN(8);
        _etext = .;
    } > SRAM

    .ARM : {
    __exidx_start = .;
      *(.ARM.exidx*)
      __exidx_end = .;
    } > SRAM

    __init_data = .;

    .data : AT(__init_data)
    {
        _data = .;
        *(.data*)
	. = ALIGN (8);
        _edata = .;
    } > SRAM

    .bss :
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > SRAM

    .heap :
    {
	_heap = .;
	. = . + HEAP_SIZE;
	. = ALIGN(8);
	_eheap = .;

    }
}

