/*
 * timerIC.c
 *
 *  Created on: Jul 12, 2024
 *      Author: daniel
 */

#include "timerIC.h"

extern TIM_HandleTypeDef htim10;
extern TIM_Base_InitTypeDef Init;
extern UART_HandleTypeDef huart6;
extern TIM_IC_InitTypeDef sConfig_nonConst;
extern char msg[100];
extern uint32_t input_captures[2];
extern uint8_t count;
extern uint8_t is_capture_done;

void SystemClock_Config(void) {
	RCC_OscInitTypeDef  oscInit;
	RCC_ClkInitTypeDef  clkInit;

	oscInit.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
	oscInit.HSEState = RCC_HSE_BYPASS;
	oscInit.LSEState = RCC_LSE_ON;
	oscInit.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	oscInit.PLL.PLLM = 19;
	oscInit.PLL.PLLN = 152;
	oscInit.PLL.PLLP = RCC_PLLP_DIV2;
	oscInit.PLL.PLLState = RCC_PLL_ON;
	if(HAL_RCC_OscConfig(&oscInit) != HAL_OK) {
		Error_Handler();
	}

	clkInit.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	clkInit.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	clkInit.AHBCLKDivider = RCC_SYSCLK_DIV2;
	clkInit.APB1CLKDivider = RCC_HCLK_DIV1;
	clkInit.APB2CLKDivider = RCC_HCLK_DIV1;
	if( HAL_RCC_ClockConfig(&clkInit, FLASH_ACR_LATENCY_1WS) != HAL_OK) {
		Error_Handler();
	}

	__HAL_RCC_HSI_DISABLE(); // Turn off the HSI to save power now

	// Reconfigure Systick now to work withe the new System Clock Frequency
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

}

void Timer_Config(TIM_TypeDef *Instance, TIM_IC_InitTypeDef sConfig_nonConst, uint32_t Channel) {
	// Start Timer Peripheral Clock
	__HAL_RCC_TIM10_CLK_ENABLE();

	// Initialize Input Capture Timer
	htim10.Instance = Instance;
	htim10.Init = Init;
	if(HAL_TIM_IC_Init(&htim10) != HAL_OK) {
		Error_Handler();
	};

	const TIM_IC_InitTypeDef sConfig = sConfig_nonConst;
  // Configure the Channel for the Input Capture Timer
	if(HAL_TIM_IC_ConfigChannel(&htim10, &sConfig, Channel) != HAL_OK) {
		Error_Handler();
	};
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

	if(count == 1) {
		input_captures[0] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
		count++;
	}
	else {
		input_captures[1] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
		count = 1;
		is_capture_done = TRUE;
	}
}

void LSE_Init(void){
#if 0
	RCC_OscInitTypeDef  *RCC_OscInitStruct;
	memset(RCC_OscInitStruct, 0, sizeof(&RCC_OscInitStruct));
	RCC_OscInitStruct->OscillatorType = RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct->LSEState = RCC_LSE_ON;

	if(HAL_RCC_OscConfig(RCC_OscInitStruct) != HAL_ERROR) {
		Error_Handler();
	}
#endif

	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_LSE, RCC_MCODIV_1);
}

void UART6_Init() {
	// Start USART6 Clock
	__HAL_RCC_USART6_CLK_ENABLE();

	// Initialize USART6 Handle
	huart6.Instance = USART6;
	huart6.Init.BaudRate = 115200;
	huart6.Init.WordLength = UART_WORDLENGTH_8B;
	huart6.Init.StopBits = UART_STOPBITS_1;
	huart6.Init.Mode = UART_MODE_TX;
	huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	if(HAL_UART_Init(&huart6) != HAL_OK) {
		Error_Handler();
	}
}

void BTN_GPIO_Init(void) {
	__HAL_RCC_GPIOJ_CLK_ENABLE();
	GPIO_InitTypeDef gpioJ_Init;
	memset(&gpioJ_Init, 0, sizeof(gpioJ_Init));
	gpioJ_Init.Pin = GPIO_PIN_1;
	gpioJ_Init.Mode = GPIO_MODE_INPUT;
	gpioJ_Init.Pull = GPIO_PULLUP;
	gpioJ_Init.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOJ, &gpioJ_Init);

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	  memset(msg, 0, strlen(msg));
	  sprintf(msg, "0\n");
	  if(HAL_UART_Transmit(&huart6, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY) != HAL_OK) {
		Error_Handler();
	  }
}


void Error_Handler(void) {
	while(1);
}
