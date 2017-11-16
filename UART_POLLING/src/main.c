/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/




/* Includes ------------------------------------------------------------------*/

#include "string.h"

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "LIS3DSH_definitions.h"


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/

#define USART_PORT GPIOB
#define USART_PIN_TX GPIO_PIN_10
#define USART_PIN_RX GPIO_PIN_11
#define USART_PIN_CX GPIO_PIN_12
#define USART_PIN_CTS GPIO_PIN_13


/* Threading definitions -----------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
//USART_HandleTypeDef husart;
UART_HandleTypeDef huart;

#define USART_BUFFER_SIZE 80
#define USART_TIMEOUT 1000

static uint8_t pDataTx[USART_BUFFER_SIZE];
static uint8_t pDataRx[USART_BUFFER_SIZE];

/* Private functions prototypes ---------------------------------------------*/

static void SystemClock_Config(void);
static void Error_Handler(void);

static void USART3_UART_InitialInit(void);


int main(void)
{

	HAL_Init();

	SystemClock_Config();

	USART3_UART_InitialInit();

	for(;;)
	{
		if(HAL_UART_Receive(&huart,&pDataTx[0],USART_BUFFER_SIZE,USART_TIMEOUT)==HAL_OK)
		{
			HAL_UART_Transmit(&huart,&pDataTx[0],USART_BUFFER_SIZE,USART_TIMEOUT);
		}
//		HAL_UART_Transmit(&huart,&pDataRx[0],USART_BUFFER_SIZE,USART_TIMEOUT);

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
		Error_Handler();
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		Error_Handler();
	}

	/* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
	if (HAL_GetREVID() == 0x1001)
	{
		/* Enable the Flash prefetch */
		__HAL_FLASH_PREFETCH_BUFFER_ENABLE();
	}
}

static void Error_Handler(void)
{
	/* Turn LED5 on */
	BSP_LED_On(LED5);
	while(1)
	{
	}
}


/* HAL weak function redefinition */

void HAL_MspInit(void)
{

}

void HAL_MspDeInit(void)
{

}

void HAL_UART_MspInit(UART_HandleTypeDef * huart)
{

	// Enable the UART3 clock
	__HAL_RCC_USART3_CLK_ENABLE();

	// Enable the GPIOA clock
	__HAL_RCC_GPIOB_CLK_ENABLE();


	// PORT B INIT:

	GPIO_InitTypeDef hgpioInitInput;

	hgpioInitInput.Pin = USART_PIN_TX | USART_PIN_RX ;

	hgpioInitInput.Mode = GPIO_MODE_AF_PP;
	hgpioInitInput.Pull = GPIO_PULLUP;
	hgpioInitInput.Speed = GPIO_SPEED_MEDIUM;
	hgpioInitInput.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(USART_PORT,&hgpioInitInput);


	// Configure NVIC

	// Configure DMA

}


void HAL_UART_MspDeInit(UART_HandleTypeDef * huart)
{
	//Nothing yet

}



/* Private functions definition ----------------------------------------------*/

static void USART3_UART_InitialInit(void)
{

//	husart.Init.BaudRate = 9600;
//	husart.Init.WordLength = USART_WORDLENGTH_8B;
//	husart.Init.StopBits = USART_STOPBITS_1;
//	husart.Init.Parity = USART_PARITY_NONE;
//	husart.Init.Mode = USART_MODE_TX_RX;
//	husart.Init.CLKPolarity = USART_POLARITY_LOW;
//	husart.Init.CLKPhase = USART_PHASE_1EDGE;
//	husart.Init.CLKLastBit = USART_LASTBIT_DISABLE;
//	HAL_USART_Init(&husart);


	// Instance for the USART3
	huart.Instance = USART3;

	// Config Init
	huart.Init.BaudRate = 9600;
	huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart.Init.Mode = UART_MODE_TX_RX;
	huart.Init.OverSampling = UART_OVERSAMPLING_16;
	huart.Init.Parity = UART_PARITY_NONE;
	huart.Init.StopBits = UART_STOPBITS_1;
	huart.Init.WordLength = UART_WORDLENGTH_8B;


	if(HAL_UART_Init(&huart)!= HAL_OK)
	{
		Error_Handler();
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
