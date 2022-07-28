/*
 * MyAdc.c
 *
 *  Created on: Nov 12, 2021
 *      Author: Admin
 */

#include "MyAdc.h"
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "stm32l4xx.h"
#include "MyPrintServer.h"
#include <stdio.h>
#include "cmsis_os2.h"

#define vref 3000
#define adc_res 4095
#define error 0x04U
#define full 0x02U
#define half 0x01U

volatile uint16_t adc_buffer[50];

const uint16_t v_ref;

ADC_ChannelConfTypeDef sConfig;
uint32_t aResultDMA;
osThreadId_t adc_thread_id;

void adc_thread(){
    int flag = osThreadFlagsWait((error|half|full), osFlagsWaitAny, osWaitForever);

    if(flag == error){
        HAL_ADC_Stop_DMA(&hadc2);
    }else if (flag == half){
        short val = 0;
        for(int i = 0; i < 25; i++){
            val += adc_buffer[i];
        }
    }else if (flag == full){
        short val = 0;
        for(int i = 25; i < 50; i++){
            val += adc_buffer[i];
        }

        val /= 50;
        val = 0;
    }
    
}

int my_adc_start(ADC_HandleTypeDef *AdcHandle){
    if (HAL_ADCEx_Calibration_Start(AdcHandle, ADC_SINGLE_ENDED) !=  HAL_OK){
        Error_Handler();
    }
    if (HAL_ADC_Start(AdcHandle) != HAL_OK){
        Error_Handler();
    }
    // Poll ADC1 Perihperal & TimeOut = 1mSec
    if (HAL_ADC_PollForConversion(AdcHandle, 1000) !=  HAL_OK){
        Error_Handler();
    }
    
       // Read The ADC Conversion Result & Map It To PWM DutyCycle
    uint32_t adc_val = HAL_ADC_GetValue(AdcHandle);
    uint32_t vref_cal = *((uint16_t*)((uint32_t)VREFINT_CAL_ADDR));
    uint32_t vdd;
    vdd = 3000 * vref_cal / adc_val;
    PRINTF("Vref %i", vdd);
    
    if (HAL_ADC_Stop(AdcHandle) != HAL_OK){
        Error_Handler();
    }
    
    return vdd;
}

int my_dma_start(){
	if (HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED) !=  HAL_OK){
	        Error_Handler();
	   }
    if (HAL_ADC_Start_DMA(&hadc2, (uint32_t*)adc_buffer, 50) != HAL_OK){
    	Error_Handler();
    }

    if (HAL_TIM_Base_Start(&htim6) !=  HAL_OK){
        Error_Handler();
    }

}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc){
    osThreadFlagsSet (adc_thread_id, error);
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc){
    osThreadFlagsSet (adc_thread_id, full);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc){
    osThreadFlagsSet (adc_thread_id, half);
}

osThreadAttr_t thred_attr;
void my_ADC_init(ADC_HandleTypeDef *hadc_ptr, ADC_HandleTypeDef *hadc_ptr_CAL, TIM_HandleTypeDef *htim){

    if (my_adc_start(&hadc1) !=  HAL_OK){
        Error_Handler();
    }

    if (my_dma_start() !=  HAL_OK){
            Error_Handler();
    }

    adc_thread_id = osThreadNew((void *)adc_thread, NULL, &thred_attr );

}
