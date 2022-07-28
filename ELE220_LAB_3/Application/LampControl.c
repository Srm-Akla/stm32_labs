/*
 * LampControl.c
 *
 *  Created on: Nov 11, 2020
 *      Author: Admin
 */

#include "LampControl.h"
#include "main.h"
#include "MyPrintServer.h"
#include "cmsis_os.h"

#include <stdbool.h>

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
	lamp1_thread_id = osThreadNew(LampControl, (void *)&thread_config[0], &lamp_control_thread_attributes);
	if(!lamp1_thread_id) return false;

	lamp_control_thread_attributes.name = "LampControlThread2";
	lamp2_thread_id = osThreadNew(LampControl, (void *)&thread_config[1], &lamp_control_thread_attributes);
	if(!lamp2_thread_id) return false;

	lamp_control_thread_attributes.name = "LampControlThread3";
	lamp3_thread_id = osThreadNew(LampControl, (void *)&thread_config[2], &lamp_control_thread_attributes);
	if(!lamp3_thread_id) return false;

	lamp_control_thread_attributes.name = "LampControlThread4";
	lamp4_thread_id = osThreadNew(LampControl, (void *)&thread_config[3], &lamp_control_thread_attributes);
	if(!lamp4_thread_id) return false;

	multiplex_id = osSemaphoreNew(MAX_LEDS, MAX_LEDS, NULL);


	return(true);

}

void LampControl(void *arg)
{

	thread_config_t *cfg = (thread_config_t *)arg;

		while(1)
		{
		osSemaphoreAcquire(multiplex_id, osWaitForever);
		HAL_GPIO_WritePin(cfg->port, cfg->pin, GPIO_PIN_RESET);

		PRINTF("Lamp %u turned ON\r\n", cfg->id);
		osDelay(cfg->delay);

		HAL_GPIO_WritePin(cfg->port, cfg->pin, GPIO_PIN_SET);
		osSemaphoreRelease(multiplex_id);

		PRINTF("Lamp %u turned OFF\r\n", cfg->id);
		osDelay(cfg->delay);

		}

}

