/*
 * myI2C.h
 *
 *  Created on: Aug 7, 2024
 *      Author: Le Anh
 */

#ifndef INC_MYI2C_H_
#define INC_MYI2C_H_

#include "myI2C_UserConfig.h"
/*
 *	Address base
 */
#define FLASH_BASE            0x08000000UL /*!< FLASH base address in the alias region */
#define FLASH_BANK1_END       0x0801FFFFUL /*!< FLASH END address of bank1 */
#define SRAM_BASE             0x20000000UL /*!< SRAM base address in the alias region */
#define PERIPH_BASE           0x40000000UL /*!< Peripheral base address in the alias region */

#define SRAM_BB_BASE          0x22000000UL /*!< SRAM base address in the bit-band region */
#define PERIPH_BB_BASE        0x42000000UL /*!< Peripheral base address in the bit-band region */


/*!< Peripheral memory map */
#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000UL)
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x00020000UL)

/*I2C base*/
#define I2C1_BASE             (APB1PERIPH_BASE + 0x00005400UL)
#define I2C2_BASE             (APB1PERIPH_BASE + 0x00005800UL)

/*others*/
#define RCC_BASE              (AHBPERIPH_BASE + 0x00001000UL)

/*
 *	defining the register structure
 */
typedef struct{
	uint16_t I2C_CR1;
	uint16_t I2C_CR2;
	uint16_t I2C_OAR1;
	uint16_t I2C_OAR2;
	uint16_t I2C_DR;
	uint16_t I2C_SR1;
	uint16_t I2C_SR2;
	uint16_t I2C_CCR;
	uint16_t I2C_TRISE;
}I2C_Regdef_t;


/* RCC register definition*/
typedef struct
{
  uint32_t CR;
  uint32_t CFGR;
  uint32_t CIR;
  uint32_t APB2RSTR;
  uint32_t APB1RSTR;
  uint32_t AHBENR;
  uint32_t APB2ENR;
  uint32_t APB1ENR;
  uint32_t BDCR;
  uint32_t CSR;
} RCC_RegDef_t;

#define I2C1                ((I2C_Regdef_t *)I2C1_BASE)
#define I2C2                ((I2C_Regdef_t *)I2C2_BASE)
#define RCC     			((RCC_RegDef_t*)RCC_BASE)

#define RCC_APB1ENR_I2C1EN_Pos 			21
#define RCC_APB1ENR_I2C2EN_Pos 			22


/*Enable clock for i2c peripheral*/
#define I2C1_PCLK_EN()		(RCC->APB1ENR|=(1U<<RCC_APB1ENR_I2C1EN_Pos))
#define I2C2_PCLK_EN()		(RCC->APB1ENR|=(1U<<RCC_APB1ENR_I2C2EN_Pos))

/*
 * Configuration structure for I2Cx peripheral
 */
typedef struct{
	uint32_t I2C_SCLSpeed;
	uint8_t I2C_DeviceAddress;
	uint8_t I2C_ACKControl;
	uint16_t I2C_FMDutyCycle;
}I2C_Config_t;

/*
 * Handle structure for I2Cx peripheral
 */

typedef struct{
	I2C_Config_t I2C_Config;
	RCC_RegDef_t * I2Cx;
}I2C_Handle_t;

/*
 * function prototype
 */


/*
 * Peripheral Clock setup
 */

void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

/*
 * Init and De - init
 */

void SPI_Init(I2C_RegDef_t *pI2Cx, I2C_Config_t *pI2CConfig);
void SPI_DeInit(I2C_RegDef_t *pI2Cx);

/*
 * Data Send and Receive
 */


/*
 * IRQ Configuaration and ISR handling
 */
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);


#endif /* INC_MYI2C_H_ */
