/*
 * timerIC.h
 *
 *  Created on: Jul 12, 2024
 *      Author: daniel
 */

#ifndef INC_TIMERIC_H_
#define INC_TIMERIC_H_

#include <stdio.h>
#include <string.h>
#include "stm32f7xx.h"

#define TRUE	1
#define FALSE	0

void SystemClock_Config(void);
void Timer_Config(TIM_TypeDef *Instance, TIM_IC_InitTypeDef sConfig_nonConst, uint32_t Channel);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void LSE_Init(void);
void UART6_Init(void);
void BTN_GPIO_Init(void);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void Error_Handler(void);

#endif /* INC_TIMERIC_H_ */
