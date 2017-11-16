/*
 * ESP8266_binding.c
 *
 *  Created on: Nov 15, 2017
 *      Author: jubeor
 */

//The start commit for this project. It's actually a mutation of a USART
// proyect because i'm implementing here ESP8266_binding to be use later
// at a bigger project.

#include "ESP8266_binding.h"



static void ESP8266_UART_InitialInit(ESP8266_BindingTypeDef * hesp8266);

/**
 * @brief
 * @param
 *
 * @retval
 */
HAL_StatusTypeDef ESP8266_Init(ESP8266_BindingTypeDef * hesp8266)
{

	USART3_UART_InitialInit(&hesp8266);
	ESP8266_MspInit(&hesp8266);
	return(HAL_OK);
}


HAL_StatusTypeDef ESP8266_DeInit(ESP8266_BindingTypeDef * hesp8266)
{

	ESP8266_MspDeInit(&hesp8266);
	return(HAL_OK);
}


__weak void ESP8266_MspInit(ESP8266_BindingTypeDef * hesp8266)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(huart);
	/* NOTE: This function Should not be modified, when the callback is needed,
	           the HAL_UART_MspInit could be implemented in the user file
	 */
}


__weak void ESP8266_MspDeInit(ESP8266_BindingTypeDef * hesp8266)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(huart);
	/* NOTE: This function Should not be modified, when the callback is needed,
	           the HAL_UART_MspInit could be implemented in the user file
	 */
}

static void ESP8266_UART_InitialInit(ESP8266_BindingTypeDef * hesp8266)
{

#ifdef  USE_FULL_ASSERT
	assert_param(hesp8266->USART_PIN_RX!=NULL);
	assert_param(hesp8266->USART_PIN_TX!=NULL);
	assert_param(hesp8266->USART_PORT!=NULL);
	assert_param(hesp8266->USART_Instance!=NULL);
#endif


	// Instance for the USART3
	hesp8266->huart.Instance = hesp8266->USART_Instance;

	// Configure Init
	hesp8266->huart.Init.BaudRate = 115200;
	hesp8266->huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	hesp8266->huart.Init.Mode = UART_MODE_TX_RX;
	hesp8266->huart.Init.OverSampling = UART_OVERSAMPLING_16;
	hesp8266->huart.Init.Parity = UART_PARITY_NONE;
	hesp8266->huart.Init.StopBits = UART_STOPBITS_1;
	hesp8266->huart.Init.WordLength = UART_WORDLENGTH_8B;

	if(HAL_UART_Init(&hesp8266->huart)!= HAL_OK)
	{
		Error_Handler();
	}

	GPIO_InitTypeDef hgpioInitInput;

	hgpioInitInput.Pin = hesp8266->USART_PIN_TX | hesp8266->USART_PIN_RX ;

	hgpioInitInput.Mode = GPIO_MODE_AF_PP;
	hgpioInitInput.Pull = GPIO_PULLUP;
	hgpioInitInput.Speed = GPIO_SPEED_MEDIUM;


	// Enable the UART clock: Only for STM32F407vx
	switch(hesp8266->USART_Instance)
	{
	case USART2:
		__HAL_RCC_USART2_CLK_ENABLE();
		hgpioInitInput.Alternate = GPIO_AF7_USART2;
		break;

	case USART3:
		__HAL_RCC_USART3_CLK_ENABLE();
		hgpioInitInput.Alternate = GPIO_AF7_USART3;
		break;

	case UART4:
		__HAL_RCC_USART4_CLK_ENABLE();
		hgpioInitInput.Alternate = GPIO_AF8_UART4;
		break;

	case UART5:
		__HAL_RCC_USART5_CLK_ENABLE();
		hgpioInitInput.Alternate = GPIO_AF8_UART5;
		break;

	default:
		Error_handler();
	}

	// Enable the GPIO clock
	switch(hesp8266->USART_PORT)
	{
	case GPIOA:
		__HAL_RCC_GPIOA_CLK_ENABLE();
		break;
	case GPIOB:
		__HAL_RCC_GPIOB_CLK_ENABLE();
		break;
	case GPIOC:
		__HAL_RCC_GPIOC_CLK_ENABLE();
		break;
	case GPIOD:
		__HAL_RCC_GPIOD_CLK_ENABLE();
		break;
	case GPIOE:
		__HAL_RCC_GPIOE_CLK_ENABLE();
		break;
	case GPIOF:
		__HAL_RCC_GPIOF_CLK_ENABLE();
		break;
	case GPIOG:
		__HAL_RCC_GPIOG_CLK_ENABLE();
		break;
	case GPIOH:
		__HAL_RCC_GPIOH_CLK_ENABLE();
		break;
	case GPIOI:
		__HAL_RCC_GPIOI_CLK_ENABLE();
		break;
	default:
		Error_Handler();

	}

	// PORT INIT:


	HAL_GPIO_Init(hesp8266->USART_PORT,&hgpioInitInput);

}

void HAL_UART_MspInit(UART_HandleTypeDef * huart)
{


}


void HAL_UART_MspDeInit(UART_HandleTypeDef * huart)
{
	//Nothing yet

}


