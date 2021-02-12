/**************************************************************************//**
 * @file     startup_<Device>.S
 * @brief    CMSIS Cortex-M# Core Device Startup File for
 *           Device <Device>
 * @version  V2.0
 * @date     16. August 2013
 *
 * @note     GCC for ARM Embedded Processors
 *
 ******************************************************************************/
/* Copyright (c) 2013 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/


	.syntax	unified
	.arch	armv6-m
	.cpu cortex-m4

	.section .isr_vector
	.align 2
	.globl	__isr_vector
__isr_vector:
	.long	__stack_end__            /* Top of Stack */
	.long	Reset_Handler         /* Reset Handler */
	.long	NMI_Handler           /* NMI Handler */
	.long	HardFault_Handler     /* Hard Fault Handler */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	SVC_Handler           /* SVCall Handler */
	.long	0                     /* Reserved */
	.long	0                     /* Reserved */
	.long	PendSV_Handler        /* PendSV Handler */
	.long	SysTick_Handler       /* SysTick Handler */

	.size	__isr_vector, . - __isr_vector


/* Reset Handler */	
	.thumb_func
	.align	1
	.globl	Reset_Handler
	.type	Reset_Handler, %function
Reset_Handler:
	.extern SystemInit
	.extern main
	CPSID	I
	ldr	R0, =SystemInit
	blx	R0
	CPSIE	I
	ldr R0, =main
	blx	R0

	.pool
	.size	Reset_Handler, . - Reset_Handler

	.text
	.thumb
	.thumb_func
	.align 2
	.global SysTick_Handler
	.type 	SysTick_Handler, %function
SysTick_Handler:
	.extern currentPt
    CPSID   I                  
    PUSH    {R4-R11}        @save r4,r5,r6,r7,r8,r9,r10,r11   
    LDR     R0, =currentPt  @ r0 points to currentPt       
    LDR     R1, [R0]        @ r1= currentPt   
    STR     SP, [R1]           
    LDR     R1, [R1,#4]     @ r1 =currentPt->next   
    STR     R1, [R0]        @currentPt =r1   
    LDR     SP, [R1]        @SP= currentPt->stackPt   
    POP     {R4-R11}           
    CPSIE   I                  
    BX      LR 

	.align	1
	.thumb_func
	.weak	Default_Handler
	.type	Default_Handler, %function
Default_Handler:
	b	.
	.size	Default_Handler, . - Default_Handler

/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
	.macro	def_irq_handler	handler_name
	.weak	\handler_name
	.set	\handler_name, Default_Handler
	.endm

	def_irq_handler	NMI_Handler
	def_irq_handler	HardFault_Handler
	def_irq_handler	SVC_Handler
	def_irq_handler	PendSV_Handler


/* IRQ Handlers */

/* ToDo(allenm):  Add here the export definition for the device specific external interrupts handler */
/* ToDo(allenm):  Add here the names for the device specific external interrupts handler             */

	@def_irq_handler	<DeviceInterrupt>_IRQHandler

	.end
