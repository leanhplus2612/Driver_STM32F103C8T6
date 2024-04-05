/*
 * middleware.c
 *
 *  Created on: Apr 5, 2024
 *      Author: My Laptop
 */

#include "stdint.h"
#include "myGPIO.h"

void PINinit(){
	PortConfig btn = {
			.port = PORT_A,
			.setupPin = ANALOG_OR_GP_PULL,
			.mode = OUTPUT_50HZ,
			.IRQ = TRUE,
	};
	PORT_Pin_init(GPIOA, &btn, 8);
	enableIRQ(GPIOA,8);

}
