/*
 * 010i2c_master_tx_testing.c
 *
 *  Created on: Jan 25, 2025
 *      Author: adams
 */

#include <string.h>
#include "stm32f407xx.h"
#include <stdio.h>

extern void initialise_monitor_handles();

#define MY_ADDR  	0x61
#define SLAVE_ADDR 	0x68

void delay(void){
	for(uint32_t i = 0; i < 500000; i++);

}

I2C_Handle_t I2C1Handle;

uint8_t rcv_buf[32];

/*
 * PB6 -> SCL
 * PB7 -> SDA
 */

void I2C1_GPIOInits(void)
{
	GPIO_Handle_t I2CPins;

	I2CPins.pGPIOx = GPIOB;
	I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PU;
	I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;

	//scl
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;

	GPIO_Init(&I2CPins);

	//sda
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;

	GPIO_Init(&I2CPins);

}

void I2C1_Inits(void)
{
	I2C1Handle.pI2Cx = I2C1;
	I2C1Handle.I2C_Config.I2C_ACKControl = I2C_ACK_ENABLE;
	I2C1Handle.I2C_Config.I2C_DeviceAddress = MY_ADDR;
	I2C1Handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;
	I2C1Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;

	I2C_Init(&I2C1Handle);

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

int main()
{
	uint8_t command_code, len;

	initialise_monitor_handles();

	printf("Application is running\n");

	I2C1_GPIOInits();

	I2C1_Inits();

	GPIO_ButtonInit();

	//enable the i2c peripheral
	I2C_PeripheralControl(I2C1, ENABLE);

	//ack bit is made 1 after PE=1
	I2C_ManageAcking(I2C1,I2C_ACK_ENABLE);



	while(1){
		//wait till button is pressed
		while( ! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0) );

		//to avoid button de-bouncing related issues 200ms of delay
		delay();

		command_code = 0x51;

		I2C_MasterSendData(&I2C1Handle, &command_code, 1, SLAVE_ADDR,I2C_ENABLE_REPEAT_START);

		I2C_MasterReceiveData(&I2C1Handle, &len, 1, SLAVE_ADDR,I2C_ENABLE_REPEAT_START);

		command_code = 0x52;

		I2C_MasterSendData(&I2C1Handle, &command_code, 1, SLAVE_ADDR,I2C_ENABLE_REPEAT_START);

		I2C_MasterReceiveData(&I2C1Handle, rcv_buf, len, SLAVE_ADDR,I2C_DISABLE_REPEAT_START);

		rcv_buf[len+1] = '\0';

		printf("Data: %s", rcv_buf);


	}

	return 0;
}

