/*
 * systick_timer.c
 *
 *  Created on: Jun 4, 2024
 *      Author: My Laptop
 */
#include <stdio.h>
#include <stdint.h>
#include <stm32f1xx.h>
#include <core_cm3.h>
//#include <stm32f1xx_it.h>

volatile uint32_t msTicks;


void SysTick_Handler(){
	++msTicks;

}

uint32_t getTicksCtrl(void){
	uint32_t tickCtrl;
	__disable_irq();
	tickCtrl = msTicks;
	__enable_irq();
	return tickCtrl;

}

void clock_init(){

	// 8 MHz
	RCC->CR |= (1U<<0);
	RCC->CFGR &= (3U <<24);
	RCC->CFGR |= (5U <<24);
	//8MHz/8 = 1Mhz per 1 pulse
}

void systick_config(){
	clock_init();
//	SysTick->CTRL |=
	SysTick->LOAD = 999;
	SysTick->CTRL &= (1U <<2) ; // external clock source
	SysTick->CTRL |= (1U << 1); // enable interrupt
	SysTick->CTRL |=(1U<<0);
	NVIC_EnableIRQ(SysTick_IRQn);
}

void mydelay(uint32_t ticks){
	uint32_t start = getTicksCtrl();
	while(getTicksCtrl() - start < ticks);
}
