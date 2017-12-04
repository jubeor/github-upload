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

static void Error_Handler_detailed(int line, char * file);

static void USART3_UART_InitialInit(void);

//static void serialSend_P()

static void execute_serial_command(uint8_t * command);


int main(void)
{
	HAL_Init();

	SystemClock_Config();

	USART3_UART_InitialInit();

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


void Error_Handler_detailed(int line, char * file)
{
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
	// a. Enable the UART3 clock
	__HAL_RCC_USART3_CLK_ENABLE();

	// b. Enable the GPIOA clock
	__HAL_RCC_GPIOB_CLK_ENABLE();


	// b. PORT B INIT:

	GPIO_InitTypeDef hgpioInitInput;
//	hgpioInitInput = malloc(sizeof(GPIO_InitTypeDef *));

	hgpioInitInput.Pin = USART_PIN_TX | USART_PIN_RX ;
	hgpioInitInput.Mode = GPIO_MODE_AF_PP;
	hgpioInitInput.Pull = GPIO_PULLUP;
	hgpioInitInput.Speed = GPIO_SPEED_MEDIUM;
	hgpioInitInput.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(USART_PORT,&hgpioInitInput);

//	free(hgpioInitInput);

	// c. NVIC configuration (when using HAL_UART_Transmit_IT() and HAL_UART_Receive_IT() APIs)

	//	Configure the USARTx interrupt priority.			references: p66,
	HAL_NVIC_SetPriority(USART3_IRQn,6,0);

	//	Enable the NVIC USART IRQ handle
	HAL_NVIC_EnableIRQ(USART3_IRQn);

}


void HAL_UART_MspDeInit(UART_HandleTypeDef * huart)
{
	//Nothing yet
	//free(huart);

	__HAL_RCC_USART3_CLK_DISABLE();

}


/* Private functions definition ----------------------------------------------*/

static void USART3_UART_InitialInit(void)
{

//	huart = (UART_HandleTypeDef *) malloc(sizeof(UART_HandleTypeDef *));
	// Instance for the USART3
	huart3.Instance = USART3;

	// Initial Configuration
	huart3.Init.BaudRate = 115200;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;


	if(HAL_UART_Init(&huart3)!= HAL_OK)
	{
		Error_Handler_detailed(__LINE__,__FILE__);
	}
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

//	HAL_UART_Transmit_IT(&huart,pDataTx,strlen(pDataTx));
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	__HAL_UART_FLUSH_DRREGISTER(huart); // Clear the buffer to prevent overrun

	switch (serialRx)
	{
		// Borrar linea o flecha atrás?
		case 8:
		case 127:
			if (serialRxBufferIndex>0) serialRxBufferIndex--;
			break;

		// Fin de linea
		case '\n':
		case '\r':
			if (serialRxBufferIndex==0) break;
			strcat(serialRxBuffer,"\r");
			execute_serial_command(serialRxBuffer);
			serialRxBufferIndex = 0;
			break;

		default:
			if (serialRxBufferIndex == 0)
			{
				for (int i = 0; i<USART_BUFFER_SIZE; i++) serialRxBuffer[i] = 0;
			}
			serialRxBuffer[serialRxBufferIndex] = serialRx;
			serialRxBufferIndex++;
			if (serialRxBufferIndex >=USART_BUFFER_SIZE)
			{
				serialRxBufferIndex = 0;
				for (int i = 0; i<USART_BUFFER_SIZE; i++) serialRxBuffer[i] = 0;
			}
	}

	/*	RxBufferIndex	serialRxBuffer[USART_BUFFER_SIZE]	*/
	HAL_UART_Receive_IT(huart,&serialRx,1);
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


void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	Error_Handler_detailed(__LINE__,__FILE__);
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
