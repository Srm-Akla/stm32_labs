
#include <MultiBlink.h>
#include <MyApp.h>
#include <MyPrintServer.h>
#include <RedBlink.h>
#include "adc.h"
#include "cmsis_os2.h"

#include "tim.h"
#include "MyAdc.h"
#include "main.h"
#include "qbitClear.h"
#include "LampControl.h"
#include "LedFun.h"

#define LEDS_ALLOWED 3

void blinky()
{
	PrintServerInit();
	PRINTF("Blinky has been called. \n\r");

	LampControlInit();
	SwitchMonitorInit();
	my_ADC_init(&hadc2, &hadc1,&htim6);
	LedFunInit();

}

/* Chose option 1. Added function to stm32l4xx_it.c */
void MyHardFault_Handler(void)
{
	PRINTF("You're now in the hardfault_handler.\n");
	while(1)
	{
		HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);
		//HAL_Delay(100); // Replaced by line under, as HAL_Delay is an interrupt of a lower level as the Hardfault handler.
		for(volatile uint32_t i = 0; i < SystemCoreClock/40; i++){}
	}
}
