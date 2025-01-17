/*
 * 005button_interupt.c
 *
 *  Created on: Jan 1, 2025
 *      Author: adams
 */

#include "stm32f407xx.h"
#include <stdint.h>
#include <string.h>

#define HIGH			1
#define LOW				0
#define BTN_PRESSED		LOW

void delay(void){
	for(uint32_t i = 0; i < 500000; i++);

}

int main(void){
	// LEDs @ PD12,13,14,15
	//on board RED LED @ PD14

	GPIO_Handle_t GpioLed,GpioBtn;
	memset(&GpioLed,0,sizeof(GpioLed));
	memset(&GpioBtn,0,sizeof(GpioBtn));

	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;



	GpioBtn.pGPIOx = GPIOD;
	GpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
	GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_RT;
	GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;

	GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PU;

	GPIO_PeriClckCtrl(GpioLed.pGPIOx, ENABLE);
	GPIO_Init(&GpioLed);

	GPIO_PeriClckCtrl(GpioBtn.pGPIOx, ENABLE);
	GPIO_Init(&GpioBtn);

	//IRQ Configurations
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI9_5, NVIC_IRQ_PRI15);
	GPIO_IRQInteruptConfig(IRQ_NO_EXTI9_5, ENABLE);



	while(1)
	{
	};

	return 0;
}
	//ISR
void EXTI9_5_IRQHandler(void)
{
	delay();
	GPIO_IRQHandling(GPIO_PIN_NO_5);
	GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_15);
}
