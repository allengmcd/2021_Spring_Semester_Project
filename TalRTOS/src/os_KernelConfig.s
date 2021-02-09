.section .text
.cpu cortex-m4
.syntax unified
.thumb

@.global currentPtr
.global  SysTick_Handler
.global  os_SchedulerLaunch
.global  SysTick_Handler

SysTick_Handler:        @ Saves r0,r1,r2,r12,lr,pc,psr
        CPSID   I   
        PUSH    {r4,r5,r6,r7,r8,r10,r11} @ Save r4,r5,r6,r7,r8,r10,r11
        @LDR     R0,=currentPtr  @ r1 = CurrentPtr
        LDR     R1,[R0]
        STR     SP,[R1,#4] @ r1 = currentPtr->next
        LDR     R1,[R0]    @ currentPtr = r1
        LDR     SP,[R1]     @SP= currentPtr->stackPtr
        POP     {R4-R11}
        CPSIE   I
        BX      LR
        
os_SchedulerLaunch:
 @       LDR     R0,=currentPtr
        LDR     R2,[R0] @ r2 = currentPtr
        LDR     SP,[R2] @ SP =currentPtr->stackPtr
        POP     {R4-R11}
        POP     {R0-R3}
        POP     {R12}
        ADD     SP,SP,#4
        POP     {LR}
        ADD     SP,SP,#4
        CPSIE   I
        BX      LR
