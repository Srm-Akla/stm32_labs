/*
 * LedFun.h
 *
 *  Created on: Nov 20, 2020
 *      Author: Admin
 */

#ifndef LEDFUN_H_
#define LEDFUN_H_

#include "stdbool.h"
#include "cmsis_os2.h"

osThreadId_t Led_Thread;


bool LedFunInit(void);
void LedFun(void *arg);

#endif /* LEDFUN_H_ */
