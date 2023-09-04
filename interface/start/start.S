.text
.global _start
_start:
	/*
	 * Vector table
	 */ 
	b reset
	b .
	b .
	b .
	b .
	b .
	b .
	b .

reset:
	/*
	 * Set vector address in CP15 VBAR register
	 */ 
	ldr	r0, =_start
	mcr	p15, 0, r0, c12, c0, 0	@Set VBAR

	/*
	 * Set the cpu to SVC32 mode, Disable FIQ/IRQ
	 */  
	mrs r0, cpsr
	bic r0, r0, #0x1f
	orr	r0, r0, #0xd3
	msr	cpsr ,r0

	/*
	 * Defines access permissions for each coprocessor
	 */  
    mov	r0, #0xfffffff
    mcr	p15, 0, r0, c1, c0, 2  	

	/*
	 * Invalidate L1 I/D                                                                                                                   
	 */
	mov	r0, #0					@Set up for MCR
	mcr	p15, 0, r0, c8, c7, 0	@Invalidate TLBs
	mcr	p15, 0, r0, c7, c5, 0	@Invalidate icache
	
	/*
	 * Set the FPEXC EN bit to enable the FPU
	 */ 
	mov r3, #0x40000000
	fmxr FPEXC, r3
	
	/*
	 * Disable MMU stuff and caches
	 */
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00002000		@Clear bits 13 (--V-)
	bic	r0, r0, #0x00000007		@Clear bits 2:0 (-CAM)
	orr	r0, r0, #0x00001000		@Set bit 12 (---I) Icache
	orr	r0, r0, #0x00000002		@Set bit 1 (--A-) Align
	orr	r0, r0, #0x00000800		@Set bit 11 (Z---) BTB
	mcr	p15, 0, r0, c1, c0, 0

	/*
	 * Initialize stacks                                                                                                                  
	 */
init_stack:     
	/*svc mode stack*/
	msr cpsr, #0xd3
	ldr sp, _stack_svc_end

	/*undef mode stack*/
	msr cpsr, #0xdb
	ldr sp, _stack_und_end

	/*abort mode stack*/	
	msr cpsr,#0xd7
	ldr sp,_stack_abt_end

	/*irq mode stack*/	
	msr cpsr,#0xd2
	ldr sp, _stack_irq_end
	
	/*fiq mode stack*/
	msr cpsr,#0xd1
	ldr sp, _stack_fiq_end
	
	/*user mode stack, enable FIQ/IRQ*/
	msr cpsr,#0x10
	ldr sp, _stack_usr_end

	/*Call main*/
	b main

_stack_svc_end:      
	.word stack_svc + 512
_stack_und_end:      
	.word stack_und + 512
_stack_abt_end:      
	.word stack_abt + 512
_stack_irq_end:      
    .word stack_irq + 512
_stack_fiq_end:
    .word stack_fiq + 512
_stack_usr_end:      
    .word stack_usr + 512

.data
stack_svc:      
	.space 512
stack_und:
	.space 512
stack_abt:      
	.space 512
stack_irq:      
	.space 512
stack_fiq:      
	.space 512
stack_usr:      
	.space 512
