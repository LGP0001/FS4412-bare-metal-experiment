.text
.global _start
_start:
	/*
	 * Vector table @ 异常终端表
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
	 * Set vector address in CP15 VBAR register @ 在CP15 VBAR寄存器中设置向量地址
	 */ 
	ldr	r0, =_start                             @ 将向量表地址加载到r0寄存器中
	mcr	p15, 0, r0, c12, c0, 0	@Set VBAR       @ 使用MCR指令将此地址设置为CP15 VBAR（Vector Base Address Register）的内容

	/*
	 * Set the cpu to SVC32 mode, Disable FIQ/IRQ @ 将CPU设置为SVC32模式，并禁用FIQ（快速中断请求）和IRQ（中断请求）
	 */  
	mrs r0, cpsr
	bic r0, r0, #0x1f
	orr	r0, r0, #0xd3
	msr	cpsr ,r0

	/*
	 * Defines access permissions for each coprocessor @ 定义每个协处理器的访问权限
	 */  
    mov	r0, #0xfffffff
    mcr	p15, 0, r0, c1, c0, 2  	

	/*
	 * Invalidate L1 I/D                               @ 无效L1指令/数据缓存和TLB                                                                                                                
	 */
	mov	r0, #0					@Set up for MCR
	mcr	p15, 0, r0, c8, c7, 0	@Invalidate TLBs 使TLBs（Translation Lookaside Buffers）无效
	mcr	p15, 0, r0, c7, c5, 0	@Invalidate icache 使指令缓存无效
	
	/*
	 * Set the FPEXC EN bit to enable the FPU @ 启用浮点单元（FPU）
	 */ 
	mov r3, #0x40000000
	fmxr FPEXC, r3              @ 设置FPEXC的EN位以启用浮点单元
	
	/*
	 * Disable MMU stuff and caches  @ 禁用MMU和缓存，修改c1控制寄存器的内容，以禁用MMU、缓存等，并启用某些功能，如对齐检查
	 */
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00002000		@Clear bits 13 (--V-)
	bic	r0, r0, #0x00000007		@Clear bits 2:0 (-CAM)
	orr	r0, r0, #0x00001000		@Set bit 12 (---I) Icache
	orr	r0, r0, #0x00000002		@Set bit 1 (--A-) Align
	orr	r0, r0, #0x00000800		@Set bit 11 (Z---) BTB
	mcr	p15, 0, r0, c1, c0, 0

	/*
	 * Initialize stacks        @ 初始化堆栈， 对于ARM的每种模式（SVC、UNDEF、ABORT、IRQ、FIQ、USER），都分配并初始化一个堆栈                                                                                                          
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

	/*Call main @ 跳转到主函数*/
	b main

    /*堆栈存储和定义*/
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
