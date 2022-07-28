/*
 * print_server.c
 *
 *  Created on: Oct 22, 2020
 *      Author: Admin
 */
#include<stdarg.h>
#include<stdio.h>
#include<string.h>

#include "usart.h"
#include "onefour.h"


#define PRINT_BUFFER_SIZE 64

UART_HandleTypeDef*uart_handle_p = &huart2;


void PrintServerPrintf(const char *fmt, ...){


	uint8_t buff[PRINT_BUFFER_SIZE];

	va_list args;

	va_start(args, fmt);

	vsnprintf((char*)buff, PRINT_BUFFER_SIZE, fmt, args);

	va_end(args);

	HAL_UART_Transmit(uart_handle_p, (uint8_t*)buff, strlen((char*)buff), 10);

}
