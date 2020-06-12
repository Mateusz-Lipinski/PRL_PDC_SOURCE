/* USER CODE BEGIN Header */
/**
	******************************************************************************
	* @file           : main.h
	* @brief          : Header for main.c file.
	*                   This file contains the common defines of the application.
	******************************************************************************
	* @attention
	*
	* <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
	* All rights reserved.</center></h2>
	*
	* This software component is licensed by ST under BSD 3-Clause license,
	* the "License"; You may not use this file except in compliance with the
	* License. You may obtain a copy of the License at:
	*                        opensource.org/licenses/BSD-3-Clause
	*
	******************************************************************************
	*/
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"
#include "stm32g0xx_ll_system.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "usart.h"
	/* USER CODE END Includes */

	/* Exported types ------------------------------------------------------------*/
	/* USER CODE BEGIN ET */

	/* USER CODE END ET */

	/* Exported constants --------------------------------------------------------*/
	/* USER CODE BEGIN EC */

	/* USER CODE END EC */

	/* Exported macro ------------------------------------------------------------*/
	/* USER CODE BEGIN EM */

#define SERIAL_WRITE(f_, ...)                                     \
	{                                                             \
		uint8_t ___buff[100] = {0};                               \
		int ___len = sprintf((char *)___buff, f_, ##__VA_ARGS__); \
		HAL_UART_Transmit(&huart2, ___buff, ___len, 1000);        \
	}

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)       \
	(byte & 0x80 ? '1' : '0'),     \
		(byte & 0x40 ? '1' : '0'), \
		(byte & 0x20 ? '1' : '0'), \
		(byte & 0x10 ? '1' : '0'), \
		(byte & 0x08 ? '1' : '0'), \
		(byte & 0x04 ? '1' : '0'), \
		(byte & 0x02 ? '1' : '0'), \
		(byte & 0x01 ? '1' : '0')

#define UART_RX_BUFFER_LENGTH 64
#define LPTIM_PRESCALER_1s_LSE 255
#define LPTIM_PRESCALER_1s_LSI 250

	/* USER CODE END EM */

	/* Exported functions prototypes ---------------------------------------------*/
	void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_PHASE_Pin GPIO_PIN_0
#define LED_PHASE_GPIO_Port GPIOA
#define LED_TEMP_Pin GPIO_PIN_1
#define LED_TEMP_GPIO_Port GPIOA
#define UART_TX_Pin GPIO_PIN_2
#define UART_TX_GPIO_Port GPIOA
#define UART_RX_Pin GPIO_PIN_3
#define UART_RX_GPIO_Port GPIOA
#define ADC_nError_Pin GPIO_PIN_6
#define ADC_nError_GPIO_Port GPIOA
#define ADC_nSYNC_Pin GPIO_PIN_7
#define ADC_nSYNC_GPIO_Port GPIOA
#define DAC_nLOAD_Pin GPIO_PIN_0
#define DAC_nLOAD_GPIO_Port GPIOB
#define DAC_nRESET_Pin GPIO_PIN_6
#define DAC_nRESET_GPIO_Port GPIOC
#define DAC_nSYNC_Pin GPIO_PIN_15
#define DAC_nSYNC_GPIO_Port GPIOA
#define DAC_SCK_Pin GPIO_PIN_3
#define DAC_SCK_GPIO_Port GPIOB
#define DAC_MISO_Pin GPIO_PIN_4
#define DAC_MISO_GPIO_Port GPIOB
#define DAC_MOSI_Pin GPIO_PIN_5
#define DAC_MOSI_GPIO_Port GPIOB
#define ADC_MISO_Pin GPIO_PIN_6
#define ADC_MISO_GPIO_Port GPIOB
#define ADC_MOSI_Pin GPIO_PIN_7
#define ADC_MOSI_GPIO_Port GPIOB
#define ADC_SCK_Pin GPIO_PIN_8
#define ADC_SCK_GPIO_Port GPIOB
	/* USER CODE BEGIN Private defines */

	/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
