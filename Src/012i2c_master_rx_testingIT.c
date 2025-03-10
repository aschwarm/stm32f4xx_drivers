/*
 * 012i2c_master_rx_testingIT.c
 *
 *  Created on: Feb 16, 2025
 *      Author: adams
 */

#include <string.h>
#include "stm32f407xx.h"
#include <stdio.h>

extern void initialise_monitor_handles();

//Flag variable
uint8_t rxComplt = RESET;

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

	//I2C IRQ configuration
	I2C_IRQInteruptConfig(IRQ_NO_I2C1_EV, ENABLE);
	I2C_IRQInteruptConfig(IRQ_NO_I2C1_ER, ENABLE);

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

		while(I2C_MasterSendDataIT(&I2C1Handle, &command_code, 1, SLAVE_ADDR,I2C_ENABLE_REPEAT_START) != I2C_READY);

		while(I2C_MasterReceiveDataIT(&I2C1Handle, &len, 1, SLAVE_ADDR,I2C_ENABLE_REPEAT_START) != I2C_READY);

		command_code = 0x52;

		while(I2C_MasterSendDataIT(&I2C1Handle, &command_code, 1, SLAVE_ADDR,I2C_ENABLE_REPEAT_START) != I2C_READY);

		while(I2C_MasterReceiveDataIT(&I2C1Handle, rcv_buf, len, SLAVE_ADDR,I2C_DISABLE_REPEAT_START) != I2C_READY);

		rxComplt = RESET;

		//wait until rx completes
		while(rxComplt != SET);

		rcv_buf[len+1] = '\0';

		printf("Data: %s", rcv_buf);

		rxComplt = RESET;


	}

	return 0;
}

void	I2C1_EV_IRQHandler(void)
{
	I2C_EV_IRQHandling(&I2C1Handle);
}

void	I2C1_ER_IRQHandler(void)
{
	I2C_ER_IRQHandling(&I2C1Handle);
}

void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv)
{
	if(AppEv == I2C_EV_TX_CMPLT){
		printf("Tx is completed\n");
	}
	else if(AppEv == I2C_EV_RX_CMPLT)
	{
		printf("Rx is completed\n");
		rxComplt = SET;
	}
	else if(AppEv == I2C_ERROR_AF)
	{
		printf("Error: Ack Failure\n");
		//in master ack failure happens when slave fails to send ack for the byte
		//sent from the master
		I2C_CloseSendData(pI2CHandle);

		//generate the stop condition to release the bus
		I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

		//hang in infinite loop
		while(1);
	}
}


