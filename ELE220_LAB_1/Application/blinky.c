/*
 * blinky.c
 *
 *  Created on: Oct 22, 2020
 *      Author: Admin
 */

#include "main.h"
#include "blinky.h"
#include "onefour.h"
#include "onefive.h"


void blinky()
{

	uint32_t numbers = 48879;
	uint32_t srebmun = __REV(numbers);

	PRINTF("numbers: %i\n", numbers);
	PRINTF("%i :srebmun\n"), srebmun;

	while(1)
	{
		HAL_Delay(500);
		HAL_GPIO_TogglePin(RUNNING_LED_GPIO_Port, RUNNING_LED_Pin);
	}
}

/* Chose option 1. Added function to stm32l4xx_it.c */

