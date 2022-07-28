/*
 * LampControl.h
 *
 *  Created on: Nov 11, 2020
 *      Author: Admin
 */

#ifndef LAMPCONTROL_H_
#define LAMPCONTROL_H_

#include <stdbool.h>

#include "main.h"


// 29:00

typedef struct
{
	uint32_t id;
	GPIO_TypeDef* port;
	uint32_t pin;
	uint32_t delay;

} thread_config_t;

static thread_config_t thread_config[] =
{
		{1, EXT_LED1_GPIO_Port, EXT_LED1_Pin, 500},
		{2, EXT_LED2_GPIO_Port, EXT_LED2_Pin, 1000},
		{3, EXT_LED3_GPIO_Port, EXT_LED3_Pin, 1500},
		{4, EXT_LED4_GPIO_Port, EXT_LED4_Pin, 2000}
};

bool LampControlInit(void);
void LampControl(void *arg);

#endif /* LAMPCONTROL_H_ */
