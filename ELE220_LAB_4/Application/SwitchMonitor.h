/*
 * SwitchMonitor.h
 *
 *  Created on: Nov 18, 2020
 *      Author: Admin
 */

#ifndef SWITCHMONITOR_H_
#define SWITCHMONITOR_H_

#include "cmsis_os2.h"
#include "main.h"

#define SWITCH_PRESSED		0xDEAD0000
#define SWITCH_RELEASED		0xBEEF0000
#define SWITCH_STATE_MASK	0xFFFF0000

#define EVENT_FLAG_LEFT_JOY_PRESSED 0x02
#define EVENT_FLAG_RIGHT_JOY_PRESSED 0x04
#define EVENT_FLAG_UP_JOY_PRESSED 0x08
#define EVENT_FLAG_DOWN_JOY_PRESSED 0x20

#define SwitchMonitorInterruptCB HAL_GPIO_EXTI_Callback

typedef struct {
	GPIO_TypeDef* port;
	uint16_t pin;
	uint32_t flag;
	char* desc;
} gpio_port_pin_t;

static const gpio_port_pin_t gpio_port_pin[] = {
		{JOY_UP_GPIO_Port, JOY_UP_Pin, EVENT_FLAG_UP_JOY_PRESSED, "JOY-UP"},
		{JOY_LEFT_GPIO_Port, JOY_UP_Pin, EVENT_FLAG_LEFT_JOY_PRESSED, "JOY-LEFT"},
		{JOY_RIGHT_GPIO_Port, JOY_RIGHT_Pin, EVENT_FLAG_RIGHT_JOY_PRESSED, "JOY-RIGHT"},
		{JOY_DOWN_GPIO_Port, JOY_DOWN_Pin, EVENT_FLAG_DOWN_JOY_PRESSED, "JOY-DOWN"}
};


osMessageQueueId_t Queue;
osEventFlagsId_t Switch_Flag;
osThreadId_t New_Thread;

void SwitchMonitorInit(void);
void SwitchMonitor(void *arg);


#endif /* SWITCHMONITOR_H_ */
