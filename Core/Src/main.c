/* USER CODE BEGIN Header */
extern void runFirstThread(void);
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h> //You are permitted to use this library, but currently only printf is implemented. Anything else is up to you!
#include <stdint.h>
#include "kernel.h"

/**
  * @brief  The application entry point.
  * @retval int
  */
//uint32_t* stackptr;
#define RUN_FIRST_THREAD 3
#define YIELD 4

void print_continuously(void){
while (1)
  {

	  printf("Thread 1!\r\n");
	  osYield();

  }
}

void print_again()
{
while(1)
	{
	   printf("Thread 2!\r\n");
       osYield();
    }
}
void jumpAssembly(void* fcn)
{
__asm("MOV PC, R0");
}

void run_first_thread(void)
{
  __asm("SVC #3");
}
void print_success(void)
{
__asm("SVC #17");
}
void print_fail(void)
{
__asm("SVC #18");

}

int main(void)
{

  /* MCU Configuration: Don't change this or the whole chip won't work!*/
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* MCU Configuration is now complete. Start writing your code below this line */
  uint32_t* MSP_INIT_VAL = *(uint32_t**)0x0;
  printf("MSP Init is: %p\r\n",MSP_INIT_VAL); //note the %p to print a pointer. It will be in hex
  //uint32_t PSP_val = (uint32_t)MSP_INIT_VAL - 0x400;
  //__set_PSP(PSP_val);
  //__set_CONTROL(0x2);   //hex value for 010

  //stackptr = (uint32_t*)((uint32_t)MSP_INIT_VAL - 0x800);  //stackptr must start here
  //*(--stackptr) = 1<<24; //A magic number, this is xPSR
  //*(--stackptr) = (uint32_t)print_continuously; //the function name
  //for (int i = 0; i < 14; i++) *(--stackptr) = 0xA;

  osKernelInitialize();
  osCreateThread(print_continuously);      //Run the thread
  osCreateThread(print_again);      //Run the thread


  //print_success();
  //print_fail();
 // run_first_thread();
  /* USER CODE BEGIN WHILE */

//  while (1)
  //{
    /* USER CODE END WHILE */
//	  printf("Hello, world!\r\n");
    /* USER CODE BEGIN 3 */
  //}
  /* USER CODE END 3 */

 // print_continuously();
  //jumpAssembly((void*)print_continuously);

  osKernelStart();
}

