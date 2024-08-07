/*
 * myGPIO.c
 *
 *  Created on: Apr 3, 2024
 *      Author: Le Tuan Anh
 */


#include "stm32f1xx.h"
#include "stdint.h"
#include "myGPIO.h"



void PORT_Pin_init(GPIO_TypeDef * gpio, PortConfig * portcfg, uint8_t pin){
	if(pin > 15){
		while(1);
	}
	/*enable clock
	 * Internal Clock is chosen for this driver
	 */
	RCC->CR |= (1<<0);

	// enable clock for GPIO
	RCC->APB2ENR |= (1U<<(portcfg->port));

	// configuration for pin

	/*config mode*/
	if(pin<8){
		gpio->CRL |= (portcfg->mode << pin*4);
		gpio->CRL |= (portcfg->setupPin << ((pin*4)+2));
	}
	else{
		gpio->CRH |= (portcfg->mode) << (pin-8)*4;
		gpio->CRH |= (portcfg->setupPin << (((pin-8)*4)+2));
	}

	/*checking for interrupt*/
	if(portcfg->mode == INPUT){
		RCC->APB2RSTR |= RCC_APB2RSTR_AFIORST;
		if(portcfg->IRQ == ENABLE){
			RCC->APB2ENR  |= RCC_APB2ENR_AFIOEN;

		}
	}

}

void enableIRQ(PORT_Type port, uint8_t pin){
	// pin error
	if(pin>15){
		while(1);
	}
	// config int
	AFIO->EVCR &= ~AFIO_EVCR_PORT_Msk;
	switch(port){
	case PORT_A:
		AFIO->EVCR |= AFIO_EVCR_PORT_PA;
		break;
	case PORT_B:
		AFIO->EVCR |= AFIO_EVCR_PORT_PB;
		break;
	case PORT_C:
		AFIO->EVCR |= AFIO_EVCR_PORT_PC;
		break;
	case PORT_D:
		AFIO->EVCR |= AFIO_EVCR_PORT_PD;
		break;
	case PORT_E:
		AFIO->EVCR |= AFIO_EVCR_PORT_PE;
		break;
	}
	AFIO->EVCR |= (pin<<0);
	if(pin>4 && pin < 10){
		NVIC_EnableIRQ(EXTI9_5_IRQn);
	}
	else if(pin>10){
		NVIC_EnableIRQ(EXTI15_10_IRQn);
	}
	else{
		switch(pin){
			case 0:
				NVIC_EnableIRQ(EXTI0_IRQn);
				break;
			case 1:
				NVIC_EnableIRQ(EXTI1_IRQn);
				break;
			case 2:
				NVIC_EnableIRQ(EXTI2_IRQn);
				break;
			case 3:
				NVIC_EnableIRQ(EXTI3_IRQn);
				break;
			case 4:
				NVIC_EnableIRQ(EXTI4_IRQn);
				break;
		}
	}




}


