/*
 * ESP8266_binding.h
 *
 *  Created on: Nov 15, 2017
 *      Author: jubeor
 */

/* Interface for ESP8266 TCPIP and WIFI system*/

#ifndef __ESP8266_BINDING_
#define __ESP8266_BINDING_

#include "string.h"

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#define IP4_LENGHT 15
#define MAC_LENGHT 18
#define SSID_MAX_LENGHT 64

typedef struct {
	/* UART configuration*/

	USART_TypeDef USART_Instance;
	UART_HandleTypeDef huart;
	GPIO_TypeDef USART_PORT;
	uint16_t USART_PIN_TX;
	uint16_t USART_PIN_RX;

	/* WIFI configuration*/
	char * SSID[SSID_MAX_LENGHT];
	char * ssid_Password[SSID_MAX_LENGHT];

	/* NETWORK configuration */
	char * IP_local[IP4_LENGHT];
	char * gateway_local[IP4_LENGHT];
	char * netmask_local[IP4_LENGHT];
	char * MAC[MAC_LENGHT];
	// static/dynamic IP

} ESP8266_BindingTypeDef;



HAL_StatusTypeDef ESP8266_Init(ESP8266_BindingTypeDef * hesp8266);

void ESP8266_MspInit(ESP8266_BindingTypeDef * hesp8266);

HAL_StatusTypeDef ESP8266_DeInit(ESP8266_BindingTypeDef * hesp8266);

void ESP8266_MspDeInit(ESP8266_BindingTypeDef * hesp8266);

HAL_StatusTypeDef ESP8266_Connect(ESP8266_BindingTypeDef * hesp8266);

HAL_StatusTypeDef ESP8266_Disconnect(ESP8266_BindingTypeDef * hesp8266);

HAL_StatusTypeDef ESP8266_TCP_Open(ESP8266_BindingTypeDef * hesp8266, char * IP, uint16_t remotePort);

HAL_StatusTypeDef ESP8266_TCP_Close(ESP8266_BindingTypeDef * hesp8266, char * IP, uint16_t remotePort);


HAL_StatusTypeDef ESP8266_Send(ESP8266_BindingTypeDef * hesp8266, char * tx_string);

HAL_StatusTypeDef ESP8266_Receive(ESP8266_BindingTypeDef * hesp8266, char * rx_str);

HAL_StatusTypeDef ESP8266_SendReceive(ESP8266_BindingTypeDef * hesp8266, char * tx_str, char * rx_str);

#endif  /* __ESP8266_BINDING_ */
