.global main
.thumb_func

main:
	/* enable the GPIOC clock in RCC_APB2ENR register */
	ldr	r0, =(0x40021000 + 0x18)
	ldr	r1, =(1 << 4)
	str	r1, [r0]

	/* set PC13 as output in GPIOC_CRH */
	ldr	r0, =(0x40011000 + 0x04)
	ldr	r1, =(1 << 20)
	str	r1, [r0]

loop:
	/* set PC13 in GPIOC_BSRR register */
	ldr	r0, =(0x40011000 + 0x10)
	ldr	r1, =(1 << 13)
	str	r1, [r0]

	/* loop as a delay */
	ldr	r0, =0
	ldr	r1, =200000
delay1:
	add	r0, r0, #1
	cmp	r0, r1
	bne	delay1

	/* reset PC13 in GPIOC_BRR reister */
	ldr	r0, =(0x40011000 + 0x14)
	ldr	r1, =(1 << 13)
	str	r1, [r0]

	/* loop as delay */
	ldr	r0, =0
	ldr	r1, =200000
delay2:
	add	r0, r0, #1
	cmp	r0, r1
	bne	delay2

	b	loop