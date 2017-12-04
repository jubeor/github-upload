/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @author  Ac6
  * @version V1.0
  * @date    02-Feb-2015
  * @brief   Default Interrupt Service Routines.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#ifdef USE_RTOS_SYSTICK
#include <cmsis_os.h>
#endif
#include "stm32f4xx_it.h"

/* extern variables ----------------------------------------------------------*/

extern UART_HandleTypeDef huart3; // Defined in main.c

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            	  	    Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles SysTick Handler, but only if no RTOS defines it.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
#ifdef USE_RTOS_SYSTICK
	osSystickHandler();

#endif
}

////HAL_UART_IRQHandler()

void USART3_IRQHandler(void)
{
	HAL_NVIC_ClearPendingIRQ(USART3_IRQn);
	HAL_UART_IRQHandler(&huart3);

//	__HAL_UART_CLEAR_FLAG(&huart3,UART_FLAG_RXNE);
//	__HAL_UART_CLEAR_FLAG(&huart3,UART_FLAG_TC);
//	__HAL_UART_CLEAR_FLAG(&huart3,UART_FLAG_CTS);
//	__HAL_UART_CLEAR_FLAG(&huart3,UART_FLAG_LBD);
//	__HAL_UART_CLEAR_OREFLAG(&huart3);
//	__HAL_UART_CLEAR_NEFLAG(&huart3);
}
