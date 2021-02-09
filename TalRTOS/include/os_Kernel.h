#ifndef _OS_KERNEL_H
#define _OS_KERNEL_H

#include <stdint.h>

void os_KernelLaunch(uint32_t quanta);
void os_KernelInit(void);
uint8_t os_KernelAddThreads(
    void(*task0)(void), 
    void(*task1)(void), 
    void(*task3)(void));

#endif