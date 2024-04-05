/*
 * myGPIO.h
 *
 *  Created on: Apr 3, 2024
 *      Author: Le Tuan Anh
 */

#ifndef MYDRIVER_MYGPIO_H_
#define MYDRIVER_MYGPIO_H_

#include <stdint.h>
#include "stm32f1xx.h"

typedef enum{
	PORT_A = 2U,
	PORT_B = 3U,
	PORT_C = 4U,
	PORT_D = 5U,
	PORT_E = 6U,
}PORT_Type;

typedef enum{
	INPUT 		= 0U,
	OUTPUT_10HZ = 1U,
	OUTPUT_2MHZ = 2U,
	OUTPUT_50HZ	= 3U,
}ModeType;

typedef enum{
	ANALOG_OR_GP_PULL			= 0U,
	FLOATING_OR_GP_OPEN_DRAIN	= 1U,
	INPUT_PULL_OR_ALT_PULL		= 2U,
	ALT_OPEN_DRAIN				= 3U,
}SETUP_Pin_TYPE;

typedef enum{
	FALSE = 0U,
	TRUE 	= 1U,
}IRQ_Enable_Type;

typedef struct{
	PORT_Type 		port;
	ModeType 		mode;
	SETUP_Pin_TYPE	setupPin;
	IRQ_Enable_Type		IRQ;
}PortConfig;

/*function prototype*/
void PORT_Pin_init(GPIO_TypeDef * gpio, PortConfig * portcfg, uint8_t pin);
void enableIRQ(PORT_Type port, uint8_t pin);
static inline void GPIO_Set(GPIO_TypeDef * gpio, uint8_t pin){
	gpio->ODR |= (1U<<pin);
}

static inline void GPIO_Clear(GPIO_TypeDef * gpio, uint8_t pin){
	gpio->BRR |= (1U<<pin);
}

static inline void GPIO_Toggle(GPIO_TypeDef * gpio, uint8_t pin){
	gpio->BSRR ^= (1U<<pin);
}


#endif /* MYDRIVER_MYGPIO_H_ */
