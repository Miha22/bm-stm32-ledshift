MEMORY
{
	FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 32K
	SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 10K
}

ENTRY(Reset_Handler)

SECTIONS
{    
	.isr_vector : {
	   . = ALIGN(4);
	   KEEP(*(.isr_vector))
	   . = ALIGN(4);
	} >FLASH

	. = ALIGN(4);
	.text : {
		*(.text)
		*(.text*)
		*(.glue_7)
		*(.glue_7t)
		*(.eh_frame)
		KEEP (*(.init))
		KEEP (*(.fini))
		. = ALIGN(4);
		_etext = .;
	} >FLASH
	.preinit_array : {
		. = ALIGN(4);
		__preinit_array_start = .;
		KEEP (*(.preinit_array))
		__preinit_array_end = .;
	} >FLASH
		
	.rodata : {
	        . = ALIGN(4);
	        *(.rodata)
	        *(.rodata*)
	        . = ALIGN(4);
	} >FLASH
		
    .ARM.extab : {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } >FLASH

    .ARM : {
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    } >FLASH

    .preinit_array : {
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array*))
        PROVIDE_HIDDEN (__preinit_array_end = .);
    } >FLASH

    .init_array : {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);
    } >FLASH

    .fini_array : {
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT(.fini_array.*)))
        KEEP (*(.fini_array*))
        PROVIDE_HIDDEN (__fini_array_end = .);
    } >FLASH

    . = ALIGN(4);

	.data : {
        _sdata = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;
	} >SRAM AT>FLASH
	_flash_edata = LOADADDR(.data);

	. = ALIGN(4);
	.bss : {
		_sbss = .;
	    *(.bss)
	    *(.bss*)
	    *(COMMON)
	    . = ALIGN(4);
	    _ebss = .;
	} >SRAM
	_end = NEXT(4);
		
    .ARM.attributes 0 : {
        *(.ARM.attributes)
    }
}
PROVIDE(__StackTop = ORIGIN(SRAM) + LENGTH(SRAM));