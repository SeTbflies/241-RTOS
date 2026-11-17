
#define KERNEL_H


#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>


void osKernelInitialize(void);
bool osCreateThread(void (*fn)(void));
void osKernelStart(void);
void osYield(void);


