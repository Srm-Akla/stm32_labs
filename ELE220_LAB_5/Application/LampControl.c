/*
 * LampControl.c
 *
 *  Created on: Nov 11, 2020
 *      Author: Admin
 */

#include <stdbool.h>

#include "main.h"
#include "gpio.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"

#include "LampControl.h"
#include "MyPrintServer.h"
#include "SwitchMonitor.h"

#define MAX_LEDS 2

osSemaphoreId_t multiplex_id;

osThreadAttr_t Lamp_attributes =
{
		.name = "lamp",
		.priority = (osPriority_t) osPriorityNormal,
		.stack_size = 256 * 4
};

bool LampControlInit(void)
{

	osThreadAttr_t lamp_control_thread_attributes = {.priority = osPriorityNormal};
	osThreadId_t lamp1_thread_id = NULL;
	osThreadId_t lamp2_thread_id = NULL;
	osThreadId_t lamp3_thread_id = NULL;
	osThreadId_t lamp4_thread_id = NULL;

	lamp_control_thread_attributes.name = "LampControlThread1";
	lamp1_thread_id = osThreadNew(LampControl, (void *)&thread_config[0], NULL);
	if(!lamp1_thread_id) return false;

	lamp_control_thread_attributes.name = "LampControlThread2";
	lamp2_thread_id =	osThreadNew(LampControl, (void *)&thread_config[1], NULL);
	if(!lamp2_thread_id) return false;

	lamp_control_thread_attributes.name = "LampControlThread3";
	lamp3_thread_id =	osThreadNew(LampControl, (void *)&thread_config[2], NULL);
	if(!lamp3_thread_id) return false;

	lamp_control_thread_attributes.name = "LampControlThread4";
	lamp4_thread_id = osThreadNew(LampControl, (void *)&thread_config[3], NULL); // &lamp_control_thread_attributes
	if(!lamp4_thread_id) return false;

	multiplex_id = osSemaphoreNew(MAX_LEDS, MAX_LEDS, NULL);

	return(true);

}

void LampControl(void *arg)
{

	thread_config_t *cfg = (thread_config_t *)arg;
	uint8_t blinking;

	HAL_GPIO_WritePin(cfg->port, cfg->pin, GPIO_PIN_SET);

	while(1)
	{

		uint32_t Flag = osEventFlagsWait(Switch_Flag, cfg->flag, osFlagsWaitAny, osWaitForever);

		if(!(Flag & osFlagsError))
			blinking ^= 0x01;

		if(!blinking)
		{
			HAL_GPIO_WritePin(cfg->port, cfg->pin, GPIO_PIN_SET);
			continue;
		}

		osSemaphoreAcquire(multiplex_id, osWaitForever);
		HAL_GPIO_WritePin(cfg->port, cfg->pin, GPIO_PIN_RESET);

		PRINTF("Lamp %u turned ON\r\n", cfg->id);
		osDelay(cfg->delay);

		HAL_GPIO_WritePin(cfg->port, cfg->pin, GPIO_PIN_SET);
		osSemaphoreRelease(multiplex_id);

		PRINTF("Lamp %u turned OFF\r\n", cfg->id);
		osDelay(cfg->delay);

		if(Flag == osEventFlagsWait(Switch_Flag, cfg->flag, osFlagsWaitAny, 0))
			break;
	}



}

