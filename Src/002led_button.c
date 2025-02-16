/*
 * 002led_button.c
 *
 *  Created on: Jan 1, 2025
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

	GPIO_Handle_t GpioLed,GpioBtn;

	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;



	GpioBtn.pGPIOx = GPIOA;
	GpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClckCtrl(GpioLed.pGPIOx, ENABLE);
	GPIO_Init(&GpioLed);

	GPIO_PeriClckCtrl(GpioBtn.pGPIOx, ENABLE);
	GPIO_Init(&GpioBtn);

	while(1)
	{
		if(GPIO_ReadFromInputPin(GpioBtn.pGPIOx, GpioBtn.GPIO_PinConfig.GPIO_PinNumber))
		{
			delay();
			GPIO_ToggleOutputPin(GpioLed.pGPIOx, GpioLed.GPIO_PinConfig.GPIO_PinNumber);
		}

	}
	return 0;
}
