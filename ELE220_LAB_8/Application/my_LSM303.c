/*
 * my_LSM303.c
 *
 *  Created on: Nov 17, 2021
 *      Author: admin
 */

#include "main.h"
#include <stdlib.h>
#include "cmsis_os2.h"
#include "tim.h"
#include "spi.h"
#include "MyPrintServer.h"

#define LSM303_WHO_AM_I_A_REG 0x0F
#define CTRL_REG4_A 0x23
#define CTRL_REG1_A 0x20

#define CTRL_REG4_A_buf 0b00000111
#define CTRL_REG1_A_buf 0b01010111

#define LSM303_Read 0x80
#define LSM303_Write 0x00

#define OUT_X_L_A 0x28
#define OUT_X_H_A 0x29

#define OUT_Y_L_A 0x2A
#define OUT_Y_H_A 0x2B

#define OUT_Z_L_A 0x2C
#define OUT_Z_H_A 0x2D

void LSM303_thread(){

	uint8_t adress;

	while(1){

		int16_t Z_data, Y_data, X_data;
		uint8_t buf[6];


		HAL_GPIO_WritePin(XL_CS_GPIO_Port, XL_CS_Pin, RESET);

		adress = OUT_X_L_A |LSM303_Read;


		if(HAL_SPI_Transmit(&hspi2, &adress, 1, 10) != HAL_OK )
			Error_Handler();


		if(HAL_SPI_Receive(&hspi2, buf, 6, 10) != HAL_OK )
			Error_Handler();


		HAL_GPIO_WritePin(XL_CS_GPIO_Port, XL_CS_Pin, SET);

		X_data = buf[1] << 8 | buf[0];
		Y_data = buf[3] << 8 | buf[2];
		Z_data = buf[5] << 8 | buf[4];

		PRINTF("X: %d\t Y: %d\t Z: %d\r\n", X_data, Y_data, Z_data); //0x41

		osDelay(10);
	}
}

int16_t who_am_i_func(){

	HAL_StatusTypeDef HAL_status;
	uint8_t adress;
	uint8_t buf;

	uint8_t WHO_AM_I_A;

	adress = LSM303_WHO_AM_I_A_REG|LSM303_Read;

	//set the CS pin to activate the LSM303
	HAL_GPIO_WritePin(XL_CS_GPIO_Port, XL_CS_Pin, RESET);

	if(HAL_SPI_Transmit(&hspi2, &adress, 1, 10) != HAL_OK )
		Error_Handler();

	if(HAL_SPI_Receive(&hspi2, &WHO_AM_I_A, 1, 10) != HAL_OK )
		Error_Handler();

	HAL_GPIO_WritePin(XL_CS_GPIO_Port, XL_CS_Pin, SET);

	PRINTF("WHO_AM_I_A 0x%x received\r\n", WHO_AM_I_A); //0x41


	//SPI setup-------------------------------------------------

	HAL_GPIO_WritePin(XL_CS_GPIO_Port, XL_CS_Pin, RESET);
	adress = CTRL_REG4_A|LSM303_Write;

	if(HAL_SPI_Transmit(&hspi2, &adress, 1, 10) != HAL_OK )
		Error_Handler();

	buf = CTRL_REG4_A_buf;

	if(HAL_SPI_Transmit(&hspi2, &buf, 1, 10) != HAL_OK )
		Error_Handler();

	HAL_GPIO_WritePin(XL_CS_GPIO_Port, XL_CS_Pin, SET);

	HAL_Delay(1);

	HAL_GPIO_WritePin(XL_CS_GPIO_Port, XL_CS_Pin, RESET);
	adress = CTRL_REG4_A|LSM303_Read;

	if(HAL_SPI_Transmit(&hspi2, &adress, 1, 10) != HAL_OK )
		Error_Handler();
	buf = 0;

	if(HAL_SPI_Receive(&hspi2, &buf, 1, 10) != HAL_OK )
		Error_Handler();

	HAL_GPIO_WritePin(XL_CS_GPIO_Port, XL_CS_Pin, SET);

	PRINTF("CTRL_REG4_A 0x%x received\r\n", buf); //0x41
	HAL_Delay(1);

	HAL_GPIO_WritePin(XL_CS_GPIO_Port, XL_CS_Pin, RESET);

	adress = CTRL_REG1_A|LSM303_Write;

	if(HAL_SPI_Transmit(&hspi2, &adress, 1, 10) != HAL_OK )
		Error_Handler();

	buf = CTRL_REG1_A_buf;

	if(HAL_SPI_Transmit(&hspi2, &buf, 1, 10) != HAL_OK )
		Error_Handler();

	HAL_GPIO_WritePin(XL_CS_GPIO_Port, XL_CS_Pin, SET);
	HAL_Delay(1);

	HAL_GPIO_WritePin(XL_CS_GPIO_Port, XL_CS_Pin, RESET);
	adress = CTRL_REG1_A|LSM303_Read;

	if(HAL_SPI_Transmit(&hspi2, &adress, 1, 10) != HAL_OK )
		Error_Handler();

	buf = 0;

	if(HAL_SPI_Receive(&hspi2, &buf, 1, 10) != HAL_OK )
		Error_Handler();

	HAL_GPIO_WritePin(XL_CS_GPIO_Port, XL_CS_Pin, SET);

	PRINTF("CTRL_REG1_A 0x%x received\r\n", buf); //0x41

	HAL_Delay(1);

	//end of setup-------------------------------------------------

	return WHO_AM_I_A;
}

osThreadAttr_t thred_attr;
void LSM303_init(){

	if(who_am_i_func() < 0)
		PRINTF("ERROR %i\r\n", who_am_i_func());

	//create LSM303 thread
	osThreadNew((void*)LSM303_thread, NULL, &thred_attr);
}
