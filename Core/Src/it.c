/*
 * it.c
 *
 *  Created on: Jul 12, 2024
 *      Author: daniel
 */

#include "stm32f7xx_hal.h"
#include "timerIC.h"

extern TIM_HandleTypeDef htim10;

void SysTick_Handler(void) {
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void TIM1_UP_TIM10_IRQHandler(void) {
	HAL_TIM_IRQHandler(&htim10);
}


void EXTI1_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}
