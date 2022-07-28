/*
 * twothreec.c
 *
 *  Created on: 30. okt. 2020
 *      Author: Admin
 */


#include <RedBlink.h>

void blinkRed()
{
	while(1)
	{
		HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);
		//HAL_Delay(100); // Replaced by line under, as HAL_Delay is an interrupt of a lower level as the Hardfault handler.
		for(volatile uint32_t i = 0; i < SystemCoreClock/40; i++){}
	}
}
