/*
 * drv_ws28xx.c
 *
 *  Created on: Nov 20, 2020
 *      Author: Admin
 */

#include <stdbool.h>
#include "main.h"
#include "tim.h"

#include "drv_ws28xx.h"


#define High_time 32
#define Low_time 16


TIM_HandleTypeDef * Timer;
uint32_t Channel;
DrvWS28xxSupportedTypeDef Type;
bool init = false;

uint32_t DrvWS28xxInit(TIM_HandleTypeDef *htim, uint32_t tim_channel, DrvWS28xxSupportedTypeDef ic)
{
	Timer = htim;
	Channel = tim_channel;
	if(!(ic == DRV_WS28XX_WS2811) && !(ic == DRV_WS28XX_WS2812))
	{
		Type = DRV_WS28XX_UNSUPPORTED;
		return DRV_WS28XX_ERROR;
	}

	else
	{
		Type = ic;
		init = true;
	}

	return DRV_WS28XX_OK;
}


uint32_t DrvWS28xxSetColorLeds(uint8_t* rgb, uint8_t nbr_of_leds)
{
	if(!init || nbr_of_leds > DRV_WS28XX_MAX_NUMBER_OF_LEDS)
		return DRV_WS28XX_ERROR;


	bool red[nbr_of_leds*8];
	bool green[nbr_of_leds*8];
	bool blue[nbr_of_leds*8];

	uint8_t buff[nbr_of_leds*24+1];
	buff[nbr_of_leds*24] = 0;

	uint8_t current_byte = 0;
	uint8_t current_bit = 0;

	for(uint8_t i = 0, k = 0; i < nbr_of_leds; i++)
	{
		current_byte = rgb[k];
		for(int o = 0; o < 8; o++)
		{
			current_bit = current_byte >> o;
			if(current_bit & 1)
				red[o+8*i] = true;
			else
				red[o+8*i] = false;
		}

		current_byte = rgb[k+1];
		for(int o = 0; o < 8; o++)
		{
			current_bit = current_byte >> o;
			if(current_bit & 1)
				green[o+8*i] = true;
			else
				green[o+8*i] = false;
		}

		current_byte = rgb[k+2];
		for(int o = 0; o < 8; o++)
		{
			current_bit = current_byte >> o;
			if(current_bit & 1)
				blue[o+8*i] = true;
			else
				blue[o+8*i] = false;
		}
		k+=3;
	}


	switch(Type)
	{
		case DRV_WS28XX_WS2811:
		{

			for(uint8_t i = 0; i < nbr_of_leds; i++)
			{
				for(int o = 0; o < 8; o++)
				{
					if (red[i*8+o])
						buff[i*24+o] = High_time;
					else
						buff[i*24+o] = Low_time;
				}

				for(int o = 0; o < 8; o++)
				{
					if (green[i*8+o])
						buff[i*24+o+8] = High_time;
					else
						buff[i*24+o+8] = Low_time;
				}

				for(int o = 0; o < 8; o++)
				{
					if (blue[i*8+o])
						buff[i*24+o+16] = High_time;
					else
						buff[i*24+o+16] = Low_time;
				}
			}
			break;
		}
		case DRV_WS28XX_WS2812:
		{
			for(uint8_t i = 0; i < nbr_of_leds; i++)
			{
				for(int o = 0; o < 8; o++)
				{
					if (green[i*8+o])
						buff[i*24+o] = High_time;
					else
						buff[i*24+o] = Low_time;
				}

				for(int o = 0; o < 8; o++)
				{
					if (red[i*8+o])
						buff[i*24+o+8] = High_time;
					else
						buff[i*24+o+8] = Low_time;
				}

				for(int o = 0; o < 8; o++)
				{
					if (blue[i*8+o])
						buff[i*24+o+16] = High_time;
					else
						buff[i*24+o+16] = Low_time;
				}
			}

			break;
		}

		case DRV_WS28XX_UNSUPPORTED:
			return DRV_WS28XX_ERROR;
	}

	HAL_TIM_PWM_Start_DMA(Timer, Channel, (uint32_t*)&buff[0], (nbr_of_leds*24+1));

	HAL_Delay(25);

	return DRV_WS28XX_OK;
}
