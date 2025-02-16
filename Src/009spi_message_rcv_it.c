/*
 * 009spi_message_rcv_it.c
 *
 *  Created on: Jan 23, 2025
 *      Author: adams
 */

// TODO Finish code: Master receive message from Slave



#include <string.h>
#include "stm32f407xx.h"
#include <stdio.h>

SPI_Handle_t SPI2handle;

#define MAX_LEN 	500

char RcvBuff[MAX_LEN];

volatile char ReadByte;

volatile uint8_t rcvStop = 0;

// This flag will be set in the interrupt handler of the Arduino interrupt GPIO
volatile uint8_t dataAvailable = 0;

void delay(void){
	for(uint32_t i = 0; i < 500000; i++);

}

/*
 * PB14 --> SPI2_MISO
 * PB15 --> SPI2_MOSI
 * PB13 --> SPI2_SCLK
 * PB12 --> SPI2_NSS
 * ALT function mode: 5
 */

void SPI2_GPIOInits(void){
	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;

	//SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	//MISO
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);

	//MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	//NSS
	 SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	 GPIO_Init(&SPIPins);

}

void SPI2_Inits(void){
	SPI_Handle_t SPI2handle;

	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8;
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_DI; //hardware slave management enable for NSS pin

	SPI_Init(&SPI2handle);


}

uint8_t SPI_VerifyResponse(uint8_t ackbyte)
{
	if(ackbyte == 0xF5)
	{
		//ack
		return 1;
	}

	return 0;
}


void SPI_SendRecieveData(SPI_RegDef_t* pSPIx, uint8_t cmdCode, uint8_t dummy_read, uint8_t dummy_write, uint8_t ackByte){
	//send command
	SPI_SendData(pSPIx,&cmdCode,1);

	//do dummy read to clear off the RXNE
	SPI_ReceiveData(pSPIx,&dummy_read,1);

	//Send some dummy byte to fetch the response from the slave
	SPI_SendData(pSPIx,&dummy_write,1);

	//read the ack byte received
	SPI_ReceiveData(pSPIx,&ackByte,1);

}

void Slave_GPIO_InterruptPinInit(void)
{
	GPIO_Handle_t spiIntPin;
	memset(&spiIntPin,0,sizeof(spiIntPin));

	//this is led gpio configuration
	spiIntPin.pGPIOx = GPIOD;
	spiIntPin.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	spiIntPin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
	spiIntPin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;
	spiIntPin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(&spiIntPin);

	GPIO_IRQPriorityConfig(IRQ_NO_EXTI9_5, NVIC_IRQ_PRI15);
	GPIO_IRQInteruptConfig(IRQ_NO_EXTI9_5, ENABLE);
}

int main(void){

	uint8_t dummy_write = 0xff;
	uint8_t dummy_read;
	uint8_t cmdCode;
	uint8_t ackByte;
	uint8_t args[2];

	Slave_GPIO_InterruptPinInit();

	//this function is used to initialize the GPIO pins to behave as SPI2 pins
	SPI2_GPIOInits();

	SPI2_Inits();

	/*
	 * making SSOE 1 does NSS output enable
	 * The NSS pin is automatically managed by the hardware.
	 * i.e. when SPE=1 NSS will be pulled to low
	 * and NSS pin will be high when SPI=0
	 */
	SPI_SSOEConfig(SPI2, ENABLE);

	SPI_IRQInteruptConfig(IRQ_NO_SPI2, ENABLE);

	while(1)
	{
		while(!dataAvailable);

		GPIO_IRQInteruptConfig(IRQ_NO_EXTI9_5,DISABLE);

		//enable SPI2 peripheral
		SPI_PeripheralControl(SPI2, ENABLE);

		while(!rcvStop)
		{
			// fetch the data from the SPI peripheral byte by byte in interrupt mode
			while( SPI_SendDataIT(&SPI2handle, &dummy_read, 1) == SPI_BUSY_IN_TX);
			while( SPI_ReceiveDataIT(&SPI2handle, &ReadByte, 1) == SPI_BUSY_IN_RX);
		}

		//confirm SPI is not busy
		while( SPI_GetFlagStatus(SPI2,SPI_BUSY_FLAG));

		//Disable the SPI2 peripheral
		SPI_PeripheralControl(SPI2, DISABLE);

		printf("Rcvd data = %s\n",RcvBuff);

		dataAvailable = 0;

		GPIO_IRQInteruptConfig(IRQ_NO_EXTI9_5, ENABLE);

	}

	return 0;
}

void SPI2_IRQHandler(void)
{
	SPI_IRQHandling(&SPI2handle);
}

void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv)
{
	static uint32_t i =0;

	// In the RX complete event, copy data in to rcv buffer. '\0' indicates end of message(rcvStop =1)
	if(AppEv == SPI_EVENT_RX_CMPLT)
	{
		RcvBuff[i++] = ReadByte;
		if(ReadByte == '\0' || (i == MAX_LEN))
		{
			rcvStop = 1;
			RcvBuff[i-1] = '\0';
			i = 0;
		}
	}
}

// Slave data available interrupt handler
void EXTI9_5_IRQHandler(void)
{
	GPIO_IRQHandling(GPIO_PIN_NO_6);
	dataAvailable = 1;
}
