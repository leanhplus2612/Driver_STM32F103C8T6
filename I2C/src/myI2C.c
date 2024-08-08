/*
 * myI2C.c
 *
 *  Created on: Aug 7, 2024
 *      Author: Le Anh
 */

/*
 *		LIBRARY
 */
#include "myI2C.h"
#include "stdio.h"
#include "string.h"


static uint16_t AHB_Prescaler[8] = {2,4,8,16,64,128,256};
static uint8_t APB1_Prescaler[4] = {2,4,8,16};


/*****************************************************************************************/
/*********************************************************************
 * @fn                - I2C_PeriClockControl
 *
 * @brief             - Initialize I2C Driver
 *
 * @param[in]         - pI2Cx    - I2C Hardware Instance
 * @param[in]         - EnorDi   - Enable and Disable
 *
 * @return            -
 *
 * @Note              -
 */
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi){
	if(EnorDi == ENABLE){
		if(pI2Cx == I2C1){
			I2C1_PCLK_EN();
		}
		else if(pI2Cx == I2C2){
			I2C2_PCLK_EN();
		}
		else{

		}
	}
}

/*********************************************************************
 * @fn      		  - I2C_PeripheralControl
 *
 * @brief             -
 *
 * @param[in]         - pI2Cx    - I2C Hardware Instance
 * @param[in]         - EnorDi   - Enable and Disable
 *
 * @return            -
 *
 * @Note              -

 */
void I2C_PeripheralControl(I2C_Regdef_t *pI2Cx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pI2Cx->I2C_CR1 &= ~(1U << I2C_CR1_PE);
		pI2Cx->I2C_CR1 |= (1 << I2C_CR1_PE);
		//pI2cBaseAddress->CR1 |= I2C_CR1_PE_Bit_Mask;
	}else
	{
		pI2Cx->I2C_CR1 &= ~(1U << I2C_CR1_PE);
	}

}


/*********************************************************************
 * @fn      		  - I2C_Init
 *
 * @brief             -
 *
 * @param[in]         - pI2Cx    	- I2C Hardware Instance
 * @param[in]         - pI2CConfig 	- I2C user configurable
 *
 * @return            -
 *
 * @Note              -
 */
void I2C_Init(I2C_Regdef_t *pI2Cx, I2C_Config_t *pI2CConfig){
	uint32_t tempFREQ = 0;
	/* Enable Clock for I2C peripheral*/
	I2C_PeriClockControl(pI2Cx, ENABLE);

	/*ACK Control Enable*/
	pI2Cx->I2C_CR1 &= ~(1U<<I2C_CR1_ACK);
	if(pI2CConfig->I2C_ACKControl == I2C_ACK_ENABLE){
		pI2Cx->I2C_CR1 |= (1U<<I2C_CR1_ACK);
	}

	//configure the FREQ field of CR2
	tempFREQ = RCC_GetPCLK1Value()/1000000U;
	pI2Cx->I2C_CR2 &= ~(0x3F<<I2C_CR2_FREQ);
	pI2Cx->I2C_CR2 |= ((tempFREQ&0x3F)<<I2C_CR2_FREQ);

	// device own address
	pI2Cx->I2C_OAR1 &= ~(0x7F << I2C_CR2_ADD);
	pI2Cx->I2C_OAR1 |= (pI2CConfig->I2C_DeviceAddress << I2C_CR2_ADD);
	pI2Cx->I2C_OAR1 |= (1U << 14);

	// CCR calculation

	uint16_t ccr_value = 0;
	pI2Cx->I2C_CCR &= ~(0x0FFF << I2C_CCR_CCR);
	pI2Cx->I2C_CCR &= ~(1U << I2C_CCR_FS);
	pI2Cx->I2C_CCR &= ~(1U << I2C_CCR_DUTY);
	if(pI2CConfig->I2C_SCLSpeed <= I2C_SCL_SPEED_SM){
		// standard mode
		ccr_value = (RCC_GetPCLK1Value() / ( 2 * pI2CConfig->I2C_SCLSpeed));
	}
	else{
		// fast mode
		pI2Cx->I2C_CCR |= (1U << I2C_CCR_FS);
		if(pI2CConfig->I2C_FMDutyCycle == I2C_FM_DUTY_16_9){
			pI2Cx->I2C_CCR |= (1U << I2C_CCR_DUTY);
			ccr_value = (RCC_GetPCLK1Value() / (25 * pI2CConfig->I2C_SCLSpeed));
		}
		else{
			ccr_value = (RCC_GetPCLK1Value() / (3 * pI2CConfig->I2C_SCLSpeed));
		}
	}

	pI2Cx->I2C_CCR |= ((ccr_value&0xFFF) << I2C_CCR_CCR);


	//TRISE Configuration
}

static uint32_t RCC_GetPLLOutputClock(){

}



/*********************************************************************
 * @fn      		  - RCC_GetPCLK1Value()
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              - this function just get 8MHz from HSI and HSE
 */
uint32_t RCC_GetPCLK1Value(){
	uint32_t pclk1, SystemClk;
	uint8_t clksrc = (RCC->CFGR >> 2) & (0x3U);
	uint16_t ahbp = 0;
	uint8_t apb1 = 0;
	switch(clksrc){
	case 0:
		SystemClk = 8000000;
		break;
	case 1:
		SystemClk = 8000000;
		break;
	case 2:
		SystemClk = RCC_GetPLLOutputClock();
		break;
	default:
		break;
	}
	uint8_t temp = (RCC->CFGR >> 4) & 0xFU;
	if(temp>=8){
		ahbp = AHB_Prescaler[temp - 8];
	}
	else{
		ahbp = 1;
	}
	uint8_t temp = (RCC->CFGR >> 8) & 0x7U;
	if(temp>=4){
		apb1 = APB1_Prescaler[4-temp];
	}
	else{
		apb1 = 1;
	}

	pclk1 = (SystemClk / ahbp ) / apb1;
	return pclk1;

}



