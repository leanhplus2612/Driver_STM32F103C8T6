/*
 * mySPI.c
 *
 *  Created on: Aug 3, 2024
 *      Author: LeAnh
 */

#include "mySPI.h"


void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		if(pSPIx == SPI1){
			RCC->APB2ENR &= ~(1U<<12);
			RCC->APB2ENR |= (1U<<12);
		}
		else if(pSPIx == SPI2){
			RCC->APB1ENR &= ~(1U<<14);
			RCC->APB1ENR |= (1U<<14);
		}
		else{
			while(1);
		}
	}
	else{

	}
}

void SPI_Init(SPI_RegDef_t *pSPIx, SPI_Config_t *pSPIConfig){
	/*mode device*/
	pSPIx->SPI_CR1 &= ~(1U<<2);
	pSPIx->SPI_CR1 |= (pSPIConfig->SPI_DeviceMode << 2);

	/*bus configuration*/
	pSPIx->SPI_CR1 &= ~(1U<<15);
	pSPIx->SPI_CR1 &= ~(1U<<14);
	pSPIx->SPI_CR1 &= ~(1U<<10);
	switch(pSPIConfig->SPI_BusConfig){
	case SPI_BUS_FULL_DUPLEX:
		break;
	case SPI_BUS_HALF_DUPLEX_TRANSMIT:
		pSPIx->SPI_CR1 |= (1U << 15);
		pSPIx->SPI_CR1 |= (1U << 14);
		break;
	case SPI_BUS_HALF_DUPLEX_RECEIVE:
		pSPIx->SPI_CR1 |= (1U << 15);
		break;
	case SPI_BUS_SIMPLEX_RX_ONLY:
		pSPIx->SPI_CR1 |= (1U<<10);
		break;
	}

	/*DFF config*/
	pSPIx->SPI_CR1 &= ~(1U << 11);
	pSPIx->SPI_CR1 |=  (pSPIConfig->SPI_DFF << 11);

	/* CPOL */
	pSPIx->SPI_CR1 &= ~(1U << 1);
	pSPIx->SPI_CR1 |=  (pSPIConfig->SPI_CPOL << 1);
	/* CPHA */
	pSPIx->SPI_CR1 &= ~(1U << 0);
	pSPIx->SPI_CR1 |=  (pSPIConfig->SPI_CPHA << 1);
	/* SSM */
	pSPIx->SPI_CR1 &= ~(1U << 9);
	pSPIx->SPI_CR1 &= ~(1U<<8);

	pSPIx->SPI_CR1 |= (pSPIConfig->SPI_SSM << 9);

}

void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t * pTxBuffer, uint32_t len){
	while(len>0){
		// wait for TXE is empty
		while(!(pSPIx->SPI_SR & (1<<1)));
		// check DFF bit
		if(pSPIx->SPI_CR1 & (1U << 10) == 1){
			// 16 bit DFF
			pSPIx->SPI_DR = *((uint16_t *)pTxBuffer);
			len--;
		}
		else{
			pSPIx->SPI_DR = *pTxBuffer;
		}
		pTxBuffer++;
		len--;
	}
}

void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t * pRxBuffer, uint32_t len){
	while(len>0){
		// RXNE is empty
		while(!(pSPIx->SPI_SR & (1<<0)));
		if(pSPIx->SPI_CR1 & (1U << 10) == 1){
			// 16 bit DFF
			*((uint16_t *)pRxBuffer) = pSPIx->SPI_DR;
			len--;
		}
		else{
			// 8 Bit DFF
			*pRxBuffer = (uint8_t)pSPIx->SPI_DR;
		}
		len--;
		pRxBuffer++;

	}
}

