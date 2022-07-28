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
#include "qbitClear.h"
#include "twothreec.h"



void blinky()
{

HAL_Delay(500);

HAL_GPIO_TogglePin(RUNNING_LED_GPIO_Port, RUNNING_LED_Pin);

/* Mater variabel uten fortegn */

int8_t vara = __USAT(126, 8);
/* Mater variabel med fortegn */


int8_t varb = __SSAT(+126, 8);


HAL_Delay(500);


PRINTF("Using USAT gives us: %i \n", vara);
PRINTF("Using SSAT gives us: %i \n", varb);

PRINTF("Current Q-bit value is: %i \n", __get_APSR());

if(__get_APSR() & (1<<27))
{
	MY_CLEARING_Q_FLAG_MACRO;

	HAL_Delay(100);
	PRINTF("Current Q-bit value is: %i \n", __get_APSR());
}

/* Get into the hardfault-handler. */
HAL_TIM_Base_Stop(24241212);
}

/* Chose option 1. Added function to stm32l4xx_it.c */
void MyHardFault_Handler(void)
{
	PRINTF("You're now in the hardfault_handler. \n");
	while(1)
	{

	HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);
	//HAL_Delay(100); // Replaced by line under, as HAL_Delay is an interrupt of a lower level as the Hardfault handler.
	for(volatile uint32_t i = 0; i < SystemCoreClock/40; i++){}

	}
}
