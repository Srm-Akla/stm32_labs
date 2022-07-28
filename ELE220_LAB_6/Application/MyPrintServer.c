/*
 * print_server.c
 *
 *  Created on: Oct 22, 2020
 *      Author: Admin
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "usart.h"
#include "main.h"
#include "stdbool.h"
#include "cmsis_os2.h"

#include "MyPrintServer.h"

#define PRINT_BUFFER_SIZE 64

typedef struct
{
	uint8_t loc[PRINT_BUFFER_SIZE];
} Print_Buffer_t;

UART_HandleTypeDef *uart_handle_p = &huart2;

bool PrintServerInit(void)
{
	/* Create memorypool */
	Print_Pool = osMemoryPoolNew(32, sizeof(Print_Buffer_t), NULL);

	/* Create print queue */
	Print_Queue = osMessageQueueNew(5, 4, NULL);

	/* Create a printserver thread */
	Print_Thread = osThreadNew(PrintServer, NULL, NULL);

	return(true);
}

void PrintServer(void *arg)
{
	Print_Buffer_t *pMsg;
	osStatus_t status;

	while(1)
	{
		status = osMessageQueueGet(Print_Queue, (Print_Buffer_t *)&pMsg, NULL, osWaitForever);
		if (status == osOK)
		{
			if(	HAL_UART_Transmit_DMA(uart_handle_p,
				(uint8_t *)&pMsg->loc,
				strlen((char *)pMsg)) != HAL_OK);
			{
				osMemoryPoolFree(Print_Pool, pMsg);
				continue;
			}
		}
		osThreadFlagsWait(0x1U, osFlagsWaitAny, osWaitForever);
		osMemoryPoolFree(Print_Pool, pMsg);

	}
}

void PrintServerPrintf(const char *fmt, ...){

	Print_Buffer_t *buff = (Print_Buffer_t *)osMemoryPoolAlloc(Print_Pool, 0);

	if(buff == NULL)
		return;

	va_list args;
	va_start(args, fmt);

	vsnprintf((char *)buff->loc, PRINT_BUFFER_SIZE, fmt, args);
	va_end(args);

	osStatus_t status = osMessageQueuePut(Print_Queue, &buff, 0U, 0U);
	//HAL_UART_Transmit(uart_handle_p, (uint8_t *)buff, strlen((char *)buff), 10);

	if (status != osOK)
		osMemoryPoolFree(Print_Pool, buff);

}

void HALCallback(UART_HandleTypeDef *huart)
{
	if (huart == uart_handle_p)
		osThreadFlagsSet(Print_Thread, 0x1U);
}
