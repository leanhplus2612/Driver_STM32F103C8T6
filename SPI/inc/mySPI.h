/*
 * mySPI.h
 *
 *  Created on: Aug 3, 2024
 *      Author: LeAnh
 */

#ifndef INC_MYSPI_H_
#define INC_MYSPI_H_

#include <stdint.h>

#define PERIPH_BASE           0x40000000UL
#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000UL)
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x00020000UL)


#define SPI1_BASE             (APB2PERIPH_BASE + 0x00003000UL)
#define SPI2_BASE             (APB1PERIPH_BASE + 0x00003800UL)
#define RCC_BASE              (AHBPERIPH_BASE + 0x00001000UL)

/*
 *	another peripheral needed
 */

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

/*
 * Configuration structure for SPIx peripheral
 */

typedef enum{
	SPI_DEVICE_MODE_SLAVE 		= 0U,
	SPI_DEVICE_MODE_MASTER		= 1U
}SPI_DeviceMode_t;

typedef enum{
	SPI_BUS_FULL_DUPLEX 				= 0U,
	SPI_BUS_HALF_DUPLEX_TRANSMIT 		= 1U,
	SPI_BUS_HALF_DUPLEX_RECEIVE 		= 2U,
	SPI_BUS_SIMPLEX_RX_ONLY				= 3U
}SPI_BusConfig_t;

typedef enum{
	SPI_SCLK_SPEED_DIV2				= 0U,
	SPI_SCLK_SPEED_DIV4				= 1U,
	SPI_SCLK_SPEED_DIV8				= 2U,
	SPI_SCLK_SPEED_DIV16			= 3U,
	SPI_SCLK_SPEED_DIV32			= 4U,
	SPI_SCLK_SPEED_DIV64			= 5U,
	SPI_SCLK_SPEED_DIV128			= 6U,
	SPI_SCLK_SPEED_DIV256			= 7U,
}SPI_SclkSpeed_t;

typedef enum{
	SPI_DFF_8_BIT 					= 0U,
	SPI_DFF_16_BIT					= 1U
}SPI_DFF_t;


typedef enum{
	SPI_CPOL_LOW	= 0U,
	SPI_CPOL_HIGH	= 1U
}SPI_CPOL_t;

typedef enum{
	SPI_CPHA_LOW	= 0U,
	SPI_CPHA_HIGH	= 1U
}SPI_CPHA_t;

typedef enum{
	SPI_SSM_HW		= 0U,
	SPI_SSM_SW		= 1U
}SPI_SSM_t;

typedef struct{
	SPI_DeviceMode_t SPI_DeviceMode;
	SPI_BusConfig_t SPI_BusConfig;
	SPI_SclkSpeed_t SPI_SclkSpeed;
	SPI_DFF_t SPI_DFF;
	SPI_CPOL_t SPI_CPOL;
	SPI_CPHA_t SPI_CPHA;
	SPI_SSM_t SPI_SSM;
}SPI_Config_t;

typedef struct{
	uint16_t SPI_CR1;
	uint16_t SPI_CR2;
	uint16_t SPI_SR;
	uint16_t SPI_DR;
	uint16_t SPI_CRCPR;
	uint16_t SPI_RXCRCR;
	uint16_t SPI_TXCRCR;
	uint16_t SPI_I2SCFGR;
	uint16_t SPI_I2SPR;
}SPI_RegDef_t;

#define SPI1	((SPI_RegDef_t*)SPI1_BASE)
#define SPI2	((SPI_RegDef_t*)SPI2_BASE)
#define RCC     ((RCC_RegDef_t*)RCC_BASE)

/*
 * Peripheral Clock setup
 */

void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

/*
 * Init and De - init
 */

void SPI_Init(SPI_RegDef_t *pSPIx, SPI_Config_t *pSPIConfig);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

/*
 * Data Send and Receive
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t * pTxBuffer, uint32_t len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t * pRxBuffer, uint32_t len);

/*
 * IRQ Configuaration and ISR handling
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void SPI_IRQHandling(SPI_RegDef_t *pSPIx, SPI_Config_t *pSPIConfig);



#endif /* INC_MYSPI_H_ */
