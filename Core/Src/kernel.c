#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include "main.h"
#include "kernel.h"



typedef struct k_thread{
uint32_t* sp; //stack pointer
void (*thread_function)(void); //function pointer
}thread_t;

#define MaxThreadCount 15        //lab 4 start, set 15 stacks as the number of threads in the stack

thread_t threads[MaxThreadCount];   //array
uint32_t thread_count;
uint32_t current_thread;

extern uint32_t _Min_Stack_Size;  //The .id file stack size

extern void runFirstThread(void);  // defined in the assembly file


#define RUN_FIRST_THREAD 3
#define YIELD 4
#define stack_size 0x400
#define pool_size ((uint32_t)&_Min_Stack_Size)  //Read from the id file

uint32_t last_stack_top;
uint32_t pool_used;


void stack_pool_init(void)
{
  last_stack_top = *(uint32_t*)0x0;   // initial MSP value
  pool_used = 0;
}

uint32_t* allocate_stack(void)
{
  if (pool_used + stack_size > pool_size)
  {
    return NULL;
  }

  last_stack_top -= stack_size;
  pool_used += stack_size;
  return (uint32_t*)last_stack_top;
}

bool osCreateThread(void (*fn)(void))
{
	 if (thread_count >= MaxThreadCount)
	  {
	    return false;                      // stack is full
	  }
  uint32_t* sp = allocate_stack();
  if (sp == NULL)
  {
    return false;                      // No thread created
  }

  *(--sp) = 1 << 24;			//xpsr
  *(--sp) = (uint32_t)fn;
  for (int i = 0; i < 14; i++)
  {
    *(--sp) = 0xA;                     // LR, R12, R3-R0, R11-R4
  }


  threads[thread_count].sp = sp;                      // record the thread in the kernel
  threads[thread_count].thread_function = fn;
  thread_count++;
  return true;
}

void osKernelInitialize(void)
{
  last_stack_top = *(uint32_t*)0x0;   // initial MSP value
  pool_used = 0;
  thread_count = 0;
  current_thread = 0;
}

void osKernelStart(void)
{
	if (thread_count == 0){
		return;
	}
  __asm("SVC #3");
}
void osSched(){
	threads[current_thread].sp = (uint32_t*)(__get_PSP() - 8*4);
	current_thread = (current_thread+1)%thread_count;
	__set_PSP((uint32_t)threads[current_thread].sp);

}

void osYield(void){   //New syscall wrapper
	 __asm("SVC #4");
}

void SVC_Handler_Main( unsigned int *svc_args )
{
unsigned int svc_number;
/*
* Stack contains:
* r0, r1, r2, r3, r12, r14, the return address and xPSR
* First argument (r0) is svc_args[0]
*/
svc_number = ( ( char * )svc_args[ 6 ] )[ -2 ] ;
switch( svc_number )
{
case 17: //Arbitrarily chosen
printf("Success!\r\n");
break;
case 18:
printf("Fail!\r\n");
break;
case RUN_FIRST_THREAD:
	__set_PSP((uint32_t)threads[current_thread].sp);    //Swap the struct for the new array(multithreading)
runFirstThread();
break;
default: /* unknown SVC */
break;
case YIELD:
//Pend an interrupt to do the context switch
SCB->ICSR |= 1<<28;
__asm("isb");
break;
}
}

