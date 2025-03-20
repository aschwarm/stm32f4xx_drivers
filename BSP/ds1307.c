/*
 * ds1307.c
 *
 *  Created on: Mar 11, 2025
 *      Author: adams
 */

#include "ds1307.h"
#include <stdint.h>
#include <string.h>

static void ds1307_i2c_pin_config(void);
static void ds1307_i2c_config(void);
static void ds1307_write(uint8_t value, uint8_t reg_address);
static uint8_t ds1307_read(uint8_t reg_address);

static uint8_t bcd_to_binary(uint8_t value);
static uint8_t binary_to_bcd(uint8_t value);

I2C_Handle_t g_ds1307I2C_handle;

uint8_t ds1307_init(void)
{
	//1. initialize the i2c pins
	ds1307_i2c_pin_config();

	//2. Initialize the i2c peripheral
	ds1307_i2c_config();

	//3. Enable the I2C peripheral
	I2C_PeripheralControl(DS1307_I2C, ENABLE);

	//4. Make clock halt = 0;
	ds1307_write(0x00, DS1307_ADDR_SEC);

	//5. Read back clock halt bit
	uint8_t clock_state = ds1307_read(DS1307_ADDR_SEC);

	return ((clock_state >> 7) & 0x1 );
}

void ds1307_set_current_time(RTC_time_t *pRtc_time)
{
	uint8_t seconds, hrs;
	seconds = binary_to_bcd(pRtc_time->seconds);
	seconds  &= ~(1<<7);

	ds1307_write(seconds, DS1307_ADDR_SEC);

	ds1307_write(binary_to_bcd(pRtc_time->minutes), DS1307_ADDR_MIN);

	hrs = binary_to_bcd(pRtc_time->hours);

	if(pRtc_time->time_format == TIME_FORMAT_24HRS)
	{
		hrs &= ~(1<<6);
	}
	else
	{
		hrs |= (1<<6);
		hrs = (pRtc_time->time_format == TIME_FORMAT_12HRS_PM) ? hrs | (1<<5) : hrs & ~(1<<5);
	}

	ds1307_write(hrs, DS1307_ADDR_HRS);
}

void ds1307_get_current_time(RTC_time_t *pRtc_time)
{
	uint8_t seconds, hrs;

	seconds = ds1307_read(DS1307_ADDR_SEC);

	seconds &= ~(1<<7);

	pRtc_time->seconds = bcd_to_binary(seconds);
	pRtc_time->minutes = bcd_to_binary(ds1307_read(DS1307_ADDR_MIN));

	hrs = ds1307_read(DS1307_ADDR_HRS);

	if(hrs & (1<<6)){
		//12 hrs format
		pRtc_time->time_format = !((hrs & (1 << 5)) == 0);
		hrs &= ~(0x3 << 5); //clear bit 6 and 5
	}
	else{
		//24 hrs format
		pRtc_time->time_format = TIME_FORMAT_24HRS;
	}
	pRtc_time->hours = bcd_to_binary(hrs);
}

void ds1307_set_current_date(RTC_date_t *pRtc_date)
{
	ds1307_write(binary_to_bcd(pRtc_date->date), DS1307_ADDR_DATE);

	ds1307_write(binary_to_bcd(pRtc_date->month), DS1307_ADDR_MONTH);

	ds1307_write(binary_to_bcd(pRtc_date->year), DS1307_ADDR_YEAR);

	ds1307_write(binary_to_bcd(pRtc_date->day), DS1307_ADDR_DAY);
}

void ds1307_get_current_date(RTC_date_t *pRtc_date)
{
	pRtc_date->date = bcd_to_binary(ds1307_read(DS1307_ADDR_DATE));

	pRtc_date->month = bcd_to_binary(ds1307_read(DS1307_ADDR_MONTH));

	pRtc_date->year = bcd_to_binary(ds1307_read(DS1307_ADDR_YEAR));

	pRtc_date->day = bcd_to_binary(ds1307_read(DS1307_ADDR_DAY));
}

static void ds1307_i2c_pin_config(void)
{
	GPIO_Handle_t i2c_sda, i2c_scl;

	memset(&i2c_sda,0,sizeof(i2c_sda));
	memset(&i2c_scl,0,sizeof(i2c_scl));

	i2c_sda.pGPIOx = DS1307_I2C_GPIO_PORT;
	i2c_sda.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	i2c_sda.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	i2c_sda.GPIO_PinConfig.GPIO_PinPuPdControl = DS1307_I2C_PUPD;
	i2c_sda.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	i2c_sda.GPIO_PinConfig.GPIO_PinNumber = DS1307_I2C_SDA_PIN;
	i2c_sda.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;

	GPIO_Init(&i2c_sda);

	i2c_scl.pGPIOx = DS1307_I2C_GPIO_PORT;
	i2c_scl.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	i2c_scl.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	i2c_scl.GPIO_PinConfig.GPIO_PinPuPdControl = DS1307_I2C_PUPD;
	i2c_scl.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	i2c_scl.GPIO_PinConfig.GPIO_PinNumber = DS1307_I2C_SCL_PIN;
	i2c_scl.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;

	GPIO_Init(&i2c_scl);

}

static void ds1307_i2c_config(void)
{
	g_ds1307I2C_handle.pI2Cx = DS1307_I2C;
	g_ds1307I2C_handle.I2C_Config.I2C_ACKControl = I2C_ACK_ENABLE;
	g_ds1307I2C_handle.I2C_Config.I2C_SCLSpeed = DS1307_I2C_SPEED;
	I2C_Init(&g_ds1307I2C_handle);
}

static void ds1307_write(uint8_t value, uint8_t reg_address)
{
	uint8_t tx[2];

	tx[0] = reg_address;
	tx[1] = value;

	I2C_MasterSendData(&g_ds1307I2C_handle, tx, 2, DS1307_I2C_ADDR, 0);
}

static uint8_t ds1307_read(uint8_t reg_address)
{
	uint8_t data;
	I2C_MasterSendData(&g_ds1307I2C_handle, &reg_address, 1, DS1307_I2C_ADDR, 0);
	I2C_MasterReceiveData(&g_ds1307I2C_handle, &data, 1, DS1307_I2C_ADDR, 0);

	return data;
}

static uint8_t binary_to_bcd( uint8_t value)
{
	uint8_t m, n;
	uint8_t bcd;

	bcd = value;
	if(value >= 10)
	{
		m = value / 10;
		n = value % 10;
		bcd = ((m<<4) | n );
	}



	return bcd;
}

static uint8_t bcd_to_binary( uint8_t value)
{
	uint8_t m, n;

	m = (uint8_t)(value >> 4) * 10;
	n = value & (uint8_t)0x0F;

	return (m+n);
}
