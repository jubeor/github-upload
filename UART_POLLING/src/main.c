/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


/*
 * Error occur: Overrun
 * Error occur: Noise
 * */

/* Includes ------------------------------------------------------------------*/

#include "string.h"
#include "stdlib.h"

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#include "UARTBind.h"
#include "error_handling.h"
/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/

#define USART_PORT GPIOB
#define USART_PIN_TX GPIO_PIN_10
#define USART_PIN_RX GPIO_PIN_11


/* Threading definitions -----------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef huart3;

#define USART_BUFFER_SIZE 80
#define USART_TIMEOUT 1000

static uint8_t serialTxBuffer[USART_BUFFER_SIZE];
static uint8_t serialRxBuffer[USART_BUFFER_SIZE];
static uint8_t serialRxBufferIndex = 0;
static uint8_t serialRx;

#define ENVIO_INICIAL "\r\rStarting up\r"
#define ENVIO_PERIODICO "Periodical transmission\r"

/* Private functions prototypes ---------------------------------------------*/

static void SystemClock_Config(void);

static void USART3_UART_Init(void);

static void execute_serial_command(uint8_t * command);

int main(void)
{
	HAL_Init();

	SystemClock_Config();

	USART3_UART_Init();

	strcpy(serialTxBuffer,ENVIO_INICIAL);

	if(HAL_UART_Transmit_IT(&huart3,&serialTxBuffer,strlen(serialTxBuffer))!=HAL_OK)
	{
		Error_Handler_detailed(__LINE__,__FILE__);
	}


	if(HAL_UART_Receive_IT(&huart3,&serialRx,1)!=HAL_OK)
	{
		Error_Handler_detailed(__LINE__,__FILE__);
	}


	for(;;)
	{
		HAL_Delay(1000);
		strcpy(serialTxBuffer,ENVIO_PERIODICO);
		if(HAL_UART_Transmit_IT(&huart3,&serialTxBuffer,strlen(serialTxBuffer))!=HAL_OK)
		{
			Error_Handler_detailed(__LINE__,__FILE__);
		}


	}
}


static void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();

	/* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler_detailed(__LINE__,__FILE__);
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK |
			RCC_CLOCKTYPE_HCLK |
			RCC_CLOCKTYPE_PCLK1 |
			RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		Error_Handler_detailed(__LINE__,__FILE__);
	}

	/* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
	if (HAL_GetREVID() == 0x1001)
	{
		/* Enable the Flash prefetch */
		__HAL_FLASH_PREFETCH_BUFFER_ENABLE();
	}
}


static void execute_serial_command(uint8_t * command)
{
	//TODO: Crear la lista de comandos para responder a peticiones externas

	HAL_StatusTypeDef txStatus = HAL_OK;

	while (huart3.Lock==HAL_BUSY)
	{
		HAL_Delay(10);
	}

	txStatus = HAL_UART_Transmit_IT(&huart3,command,strlen((char *)command));


	while (txStatus!=HAL_OK)
	{
		if(txStatus!=HAL_ERROR || txStatus!=HAL_TIMEOUT)
		{
			Error_Handler_detailed(__LINE__,__FILE__);
		}
		if(txStatus!=HAL_BUSY)
		{
			HAL_Delay(10);

			txStatus = HAL_UART_Transmit_IT(&huart3,command,strlen((char *)command));

		}

	}

}


#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif
