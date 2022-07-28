/*
 * twothreec.c
 *
 *  Created on: 30. okt. 2020
 *      Author: Admin
 */


#include <RedBlink.h>

void blinkRed(){

	while(1)
	{

	//HAL_Delay(100);
	HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);

	}
}
