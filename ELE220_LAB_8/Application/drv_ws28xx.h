#ifndef INCLUSION_GUARD_DRV_WS28XX_H
#define INCLUSION_GUARD_DRV_WS28XX_H

#include <stdint.h>
#include "main.h"

/** @brief Supported WS28xx ICs. */
typedef enum
{
  DRV_WS28XX_WS2811 = 0x01,
  DRV_WS28XX_WS2812,
  DRV_WS28XX_UNSUPPORTED
} DrvWS28xxSupportedTypeDef;


/** @brief Status codes returned from API functions declared in this file. */
typedef enum
{
  DRV_WS28XX_OK    = 0x00,
  DRV_WS28XX_ERROR = 0x01,
  DRV_WS28XX_BUSY  = 0x02
} DrvWS28xxStatusTypeDef;

/** @brief Maximum number of WS28xx LED drivers that can be updated. */
#define DRV_WS28XX_MAX_NUMBER_OF_LEDS 100

/**
 * @brief Function for initializing the WS28xx driver.
 *
 * This function does some basic check on the timer configuration selected
 * for the WS28xx driver and initializes the driver.
 *
 * Driver expects a timer configuration with DMA channel for feeding timer
 * capture/compare register with 32 bits (word) values for duty cycles.
 *
 * @param[in] htim         Pointer to selected timer handle.
 * @param[in] tim_channel  The timer capture/compare channel used, e.g. TIM_CHANNEL_4.
 * @param[in] ic           The selected LED driver IC.
 *
 * @retval    DRV_WS28XX_OK     If initialization was successful.
 * @retval    DRV_WS28XX_ERROR  If initialization failed.
 */
uint32_t DrvWS28xxInit(TIM_HandleTypeDef *htim, uint32_t tim_channel, DrvWS28xxSupportedTypeDef ic);

/**
 * @brief Function for updating a number of WS28xx ICs.
 *
 * The uint8_t array consist of PWM duty ratios for red/green/blue for each LED.
 * The size of the array is therefore 3 bytes * number of LEDs long.
 * E.g. if number of LEDs are 2, the byte array is 6 byte long and arranged like this:
 *    1st. byte red   LED1
 *    2nd. byte green LED1
 *    3rd. byte blue  LED1
 *    4th. byte red   LED2
 *    5th. byte green LED2
 *    6th. byte blue  LED2
 *
 * @param[in] rgb          Pointer to an array of WS28xx PWM duty ratios.
 * @param[in] nbr_of_leds  Number of LEDs is identical to number of WS28xx ICs.
 *
 * @retval    DRV_WS28XX_OK     If everything was successful.
 * @retval    DRV_WS28XX_BUSY   If driver is busy, the caller should try again later.
 * @retval    DRV_WS28XX_ERROR  If something went wrong.
 */
uint32_t DrvWS28xxSetColorLeds(uint8_t* rgb, uint8_t nbr_of_leds);

#endif /* INCLUSION_GUARD_DRV_WS28XX_H */
