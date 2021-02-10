#include "os_Kernel.h"
#include "TM4C123GH6PM.h"

#define SYSPRI3 (*((volatile uint32_t *)0xE000Ed20))

#define NUM_OF_THREADS  3
#define STACKSIZE       100

#define BUS_FREQ        16000000

void os_SchedulerLaunch(void);

uint32_t MILLIS_PRESCALER;

struct tcb {
    int32_t *stackPtr;
    struct tcb *nextPtr;
};

typedef struct  tcb tcbType;
tcbType tcbs[NUM_OF_THREADS];
tcbType *currentPtr;

int32_t TCB_STACK[NUM_OF_THREADS][STACKSIZE];

void os_KernelStackInit(int i)
{
    tcbs[i].stackPtr = &TCB_STACK[i][STACKSIZE-16];
    TCB_STACK[i][STACKSIZE-1] = 0x01000000;
}

uint8_t os_KernelAddThreads(
                            void(*task0)(void), 
                            void(*task1)(void), 
                            void(*task2)(void))
{
    //__disable_irq(); //TODO(allen): This needs to be implemented to disable interupts
    tcbs[0].nextPtr = &tcbs[1];
    tcbs[1].nextPtr = &tcbs[2];
    tcbs[2].nextPtr = &tcbs[0];

    os_KernelStackInit(0);
    TCB_STACK[0][STACKSIZE - 2] = (int32_t)(task0);
    
    os_KernelStackInit(1);
    TCB_STACK[1][STACKSIZE - 2] = (int32_t)(task1);
    
    os_KernelStackInit(2);
    TCB_STACK[2][STACKSIZE - 2] = (int32_t)(task2);

    currentPtr = &tcbs[0];

    //__enable_irq(); //TODO(allen): This needs to be implemented to enable interupts

    return 1;
}


void os_KernelInit(void)
{
    __disable_irq(); //TODO(allen): This needs to be implemented to disable interupts

    MILLIS_PRESCALER = BUS_FREQ / 1000;
    
    __enable_irq(); //TODO(allen): This needs to be implemented to enable interupts
}


void os_KernelLaunch(uint32_t quanta)
{
    SysTick->CTRL = 0;
    SysTick->VAL = 0;
    SYSPRI3 = (SYSPRI3&0x00FFFFFF) | 0xE0000000;
    SysTick->LOAD = (quanta*MILLIS_PRESCALER)-1;
    SysTick->CTRL=0x00000007;
    os_SchedulerLaunch();
}