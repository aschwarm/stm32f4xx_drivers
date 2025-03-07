/*
 * stm32f407_rcc_driver.h
 *
 *  Created on: Mar 5, 2025
 *      Author: adams
 */

#ifndef INC_STM32F407_RCC_DRIVER_H_
#define INC_STM32F407_RCC_DRIVER_H_

#include "stm32f407xx.h"

uint32_t RCC_GetPCLK1Value(void);
uint32_t RCC_GetPCLK2Value(void);
uint32_t RCC_GetPLLOutPutClock(void);

#endif /* INC_STM32F407_RCC_DRIVER_H_ */
