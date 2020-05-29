        .syntax unified
	.cpu cortex-m4
	.fpu softvfp
	.thumb


/* start address for the initialization values of the .data section. defined in linker script */
.word	_sidata
/* start address for the .data section. defined in linker script */
.word	_sdata
/* end address for the .data section. defined in linker script */
.word	_edata
/* start address for the .bss section. defined in linker script */
.word	_sbss
/* end address for the .bss section. defined in linker script */
.word	_ebss


        .section        .text.Reset_Handler
	.type	Reset_Handler, %function
        .global Reset_Handler
/* MyReset handler */
Reset_Handler:
/*        ldr   sp, =_estack     Atollic update: set stack pointer */
        ldr     r0, =SystemSelect
        blx     r0
        /* Copy the data segment initializers from flash to SRAM */
        movs	r1, #0
        b	LoopCopyDataInit
CopyDataInit:
	ldr	r3, =_sidata
	ldr	r3, [r3, r1]
	str	r3, [r0, r1]
	adds	r1, r1, #4
LoopCopyDataInit:
	ldr	r0, =_sdata
	ldr	r3, =_edata
	adds	r2, r0, r1
	cmp	r2, r3
	bcc	CopyDataInit
	ldr	r2, =_sbss
	b	LoopFillZerobss
        /* Zero fill the bss segment */
FillZerobss:
	movs	r3, #0
	str	r3, [r2], #4
LoopFillZerobss:
	ldr	r3, = _ebss
	cmp	r2, r3
	bcc	FillZerobss
        /* Call the clock system intitialization function */
/*        bl      SystemInit */
        /* Call static constructors */
        bl      __libc_init_array
        /* Call the application's entry point */
	bl	main
LoopForever:
        b       LoopForever


        .type	start_808, %function
        .global start_808
start_808:
        ldr     r1, =0x8080000
        ldr     r0, [r1, #0]
        mov     sp, r0
        ldr     r0, [r1, #4]
        push    {r0}
        pop     {pc}


