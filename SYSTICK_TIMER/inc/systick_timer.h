/*
 * systick_timer.h
 *
 *  Created on: Jun 5, 2024
 *      Author: My Laptop
 */

#ifndef MYDRIVER_SYSTICK_TIMER_H_
#define MYDRIVER_SYSTICK_TIMER_H_

#include <stdint.h>

void systick_config();
void mydelay(uint32_t ticks);

#endif /* MYDRIVER_SYSTICK_TIMER_H_ */
