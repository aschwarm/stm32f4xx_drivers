/*
 * stm32f407_gpio_driver.h
 *
 *  Created on: Dec 4, 2024
 *      Author: adams
 */

#ifndef INC_STM32F407_GPIO_DRIVER_H_
#define INC_STM32F407_GPIO_DRIVER_H_

#include "stm32f407xx.h"


typedef struct
{
	uint8_t GPIO_PinNumber;			/*!< Possible values from @GPIO_PIN_NUMBERS >*/
	uint8_t GPIO_PinMode;			/*!< Possible values from @GPIO_PIN_MODES >*/
	uint8_t GPIO_PinSpeed;			/*!< Possible values from @GPIO_OUT_SPEEDS >*/
	uint8_t GPIO_PinPuPdControl;	/*!< Possible values from @GPIO_PUPD_MODES >*/
	uint8_t GPIO_PinOPType;			/*!< Possible values from @GPIO_OUTPUT_TYPES >*/
	uint8_t GPIO_PinAltFunMode;

} GPIO_PinConfig_t;

/*
 * This is a handle structure for a GPIO pin
 */

typedef struct
{
	GPIO_RegDef_t *pGPIOx;
	GPIO_PinConfig_t GPIO_PinConfig;
}GPIO_Handle_t;

/*
 * @GPIO_PIN_NUMBERS
 * GPIO pin numbers
 */
#define GPIO_PIN_NO_0 		0
#define GPIO_PIN_NO_1 		1
#define GPIO_PIN_NO_2 		2
#define GPIO_PIN_NO_3 		3
#define GPIO_PIN_NO_4 		4
#define GPIO_PIN_NO_5 		5
#define GPIO_PIN_NO_6 		6
#define GPIO_PIN_NO_7 		7
#define GPIO_PIN_NO_8 		8
#define GPIO_PIN_NO_9 		9
#define GPIO_PIN_NO_10 		10
#define GPIO_PIN_NO_11 		11
#define GPIO_PIN_NO_12 		12
#define GPIO_PIN_NO_13 		13
#define GPIO_PIN_NO_14 		14
#define GPIO_PIN_NO_15 		15


/*
 * @GPIO_PIN_MODES
 * GPIO pin possible modes
 */
#define GPIO_MODE_IN 		0
#define GPIO_MODE_OUT 		1
#define GPIO_MODE_ALTFN  	2
#define GPIO_MODE_ANALOG 	3
#define GPIO_MODE_IT_FT		4
#define GPIO_MODE_IT_RT		5
#define GPIO_MODE_IT_RFT	6

/*
 * @GPIO_OUTPUT_TYPES
 * GPIO pin possible output types
 */
#define GPIO_OP_TYPE_PP		0
#define GPIO_OP_TYPE_OD		1


/*
 * @GPIO_OUT_SPEEDS
 * GPIO pin possible output speeds
 */
#define GPIO_SPEED_LOW		0
#define GPIO_SPEED_MEDIUM	1
#define GPIO_SPEED_HIGH		2
#define GPIO_SPEED_V_HIGH	3

/*
 * @GPIO_PUPD_MODES
 * GPIO pin pull up pull down configuration macros
 */
#define GPIO_NO_PUPD		0
#define GPIO_PU				1
#define GPIO_PD				2

/*
 * APIs supported by this driver
 */

void GPIO_PeriClckCtrl(GPIO_RegDef_t *pGPIOx, uint8_t EnOrDi);

void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

/*
 * Data read and write
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint8_t Value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);


void GPIO_IRQInteruptConfig(uint8_t IRQNumber, uint8_t EnOrDi);
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void GPIO_IRQHandling(uint8_t PinNumber);

#endif /* INC_STM32F407_GPIO_DRIVER_H_ */
