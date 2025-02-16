/*
 * 008spi_cmd_handling.c
 *
 *  Created on: Jan 22, 2025
 *      Author: adams
 */

/*
 * PB14 --> SPI2_MISO
 * PB15 --> SPI2_MOSI
 * PB13 --> SPI2_SCLK
 * PB12 --> SPI2_NSS
 * ALT function mode: 5
 */

#include <string.h>
#include "stm32f407xx.h"
#include <stdio.h>

#define COMMAND_LED_CTRL		0x50
#define COMMAND_SENSOR_READ		0x51
#define COMMAND_LED_READ		0x52
#define COMMAND_PRINT			0x53
#define COMMAND_ID_READ			0x54

#define LED_ON		1
#define LED_OFF		0

// arduino analog pins
#define ANALOG_PIN0		0
#define ANALOG_PIN1		1
#define ANALOG_PIN2		2
#define ANALOG_PIN3		3
#define ANALOG_PIN4		4

// arduino LED
#define LED_PIN			9

void delay(void){
	for(uint32_t i = 0; i < 500000; i++);

}

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

uint8_t SPI_VerifyResponse(uint8_t ackbyte)
{
	if(ackbyte == 0xF5)
	{
		//ack
		return 1;
	}

	return 0;
}

void waitForBtnPress(void)
{
	//wait till button is pressed
	while( ! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0) );

	//to avoid button de-bouncing related issues 200ms of delay
	delay();
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

int main(void){

	uint8_t dummy_write = 0xff;
	uint8_t dummy_read;
	uint8_t cmdCode;
	uint8_t ackByte;
	uint8_t args[2];


	GPIO_ButtonInit();

	SPI2_GPIOInits();

	SPI2_Inits();

	/*
	 * making SSOE 1 does NSS output enable
	 * The NSS pin is automatically managed by the hardware.
	 * i.e. when SPE=1 NSS will be pulled to low
	 * and NSS pin will be high when SPI=0
	 */
	SPI_SSOEConfig(SPI2, ENABLE);

	while(1)
	{
		waitForBtnPress();

		//enable SPI2 peripheral
		SPI_PeripheralControl(SPI2, ENABLE);

		//1. CMD_LED_CTRL <pin no(1)> 	<value(1)>

			cmdCode = COMMAND_LED_CTRL;
			uint8_t args[2];

			SPI_SendRecieveData(SPI2,cmdCode, dummy_read, dummy_write, ackByte);

			if (SPI_VerifyResponse(ackByte))
			{
				// send arguments
				args[0] = LED_PIN;
				args[1] = LED_ON;
				SPI_SendData(SPI2, args, 2);
			}
			// end of COMMAND_LED_CTRL

		//2. CMD_SENSOR_READ <analog pin number(1) >

			waitForBtnPress();

			cmdCode = COMMAND_SENSOR_READ;

			SPI_SendRecieveData(SPI2,cmdCode, dummy_read, dummy_write, ackByte);

			if (SPI_VerifyResponse(ackByte))
			{
				args[0] = ANALOG_PIN0;

				// send arguments
				SPI_SendData(SPI2, args, 1);

				//do dummy read to clear off the RXNE
				SPI_ReceiveData(SPI2, &dummy_read, 1);

				//insert some delay so that slave can ready with the data
				delay();

				//Send some dummy bits (1 byte) fetch the response from the slave
				SPI_SendData(SPI2, &dummy_write, 1);

				uint8_t analog_read;
				SPI_ReceiveData(SPI2, &analog_read, 1);
				printf("COMMAND_SENSOR_READ %d\n",analog_read);
			}

			//3.  CMD_LED_READ 	 <pin no(1) >

				waitForBtnPress();

				cmdCode = COMMAND_LED_READ;

				SPI_SendRecieveData(SPI2,cmdCode, dummy_read, dummy_write, ackByte);

				if( SPI_VerifyResponse(ackByte))
				{
					args[0] = LED_PIN;

					//send arguments
					SPI_SendData(SPI2,args,1); //sending one byte of

					//do dummy read to clear off the RXNE
					SPI_ReceiveData(SPI2,&dummy_read,1);

					//insert some delay so that slave can ready with the data
					delay();

					//Send some dummy bits (1 byte) fetch the response from the slave
					SPI_SendData(SPI2,&dummy_write,1);

					uint8_t led_status;
					SPI_ReceiveData(SPI2,&led_status,1);
					printf("COMMAND_READ_LED %d\n",led_status);

				}

			//4. CMD_PRINT 		<len(2)>  <message(len) >

				waitForBtnPress();

				cmdCode = COMMAND_PRINT;

				SPI_SendRecieveData(SPI2,cmdCode, dummy_read, dummy_write, ackByte);

				uint8_t message[] = "Hello ! How are you ??";
				if( SPI_VerifyResponse(ackByte))
				{
					args[0] = strlen((char*)message);

					//send arguments
					SPI_SendData(SPI2,args,1); //sending length

					//do dummy read to clear off the RXNE
					SPI_ReceiveData(SPI2,&dummy_read,1);

					delay();

					//send message
					for(int i = 0 ; i < args[0] ; i++){
						SPI_SendData(SPI2,&message[i],1);
						SPI_ReceiveData(SPI2,&dummy_read,1);
					}

					printf("COMMAND_PRINT Executed \n");

				}

			//5. CMD_ID_READ

				waitForBtnPress();

				cmdCode = COMMAND_ID_READ;

				SPI_SendRecieveData(SPI2,cmdCode, dummy_read, dummy_write, ackByte);

				uint8_t id[11];
				uint32_t i=0;
				if( SPI_VerifyResponse(ackByte))
				{
					//read 10 bytes id from the slave
					for(  i = 0 ; i < 10 ; i++)
					{
						//send dummy byte to fetch data from slave
						SPI_SendData(SPI2,&dummy_write,1);
						SPI_ReceiveData(SPI2,&id[i],1);
					}

					id[10] = '\0';

					printf("COMMAND_ID : %s \n",id);

				}
			}


		//confirm SPI is not busy
		while(SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));

		// disable SPI2 peripheral
		SPI_PeripheralControl(SPI2, DISABLE);

	return 0;
}
