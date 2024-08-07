/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx.h"

typedef enum{
	Data8bit = 0U,
	Data9bit = 1U,

}DataLength_Type;

typedef struct{
	DataLength_Type datalen;


}UART_Type;
void SystemClock_Config(void);


void UART_init(){
	/*High Speed External Enable*/
	RCC->CR &= ~(RCC_CR_HSEON_Msk);
	RCC->CR |= (RCC_CR_HSEON); // HSE - 8MHz

	/*enable clock for USART */
	RCC->APB2ENR &= ~(RCC_APB2ENR_USART1EN_Msk);
	RCC->APB2ENR |= (RCC_APB2ENR_USART1EN);
	/*enable GPIO A*/
	RCC->APB2ENR &= ~(RCC_APB2ENR_IOPAEN_Msk);
	RCC->APB2ENR |= (RCC_APB2ENR_IOPAEN);

	/*
	 * PA9 - TX - ALT Function Push pull
	 * PA10 - RX - Input pull up
	 * */
	GPIOA->CRH &= ~(GPIO_CRH_MODE9_Msk);
	GPIOA->CRH &= ~(GPIO_CRH_CNF9_Msk);
	GPIOA->CRH &= ~(GPIO_CRH_MODE10_Msk);
	GPIOA->CRH &= ~(GPIO_CRH_CNF10_Msk);

	GPIOA->CRH |= (GPIO_CRH_MODE9_1);
	GPIOA->CRH |= (GPIO_CRH_CNF9_1);
	GPIOA->CRH |= (GPIO_CRH_CNF10_1);

	GPIOA->ODR &= ~(GPIO_ODR_ODR10_Msk);
	GPIOA->ODR |= (GPIO_ODR_ODR10);

	// alternate
	AFIO->MAPR &= ~(AFIO_MAPR_USART1_REMAP_Msk);

	USART1->CR1 |= (USART_CR1_UE);
	/*Enable transmit - receive*/

	/*length*/
	USART1->CR1 &= ~(USART_CR1_M_Msk); // 8 bit
	//USART1->CR1 |= (0U << USART_CR1_M_Pos);
	/*Parity bit*/
	USART1->CR1 &= ~(USART_CR1_PCE_Msk); // no parity bit
	/*Stop bit*/
	USART1->CR2 &= ~(USART_CR2_STOP_Msk); // 1 stop bit
	/*baud rate*/
	/*baud = fck/(16*USARTDIV)
	 * --> USARTDIV = fck/(16*baud)
	 * */
	USART1->BRR = 0;
	USART1->BRR |= (52 << 4);
	USART1->BRR |= 1;


}

void UART_BlockingSending(){

}

void uart_sendChar(char a){
	// enable transmit
	USART1->CR1 |= (USART_CR1_TE);
	while((USART1->SR & (1U<<7)) == 0);
	USART1->DR = a;
	while((USART1->SR & (1U<<6)) == 0);
	USART1->CR1 &= ~(USART_CR1_TE_Msk);
}
void uart_sendString(uint8_t * data, uint32_t len){
	/*enable transmitter*/
	uint32_t idx;
	USART1->CR1 |= (USART_CR1_TE);
	for(idx = 0; idx<len; idx++){
		while((USART1->SR & (1U<<7)) == 0);
		USART1->DR = (uint8_t)(*data);
		data++;
	}
	while((USART1->SR & (1U<<6)) == 0);
	USART1->CR1 &= ~(USART_CR1_TE_Msk);
}

void uart_receiveChar(){

}
void uart_receiveString(){

}
void delay(){
	uint32_t star = 1000000;
	while(star--);
}
int main(void)
{
	uart_init();


  while (1)
  {
	  uart_sendChar('A');
	  delay();
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
