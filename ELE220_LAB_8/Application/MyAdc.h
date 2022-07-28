/*
 * MyAdc.h
 *
 *  Created on: Nov 12, 2021
 *      Author: Admin
 */
#include "adc.h"
#include "dma.h"
#include "tim.h"

#ifndef MYADC_H_
#define MYADC_H_

void my_ADC_init(ADC_HandleTypeDef *hadc_ptr, ADC_HandleTypeDef *hadc_ptr_CAL, TIM_HandleTypeDef *htim);

#endif /* MYADC_H_ */
