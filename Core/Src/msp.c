/*
 * msp.c
 *
 *  Created on: Jul 12, 2024
 *      Author: daniel
 */

#include "stm32f7xx_hal.h"
#include "timerIC.h"

void HAL_MspInit(void)
{
	// Perform the low level processor specific inits here using processor specific API's provided by the Cube HAL layer in Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c
	// 1. Set up the priority grouping of the arm cortex mx processor
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	// 2. Enable the required system exceptions of the arm cortex mx processor
	SCB->SHCSR |= (0x7 << 16);

	// 3. Configure the prority for the system
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	// Start GPIOC Clock
	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitTypeDef gpio_uart_init;
	gpio_uart_init.Pin = GPIO_PIN_6;
	gpio_uart_init.Mode = GPIO_MODE_AF_PP;
	gpio_uart_init.Alternate = GPIO_AF8_USART6;
	HAL_GPIO_Init(GPIOC, &gpio_uart_init);
}


void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
	/***********************************************************************
	 ***************          Configure PF6 for AF3          ***************
	 ***********************************************************************/

	/*
	 * Start GPIOF Clock
	 */
	__HAL_RCC_GPIOF_CLK_ENABLE();

	/*
	 * Initialize and populate member of InitTypeDef structure for GPIO Peripheral
	 */
	GPIO_InitTypeDef GPIO_Init;
	memset(&GPIO_Init, 0, sizeof(GPIO_Init));
	GPIO_Init.Pin = GPIO_PIN_6;
	GPIO_Init.Mode = GPIO_MODE_AF_PP;
	GPIO_Init.Alternate = GPIO_AF3_TIM10;

	/*
	 * Inititialize GPIOF Peripheral
	 */
	HAL_GPIO_Init(GPIOF, &GPIO_Init);

	/***********************************************************************
	 ***************          Configure PF6 for AF3          ***************
	 ***********************************************************************/
	HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);

}
