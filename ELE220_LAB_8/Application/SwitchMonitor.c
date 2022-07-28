/*
 * SwitchMonitor.c
 *
 *  Created on: Nov 18, 2020
 *      Author: Admin
 */

#include "stm32l4xx_it.h"
#include "gpio.h"
#include "cmsis_os2.h"

#include "MyPrintServer.h"
#include "SwitchMonitor.h"



void SwitchMonitorInit(void)
{

	/* Lager  meldingskø (osMessageQueueNew) */
	Queue = osMessageQueueNew(0x08, 0x20, NULL);

	/* Lager tråd (osThreadNew) */
	New_Thread = osThreadNew(SwitchMonitor, NULL, NULL);

	/* Lager hendelsesflagg objekt (osEventFlagsNew) */
	Switch_Flag = osEventFlagsNew(NULL);

}

void SwitchMonitor(void *arg)
{
	while(1)
	{
		uint32_t Pin;
		osStatus_t status = osMessageQueueGet(Queue, &Pin, NULL, osWaitForever);

		if(status != osOK)
			continue;

		if((Pin & SWITCH_STATE_MASK) == SWITCH_RELEASED)
		{
			PRINTF("Button %s is free \r\n", gpio_port_pin[Pin].desc);
			continue;
		}

		else
		{
			PRINTF("SWITCH %s is pressed \r\n", gpio_port_pin[Pin].desc);
			/*
			PRINTF("Button %i is pushed \n", Pin);
			switch(Pin)
			{
				case EVENT_FLAG_LEFT_JOY_PRESSED:
				{
					osEventFlagsSet(Switch_Flag, EVENT_FLAG_LEFT_JOY_PRESSED);
					break;
				}

				case EVENT_FLAG_RIGHT_JOY_PRESSED:
				{
					osEventFlagsSet(Switch_Flag, EVENT_FLAG_RIGHT_JOY_PRESSED);
					break;
				}

				case EVENT_FLAG_UP_JOY_PRESSED:
				{
					osEventFlagsSet(Switch_Flag, EVENT_FLAG_UP_JOY_PRESSED);
					break;
				}

				case EVENT_FLAG_DOWN_JOY_PRESSED:
				{
					osEventFlagsSet(Switch_Flag, EVENT_FLAG_DOWN_JOY_PRESSED);
					break;
				}
			}*/
		}
		osEventFlagsSet(Switch_Flag, gpio_port_pin[Pin].flag);
	}

}

void SwitchMonitorInterruptCB(uint16_t Pin)
{
	uint32_t msg;

	for(uint8_t idx = 0; idx < (sizeof(gpio_port_pin) / sizeof(gpio_port_pin_t)); idx++)
	{
		if (Pin & gpio_port_pin[idx].pin)
		{
			GPIO_PinState pin_state = HAL_GPIO_ReadPin(gpio_port_pin[idx].port, gpio_port_pin[idx].pin);

			if (pin_state == GPIO_PIN_SET)
			{
				msg = SWITCH_PRESSED | idx;
				osMessageQueuePut(Queue, &msg, 0, 0);
			}
			else
			{
				msg = SWITCH_RELEASED | idx;
				osMessageQueuePut(Queue, &msg, 0, 0);
			}
			return;
		}
	}
}






