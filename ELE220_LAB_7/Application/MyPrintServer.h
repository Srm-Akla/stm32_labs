/*
 * print_server.h
 *
 *  Created on: Oct 22, 2020
 *      Author: Admin
 */

#ifndef PRINT_SERVER_H_
#define PRINT_SERVER_H_

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "usart.h"
#include "main.h"
#include "stdbool.h"
#include "cmsis_os2.h"

#if 1
#define PRINTF(...) PrintServerPrintf(__VA_ARGS__)
#else
#define PRINTF(...) do{} while(0)
#endif

#define HALCallback HAL_UART_TxCpltCallback


osMessageQueueId_t Print_Queue;
osThreadId_t Print_Thread;
osMemoryPoolId_t Print_Pool;

bool PrintServerInit(void);
void PrintServer(void *arg);
void PrintServerPrintf(const char *fmt, ...);

#endif /* PRINT_SERVER_H_ */
