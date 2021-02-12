.section .text
.cpu cortex-m4
.syntax unified
.align  2
.thumb

.extern currentPt
.global  os_SchedulerLaunch

os_SchedulerLaunch:
    LDR     R0, =currentPt         
    LDR     R2, [R0]       @ R2 =currentPt       
    LDR     SP, [R2]       @SP = currentPt->stackPt    
    POP     {R4-R11}          
    POP     {R0-R3}            
    POP     {R12}
    ADD     SP,SP,#4           
    POP     {LR}               
    ADD     SP,SP,#4         
    CPSIE   I                 
    BX      LR     

.align
.end
