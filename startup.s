.syntax unified
.thumb

.global Default_Handler
.global Reset_Handler

.section .text.Reset_Handler
.type Reset_Handler, %function

Reset_Handler:
	ldr r0, =__StackTop
	mov sp, r0
	ldr r0, =_flash_edata   /* Load address of .data section in FLASH */
	ldr r1, =_sdata            /* RAM start address for .data section */
	ldr r2, =_edata           /* RAM end address for .data section */

copy_data:
	cmp   r1, r2               /* Check if we are done copying */
	bge   init_bss             /* If done, move to init_bss */
	ldr   r3, [r0]             /* Load word from Flash */
	add	  r0, r0, #4
	str   r3, [r1]            /* Store word in RAM */
	add   r1, r1, #4
	b     copy_data            /* Repeat copying */

init_bss:
	ldr   r0, =_sbss   /* Start of the .bss section */
	ldr   r1, =_ebss     /* End of the .bss section */
	movs  r2, #0               /* Zero value to fill */

zero_bss:
	cmp   r0, r1               /* Check if we are done zeroing */
	bge   main_c_call           /* If done, move to init_stack */
	str   r2, [r0]             /* Store zero in RAM */
    add   r0, r0, #4           /* Increment RAM address */
	b     zero_bss             /* Repeat zeroing */
main_c_call:
	b    main

.size Reset_Handler, .-Reset_Handler