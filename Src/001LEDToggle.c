/*
 * 001LEDToggle.c
 *
 *  Created on: Dec 28, 2024
 *      Author: adams
 */

#include "stm32f407xx.h"
#include <stdint.h>

void delay(void){
	for(uint32_t i = 0; i < 500000; i++);

}

int main(void){
	// LEDs @ PD12,13,14,15
	//on board RED LED @ PD14

	GPIO_Handle_t GpioLed;

	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClckCtrl(GpioLed.pGPIOx, ENABLE);

	GPIO_Init(&GpioLed);

	while(1)
	{
		GPIO_ToggleOutputPin(GpioLed.pGPIOx, GpioLed.GPIO_PinConfig.GPIO_PinNumber);
		delay();
	}
	return 0;
}
