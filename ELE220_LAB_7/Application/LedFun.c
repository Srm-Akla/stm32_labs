/*
 * LedFun.c
 *
 *  Created on: Nov 20, 2020
 *      Author: Admin
 */

#include "stdbool.h"
#include "tim.h"
#include "main.h"
#include "cmsis_os2.h"

#include "LedFun.h"
#include "drv_ws28xx.h"



bool LedFunInit(void)
{
	DrvWS28xxInit(&htim1, TIM_CHANNEL_4, DRV_WS28XX_WS2811);

	osThreadNew(LedFun, NULL, NULL);

	return(true);
}

void LedFun(void *arg)
{
	uint8_t color[3 * 5];

	color[3 * 5 - 1] = 0;

	while(1)
	{
		color[0] = 255;
		color[1] = 0;
		color[2] = 0;
		DrvWS28xxSetColorLeds(color,1);

		HAL_Delay(100);


		color[3] = 0;
		color[4] = 255;
		color[5] = 0;
		DrvWS28xxSetColorLeds(color,2);

		HAL_Delay(100);


		color[6] = 0;
		color[7] = 0;
		color[8] = 255;
		DrvWS28xxSetColorLeds(color,3);

		HAL_Delay(100);


		color[9] = 0;
		color[10] = 255;
		color[11] = 255;
		DrvWS28xxSetColorLeds(color,4);

		HAL_Delay(100);


		color[12] = 255;
		color[13] = 0;
		color[14] = 255;
		DrvWS28xxSetColorLeds(color,5);

		HAL_Delay(100);


		for(int i = 0; i < 3*5; i++)
		{
			color[i] = 0;
		}


		DrvWS28xxSetColorLeds(color,5);

		HAL_Delay(200);
	}

}
