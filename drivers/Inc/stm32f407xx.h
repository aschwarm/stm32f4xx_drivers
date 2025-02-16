/*
 * stm32f407xx.h
 *
 *  Created on: Dec 3, 2024
 *      Author: adams
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

#include <stdint.h>
#include <stddef.h>

#define __vo volatile
#define __weak __attribute__((weak))

/*
 * ARM Cortex Mx Processor NVIC ISERx register Addresses
 */
#define NVIC_ISER0			 ((__vo uint32_t*)0xE000E100)
#define NVIC_ISER1			 ((__vo uint32_t*)0xE000E104)
#define NVIC_ISER2			 ((__vo uint32_t*)0xE000E108)
#define NVIC_ISER3			 ((__vo uint32_t*)0xE000E10C)

/*
 * ARM Cortex Mx Processor NVIC ICERx register Addresses
 */
#define NVIC_ICER0			 ((__vo uint32_t*)0xE000E180)
#define NVIC_ICER1			 ((__vo uint32_t*)0xE000E184)
#define NVIC_ICER2			 ((__vo uint32_t*)0xE000E188)
#define NVIC_ICER3			 ((__vo uint32_t*)0xE000E18C)

/*
 * ARM Cortex Mx Processor Priority Register Address Calculation
 */
#define NVIC_PR_BASE_ADDR ((__vo uint32_t*)0xE000E400)

#define NO_PR_BITS_IMPLEMENTED			4

/*
 * base addresses of Flash and SRAM memories
 */

#define FLASH_BASEADDR 			0x08000000U 		/*!<Flash memory base address>!*/
#define SRAM1_BASEADDR			0x20000000U 		/*!<SRAM1 base address>!*/
#define SRAM2_BASEADDR			0x2001C000U 		/*!<SRAM2 base address>!*/
#define ROM_BASEADDR 			0x1FFF0000U			/*!<System memory base address>!*/
#define SRAM					SRAM1_BASEADDR		/*!<SRAM base address>!*/


/*
 * AHBx and APBx Bus Peripheral base addresses
 */

#define PERIPH_BASEADDR			0x40000000U
#define APB1PERIPH_BASEADDR		PERIPH_BASEADDR
#define APB2PERIPH_BASEADDR		0x40010000U
#define AHB1PERIPH_BASEADDR		0x40020000U
#define AHB2PERIPH_BASEADDR		0x50000000U

/*
 * Base addresses of peripherals which are hanging on AHB1 bus
 */

#define GPIOA_BASEADDR		(AHB1PERIPH_BASEADDR + 0x0000)
#define GPIOB_BASEADDR		(AHB1PERIPH_BASEADDR + 0x0400)
#define GPIOC_BASEADDR		(AHB1PERIPH_BASEADDR + 0x0800)
#define GPIOD_BASEADDR		(AHB1PERIPH_BASEADDR + 0x0C00)
#define GPIOE_BASEADDR		(AHB1PERIPH_BASEADDR + 0x1000)
#define GPIOF_BASEADDR		(AHB1PERIPH_BASEADDR + 0x1400)
#define GPIOG_BASEADDR		(AHB1PERIPH_BASEADDR + 0x1800)
#define GPIOH_BASEADDR		(AHB1PERIPH_BASEADDR + 0x1C00)
#define GPIOI_BASEADDR		(AHB1PERIPH_BASEADDR + 0x2000)

#define RCC_BASEADDR		(AHB1PERIPH_BASEADDR + 0x3800)
/*
 * Base addresses of peripherals which are hanging on APB1 bus
 */
#define I2C1_BASEADDR 		(APB1PERIPH_BASEADDR + 0x5400)
#define I2C2_BASEADDR 		(APB1PERIPH_BASEADDR + 0x5800)
#define I2C3_BASEADDR		(APB1PERIPH_BASEADDR + 0x5C00)

#define SPI2_BASEADDR		(APB1PERIPH_BASEADDR + 0x3800)
#define SPI3_BASEADDR		(APB1PERIPH_BASEADDR + 0x3C00)

#define USART2_BASEADDR		(APB1PERIPH_BASEADDR + 0x4400)
#define USART3_BASEADDR		(APB1PERIPH_BASEADDR + 0x4800)
#define UART4_BASEADDR		(APB1PERIPH_BASEADDR + 0x4C00)
#define UART5_BASEADDR		(APB1PERIPH_BASEADDR + 0x5000)

/*
 * Base addresses of peripherals which are hanging on APB2 bus
 */
#define SPI1_BASEADDR 		(APB2PERIPH_BASEADDR + 0x3000)
#define SPI4_BASEADDR 		(APB2PERIPH_BASEADDR + 0x3400)

#define USART1_BASEADDR 	(APB2PERIPH_BASEADDR + 0x1000)
#define USART6_BASEADDR		(APB2PERIPH_BASEADDR + 0x1400)

#define EXTI_BASEADDR		(APB2PERIPH_BASEADDR + 0x3C00)

#define SYSCFG_BASEADDR		(APB2PERIPH_BASEADDR + 0x3800)

/*
 * Peripheral register definition structures
 */

typedef struct
{
	__vo uint32_t MODER;		 /*!< GPIO port mode register, Address offset: 0x00>!*/
	__vo uint32_t OTYPER;		/*!< >!*/
	__vo uint32_t OSPEEDR;		/*!< >!*/
	__vo uint32_t PUPDR;		/*!< >!*/
	__vo uint32_t IDR;			/*!< >!*/
	__vo uint32_t ODR;			/*!< >!*/
	__vo uint32_t BSRR;			/*!< >!*/
	__vo uint32_t LCKR;			/*!< >!*/
	__vo uint32_t AFR[2];		/*!<AFR[0] : GPIO alternate function low register, AFR[1] : GPIO alternate function high register>!*/

}GPIO_RegDef_t;


typedef struct
{
	__vo uint32_t CR;		 		/*!< Address offset: 0x00 >!*/
	__vo uint32_t PLLCFGR;			/*!< Address offset: 0x04 >!*/
	__vo uint32_t CFGR;				/*!< Address offset: 0x08 >!*/
	__vo uint32_t CIR;				/*!< Address offset: 0x0C >!*/
	__vo uint32_t AHB1RSTR;			/*!< Address offset: 0x10 >!*/
	__vo uint32_t AHB2RSTR;			/*!< Address offset: 0x14 >!*/
	__vo uint32_t AHB3RSTR;			/*!< Address offset: 0x18 >!*/
		 uint32_t RESERVED0;  		/*!< Address offset: 0x1C >!*/
	__vo uint32_t APB1RSTR;			/*!< Address offset: 0x20 >!*/
	__vo uint32_t APB2RSTR;			/*!< Address offset: 0x24 >!*/
		 uint32_t RESERVED1[2];  	/*!< Address offset: 0x28-0x2C >!*/
	__vo uint32_t AHB1ENR;			/*!< Address offset: 0x30 >!*/
	__vo uint32_t AHB2ENR;			/*!< Address offset: 0x34 >!*/
	__vo uint32_t AHB3ENR;			/*!< Address offset: 0x38 >!*/
		 uint32_t RESERVED2;    	/*!< Address offset: 0x3C >!*/
	__vo uint32_t APB1ENR;			/*!< Address offset: 0x40 >!*/
	__vo uint32_t APB2ENR;			/*!< Address offset: 0x44 >!*/
	 	 uint32_t RESERVED3[2]; 	/*!< Address offset: 0x48-0x4C >!*/
	__vo uint32_t AHB1LPENR;		/*!< Address offset: 0x50 >!*/
	__vo uint32_t AHB2LPENR;		/*!< Address offset: 0x54 >!*/
	__vo uint32_t AHB3LPENR;    	/*!< Address offset: 0x58 >!*/
	 	 uint32_t RESERVED4;    	/*!< Address offset: 0x5C >!*/
	__vo uint32_t APB1LPENR; 		/*!< Address offset: 0x60 >!*/
	__vo uint32_t APB2LPENR; 		/*!< Address offset: 0x64 >!*/
	 	 uint32_t RESERVED5[2]; 	/*!< Address offset: 0x68-0x6C >!*/
	__vo uint32_t BDCR; 			/*!< Address offset: 0x70 >!*/
	__vo uint32_t CSR; 				/*!< Address offset: 0x74 >!*/
		 uint32_t RESERVED6[2]; 	/*!< Address offset: 0x78-0x7C >!*/
	__vo uint32_t SSCGR; 			/*!< Address offset: 0x80 >!*/
	__vo uint32_t PLLI2SCFGR; 		/*!< Address offset: 0x84 >!*/

}RCC_RegDef_t;


typedef struct {
	__vo uint32_t IMR;		 		/*!< Address offset: 0x00 >!*/
	__vo uint32_t EMR;				/*!< Address offset: 0x04 >!*/
	__vo uint32_t RTSR;				/*!< Address offset: 0x08 >!*/
	__vo uint32_t FTSR;				/*!< Address offset: 0x0C >!*/
	__vo uint32_t SWIER;			/*!< Address offset: 0x10 >!*/
	__vo uint32_t PR;				/*!< Address offset: 0x14 >!*/

}EXTI_RegDef_t;


typedef struct{
	__vo uint32_t MEMRMP;		 	/*!< Address offset: 0x00 >!*/
	__vo uint32_t PMC;				/*!< Address offset: 0x04 >!*/
	__vo uint32_t EXTICR[4];		/*!< Address offset: 0x08-0x14 >!*/
		 uint32_t RESERVED0[2];		/*!< Address offset: 0x18-0x1C >!*/
	__vo uint32_t CMPCR;			/*!< Address offset: 0x20 >!*/
}SYSCFG_RegDef_t;


typedef struct
{
	__vo uint32_t CR1;		/*!< Address offset: 0x00 >!*/
	__vo uint32_t CR2;		/*!< Address offset: 0x04 >!*/
	__vo uint32_t SR;		/*!< Address offset: 0x08 >!*/
	__vo uint32_t DR;		/*!< Address offset: 0x0C >!*/
	__vo uint32_t CRCPR;	/*!< Address offset: 0x10 >!*/
	__vo uint32_t RXCRCR;	/*!< Address offset: 0x14 >!*/
	__vo uint32_t TXCRCR;	/*!< Address offset: 0x18 >!*/
	__vo uint32_t I2SCFGR;	/*!< Address offset: 0x1C >!*/
	__vo uint32_t I2SPR;	/*!< Address offset: 0x20 >!*/

}SPI_RegDef_t;


typedef struct
{
	__vo uint32_t CR1;		/*!< Address offset: 0x00 >!*/
	__vo uint32_t CR2;		/*!< Address offset: 0x04 >!*/
	__vo uint32_t OAR1;		/*!< Address offset: 0x08 >!*/
	__vo uint32_t OAR2;		/*!< Address offset: 0x0C >!*/
	__vo uint32_t DR;		/*!< Address offset: 0x10 >!*/
	__vo uint32_t SR1;		/*!< Address offset: 0x14 >!*/
	__vo uint32_t SR2;		/*!< Address offset: 0x18 >!*/
	__vo uint32_t CCR;		/*!< Address offset: 0x1C >!*/
	__vo uint32_t TRISE;	/*!< Address offset: 0x20 >!*/
	__vo uint32_t FLTR;		/*!< Address offset: 0x24 >!*/

}I2C_RegDef_t;

/*
 * TODO: Complete for USART peripherals
 */
typedef struct
{

}USART_RegDef_t;


/*
 * peripheral definitions ( Peripheral base address typecasted to xxx_RegDef_t )
 */
#define GPIOA 		((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define GPIOB 		((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC 		((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD 		((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOE 		((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOF 		((GPIO_RegDef_t*)GPIOF_BASEADDR)
#define GPIOG 		((GPIO_RegDef_t*)GPIOG_BASEADDR)
#define GPIOH 		((GPIO_RegDef_t*)GPIOH_BASEADDR)
#define GPIOI 		((GPIO_RegDef_t*)GPIOI_BASEADDR)

#define RCC			((RCC_RegDef_t*)RCC_BASEADDR)

#define EXTI 		((EXTI_RegDef_t*)EXTI_BASEADDR)
#define SYSCFG		((SYSCFG_RegDef_t*)SYSCFG_BASEADDR)

#define SPI1		((SPI_RegDef_t*)SPI1_BASEADDR)
#define SPI2		((SPI_RegDef_t*)SPI2_BASEADDR)
#define SPI3		((SPI_RegDef_t*)SPI3_BASEADDR)
#define SPI4		((SPI_RegDef_t*)SPI4_BASEADDR)

#define I2C1		((I2C_RegDef_t*)I2C1_BASEADDR)
#define I2C2		((I2C_RegDef_t*)I2C2_BASEADDR)
#define I2C3		((I2C_RegDef_t*)I2C3_BASEADDR)

/*
 * Clock Enable Macros for GPIOx peripherals
 */
#define GPIOA_PCLK_EN() (RCC->AHB1ENR |= (1 << 0))
#define GPIOB_PCLK_EN() (RCC->AHB1ENR |= (1 << 1))
#define GPIOC_PCLK_EN() (RCC->AHB1ENR |= (1 << 2))
#define GPIOD_PCLK_EN() (RCC->AHB1ENR |= (1 << 3))
#define GPIOE_PCLK_EN() (RCC->AHB1ENR |= (1 << 4))
#define GPIOF_PCLK_EN() (RCC->AHB1ENR |= (1 << 5))
#define GPIOG_PCLK_EN() (RCC->AHB1ENR |= (1 << 6))
#define GPIOH_PCLK_EN() (RCC->AHB1ENR |= (1 << 7))
#define GPIOI_PCLK_EN() (RCC->AHB1ENR |= (1 << 8))


/*
 * Clock Enable Macros for I2Cx peripherals
 */
#define I2C1_PCLK_EN()	(RCC->APB1ENR |= (1 << 21))
#define I2C2_PCLK_EN()	(RCC->APB1ENR |= (1 << 22))
#define I2C3_PCLK_EN()	(RCC->APB1ENR |= (1 << 23))


/*
 * Clock Enable Macros for SPIx peripherals
 */
#define SPI1_PCLK_EN() 	(RCC->APB2ENR |= (1 << 12))
#define SPI2_PCLK_EN()	(RCC->APB1ENR |= (1 << 14))
#define SPI3_PCLK_EN()	(RCC->APB1ENR |= (1 << 15))
#define SPI4_PCLK_EN()	(RCC->APB2ENR |= (1 << 13))
#define SPI5_PCLK_EN()	(RCC->APB2ENR |= (1 << 20))
#define SPI6_PCLK_EN()  (RCC->APB2ENR |= (1 << 21))

/*
 * Clock Enable Macros for USARTx peripherals
 */
#define USART2_PCLK_EN()	(RCC->APB1ENR |= (1 << 17))
#define USART3_PCLK_EN()	(RCC->APB1ENR |= (1 << 18))

/*
 * Clock Enable Macros for SYSCFG peripheral
 */
#define SYSCFG_PCLK_EN() (RCC->APB2ENR |= (1 << 14))


/*
 * Clock Disable Macros for GPIOx peripherals
 */
#define GPIOA_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 0))
#define GPIOB_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 1))
#define GPIOC_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 2))
#define GPIOD_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 3))
#define GPIOE_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 4))
#define GPIOF_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 5))
#define GPIOG_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 6))
#define GPIOH_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 7))
#define GPIOI_PCLK_DI() (RCC->AHB1ENR &= ~(1 << 8))

/*
 * Clock Disable Macros for I2Cx peripherals
 */
#define I2C1_PCLK_DI()	(RCC->APB1ENR &= ~(1 << 21))
#define I2C2_PCLK_DI()	(RCC->APB1ENR &= ~(1 << 22))
#define I2C3_PCLK_DI()	(RCC->APB1ENR &= ~(1 << 23))
/*
 * Clock Disable Macros for SPIx peripherals
 */
#define SPI1_PCLK_DI() 	(RCC->APB2ENR &= ~(1 << 12))
#define SPI2_PCLK_DI()	(RCC->APB1ENR &= ~(1 << 14))
#define SPI3_PCLK_DI()	(RCC->APB1ENR &= ~(1 << 15))
#define SPI4_PCLK_DI()	(RCC->APB2ENR &= ~(1 << 13))
#define SPI5_PCLK_DI()	(RCC->APB2ENR &= ~(1 << 20))
#define SPI6_PCLK_DI()  (RCC->APB2ENR &= ~(1 << 21))


/*
 * Clock Disable Macros for USARTx peripherals
 */
#define USART2_PCLK_DI()	(RCC->APB1ENR &= ~(1 << 17))
#define USART3_PCLK_DI()	(RCC->APB1ENR &= ~(1 << 18))

/*
 * Clock Disable Macros for SYSCFG peripheral
 */

/*
 * Macros to reset GPIOx peripherals
 */
#define GPIOA_REG_RESET()	do {(RCC->AHB1RSTR |= (1 << 0)); (RCC->AHB1RSTR &= ~(1 << 0));} while(0)
#define GPIOB_REG_RESET()	do {(RCC->AHB1RSTR |= (1 << 1)); (RCC->AHB1RSTR &= ~(1 << 1));} while(0)
#define GPIOC_REG_RESET()	do {(RCC->AHB1RSTR |= (1 << 2)); (RCC->AHB1RSTR &= ~(1 << 2));} while(0)
#define GPIOD_REG_RESET()	do {(RCC->AHB1RSTR |= (1 << 3)); (RCC->AHB1RSTR &= ~(1 << 3));} while(0)
#define GPIOE_REG_RESET()	do {(RCC->AHB1RSTR |= (1 << 4)); (RCC->AHB1RSTR &= ~(1 << 4));} while(0)
#define GPIOF_REG_RESET()	do {(RCC->AHB1RSTR |= (1 << 5)); (RCC->AHB1RSTR &= ~(1 << 5));} while(0)
#define GPIOG_REG_RESET()	do {(RCC->AHB1RSTR |= (1 << 6)); (RCC->AHB1RSTR &= ~(1 << 6));} while(0)
#define GPIOH_REG_RESET()	do {(RCC->AHB1RSTR |= (1 << 7)); (RCC->AHB1RSTR &= ~(1 << 7));} while(0)
#define GPIOI_REG_RESET()	do {(RCC->AHB1RSTR |= (1 << 8)); (RCC->AHB1RSTR &= ~(1 << 8));} while(0)

#define GPIO_BASEADDR_TO_CODE(x)	((x == GPIOA) ? 0 :\
									(x == GPIOB) ? 1 :\
									(x == GPIOC) ? 2 :\
									(x == GPIOD) ? 3 :\
									(x == GPIOE) ? 4 :\
									(x == GPIOF) ? 5 :\
									(x == GPIOG) ? 6 :\
									(x == GPIOH) ? 7 :\
									(x == GPIOI) ? 8 :0)
/*
 * Macros to reset SPIx peripherals
 */
#define SPI1_REG_RESET()	do {(RCC->APB2RSTR |= (1 << 12)); (RCC->APB2RSTR &= ~(1 << 12));} while(0)
#define SPI2_REG_RESET()	do {(RCC->APB1RSTR |= (1 << 14)); (RCC->APB1RSTR &= ~(1 << 14));} while(0)
#define SPI3_REG_RESET()	do {(RCC->APB1RSTR |= (1 << 15)); (RCC->APB1RSTR &= ~(1 << 15));} while(0)
#define SPI4_REG_RESET()	do {(RCC->APB2RSTR |= (1 << 13)); (RCC->APB2RSTR &= ~(1 << 13));} while(0)

/*
 * Macros to reset I2Cx peripherals
 */
#define I2C1_REG_RESET()	do {(RCC->APB1RSTR |= (1 << 21)); (RCC->APB1RSTR &= ~(1 << 21));} while(0)
#define I2C2_REG_RESET()	do {(RCC->APB1RSTR |= (1 << 22)); (RCC->APB1RSTR &= ~(1 << 22));} while(0)
#define I2C3_REG_RESET()	do {(RCC->APB1RSTR |= (1 << 23)); (RCC->APB1RSTR &= ~(1 << 23));} while(0)

/*
 * IRQ (Interrupt Request) Number of STM32F407xx MCU
 */
#define IRQ_NO_EXTI0			6
#define IRQ_NO_EXTI1			7
#define IRQ_NO_EXTI2			8
#define IRQ_NO_EXTI3			9
#define IRQ_NO_EXTI4			10
#define IRQ_NO_EXTI9_5			23
#define IRQ_NO_EXTI15_10		40
#define IRQ_NO_SPI1				35
#define IRQ_NO_SPI2				36
#define IRQ_NO_SPI3				51

/*
 * TODO NVIC_IRQ Priority (0-15)
 */
#define NVIC_IRQ_PRI0			0
#define NVIC_IRQ_PRI15			15

//some generic macros

#define ENABLE				1
#define DISABLE				0
#define SET					ENABLE
#define RESET				DISABLE
#define GPIO_PIN_SET		SET
#define GPIO_PIN_RESET		RESET
#define FLAG_RESET			RESET
#define FLAG_SET			SET

/**********************************************
 *		 Bit positions of SPI peripheral
 **********************************************
 */
#define SPI_CR1_CPHA 	0
#define SPI_CR1_CPOL	1
#define SPI_CR1_BR		3

#define SPI_SR_RXNE		0
#define SPI_SR_TXE		1
#define SPI_SR_BSY		7

/**********************************************
 *		 Bit positions of SPI peripheral
 **********************************************
 */
#define I2C_CR1_PE				0
#define I2C_CR1_NOSTRETCH		7
#define I2C_CR1_START			8
#define I2C_CR1_STOP			9
#define I2C_CR1_ACK				10
#define I2C_CR1_SWRST			15

// bit position definitions I2C_CR2
#define I2C_CR2_FREQ			0
#define I2C_CR2_ITERREN			8
#define I2C_CR2_ITEVTEN			9
#define I2C_CR2_ITBUFEN			10


// bit position definitions I2C_OAR1
#define I2C_OAR1_ADD0			0
#define I2C_OAR1_ADD71			1
#define I2C_OAR1_ADD98			8
#define I2C_OAR1_ADDMODE		15

// bit position definitions I2C_SR1
#define I2C_SR1_SB				0
#define I2C_SR1_ADDR			1
#define I2C_SR1_BTF				2
#define I2C_SR1_ADD10			3
#define I2C_SR1_STOPF			4
#define I2C_SR1_RXNE			6
#define I2C_SR1_TXE				7
#define I2C_SR1_BERR			8
#define I2C_SR1_ARLO			9
#define I2C_SR1_AF				10
#define I2C_SR1_OVR				11
#define I2C_SR1_TIMEOUT			14

// bit position definitions I2C_SR2
#define I2C_SR2_MSL				0
#define I2C_SR2_BUSY			1
#define I2C_SR2_TRA				2
#define I2C_SR2_GENCALL			4
#define I2C_SR2_DUALF			7

// bit position definitions I2C_CCR
#define I2C_CCR_CCR				0
#define I2C_CCR_DUTY			14
#define I2C_CCR_FS				15

#include "stm32f407_gpio_driver.h"
#include "stm32f407_spi_driver.h"
#include "stm32f407_i2c_driver.h"

#endif /* INC_STM32F407XX_H_ */
