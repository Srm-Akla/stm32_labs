/*
 * onefive.c
 *
 *  Created on: Oct 22, 2020
 *      Author: Admin
 */

#include "onefive.h"
#include "main.h"

void moreBlink()
{

	HAL_Delay(100);
	HAL_GPIO_TogglePin(RUNNING_LED_GPIO_Port, RUNNING_LED_Pin);

	HAL_Delay(100);
	HAL_GPIO_TogglePin(RUNNING_LED_GPIO_Port, RUNNING_LED_Pin);

	HAL_Delay(100);
	HAL_GPIO_TogglePin(RUNNING_LED_GPIO_Port, RUNNING_LED_Pin);

};
