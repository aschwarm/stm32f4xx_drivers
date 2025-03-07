/*
 * 015uart_tx.c
 *
 *  Created on: Mar 5, 2025
 *      Author: adams
 */

#include "stm32f407xx.h"
#include <string.h>
#include <stdint.h>

void delay(void){
	for(uint32_t i = 0; i < 500000; i++);

}

char msg[1024] = "USART Tx testing... \n\r";

USART_Handle_t usart2_handle;

void USART2_GPIOInit(void)
{
	GPIO_Handle_t usart_gpios;

	usart_gpios.pGPIOx = GPIOA;
	usart_gpios.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	usart_gpios.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	usart_gpios.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PU;
	usart_gpios.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	usart_gpios.GPIO_PinConfig.GPIO_PinAltFunMode = 7;

	//USART2 TX
	usart_gpios.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_2;
	GPIO_Init(&usart_gpios);

	//USART2 RX
	usart_gpios.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_3;
	GPIO_Init(&usart_gpios);
}

void USART2_Init(void)
{
	usart2_handle.pUSARTx = USART2;
	usart2_handle.USARTConfig.USART_Baud = USART_STD_BAUD_115200;
	usart2_handle.USARTConfig.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
	usart2_handle.USARTConfig.USART_Mode = USART_MODE_ONLY_TX;
	usart2_handle.USARTConfig.USART_NoOfStopBits = USART_STOPBITS_1;
	usart2_handle.USARTConfig.USART_ParityControl = USART_PARITY_DISABLE;
	usart2_handle.USARTConfig.USART_WordLength = USART_WORDLEN_8BITS;
	USART_Init(&usart2_handle);
}

void GPIO_ButtonInit(void)
{
	GPIO_Handle_t GpioBtn;

	GpioBtn.pGPIOx = GPIOA;
	GpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;


	GPIO_Init(&GpioBtn);
}

int main(void)
{
	GPIO_ButtonInit();

	USART2_GPIOInit();

	USART2_Init();

	USART_PeripheralControl(USART2, ENABLE);

	// wait until button is pressed
	while(! GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));

	// avoid button de-bouncing
	delay();

	USART_SendData(&usart2_handle, (uint8_t*)msg, strlen(msg));

	while(1);

	return 0;
}

