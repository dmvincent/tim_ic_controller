/*
 * main.c
 *
 *  Created on: Jul 12, 2024
 *      Author: daniel
 */



#include "timerIC.h"



TIM_HandleTypeDef htim10;
TIM_Base_InitTypeDef Init;
TIM_IC_InitTypeDef sConfig_nonConst;
UART_HandleTypeDef huart6;

char msg[100];
uint32_t input_captures[2];
uint8_t count;
uint8_t is_capture_done;
uint16_t outer_count = 0;

int main() {
  // Init HAL
  HAL_Init(); 

  /***********************************************************************
   ***************             Configure Clock             ***************
   ***********************************************************************/
  SystemClock_Config();

  /***********************************************************************
   ***************             Configure UART6             ***************
   ***********************************************************************/
  memset(&huart6, 0, sizeof(huart6));
  UART6_Init();
  memset(msg, 0, strlen(msg));
  sprintf(msg, "Let's use Timer10's IC1 to clock the LSE speed\n");
  if(HAL_UART_Transmit(&huart6, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY) != HAL_OK) {
	Error_Handler();
  }

  memset(msg, 0, strlen(msg));
  sprintf(msg, "==============================================\r\n");
  if(HAL_UART_Transmit(&huart6, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY) != HAL_OK) {
	Error_Handler();
  }

  /***********************************************************************
   ***************           Configure BTN GPIO           ****************
   ***********************************************************************/
  BTN_GPIO_Init();
  while(HAL_GPIO_ReadPin(GPIOJ, GPIO_PIN_1) == GPIO_PIN_SET);

  /***********************************************************************
   ***************           Configure LSE OSCI           ****************
   ***********************************************************************/
  LSE_Init();

  /***********************************************************************
   ***************             Initialize Timer            ***************
   ***********************************************************************/

  /*
   * Clear the Handle for the timer
   */
  memset(&htim10, 0, sizeof(htim10));

  /*
   * Declare and Initialize the InitTypeDef Structure for the timer
   */

  memset(&Init, 0, sizeof(Init));
  Init.Prescaler = 1;
  Init.Period = 0xFFFFFFFF;
  Init.CounterMode = TIM_COUNTERMODE_UP;

  /*
   * Configure TIM Input Capture  structure and clear
   */

  memset(&sConfig_nonConst, 0, sizeof(sConfig_nonConst));
  sConfig_nonConst.ICPolarity = TIM_ICPOLARITY_RISING;
  sConfig_nonConst.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfig_nonConst.ICPrescaler = TIM_ICPSC_DIV1;
  sConfig_nonConst.ICFilter = 0;

  /*
   * Define which channel of the timer will be used
   */
  uint32_t Channel;
  Channel = TIM_CHANNEL_1;

  /*
   * Initialize Timer
   */
  Timer_Config(TIM10, sConfig_nonConst, Channel);

  /***********************************************************************
   ***************               Start Timer               ***************
   ***********************************************************************/
  count = 1;
  HAL_TIM_IC_Start_IT(&htim10, Channel);

  uint32_t diff = 0;
  double timer10_cnt_freq = 0;
  double timer10_cnt_res = 0;
  double user_signal_time_period = 0;
  double user_signal_freq = 0;

  while(1) {
	  if (is_capture_done != FALSE) {
	    outer_count++;
	    if(outer_count == 18500){
	      if(input_captures[1] > input_captures[0])
		      diff = input_captures[1] - input_captures[0];
	      else
		      diff = (0xFFFFFFFF - input_captures[0] + input_captures[1]);

	      timer10_cnt_freq = HAL_RCC_GetPCLK2Freq() / (htim10.Init.Prescaler + 1);
	      timer10_cnt_res = 1/timer10_cnt_freq;
	      user_signal_time_period = diff*timer10_cnt_res;
	      user_signal_freq = 1/user_signal_time_period;

	      memset(msg, 0, strlen(msg));
	      sprintf(msg, "%f\n", user_signal_freq);
	      if(HAL_UART_Transmit(&huart6, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY) != HAL_OK) {
		    Error_Handler();
	      }
	      outer_count = 0;
	    }
	    is_capture_done = FALSE;
	  }
  }

  return 0;
}




