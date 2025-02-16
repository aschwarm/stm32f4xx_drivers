/*
 * 006spi_tx_testing.c
 *
 *  Created on: Jan 18, 2025
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
	// SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	// GPIO_Init(&SPIPins);

	//MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	//NSS
	// SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	// GPIO_Init(&SPIPins);

}

void SPI2_Inits(void){
	SPI_Handle_t SPI2handle;

	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV2;
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_EN; //software slave management enable for NSS pin

	SPI_Init(&SPI2handle);


}

int main(void){

	char user_data[] = "Hello world";

	SPI2_GPIOInits();

	SPI2_Inits();

	SPI_SSIConfig(SPI2,ENABLE);

	//enable SPI2 peripheral
	SPI_PeripheralControl(SPI2, ENABLE);


	SPI_SendData(SPI2, (uint8_t*)user_data, strlen(user_data));

	//confirm SPI is not busy
		while(SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));

	// disable SPI2 peripheral
	SPI_PeripheralControl(SPI2, DISABLE);

	while(1);

	return 0;
}
