/* USER CODE BEGIN Header */
/**
	******************************************************************************
	* @file           : main.c
	* @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "lptim.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ADC.h"
#include "DAC.h"
#include "state.h"
#include "usart_utils.h"
#include "cmd.h"
#include "Filter.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
state State = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_LPTIM1_Init();
  MX_USART2_UART_Init();
  MX_SPI2_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  HAL_LPTIM_MspInit(&hlptim1);
  HAL_LPTIM_Counter_Start_IT(&hlptim1, LPTIM_PRESCALER_1s_LSI);
  HAL_SPI_MspInit(&hspi2);
  HAL_SPI_MspInit(&hspi1);

  SERIAL_WRITE(RESET);

  ADC_reset();
  ADC_config();
  HAL_UART_Receive_DMA(&huart2, State.UART_RX_BUFF, UART_RX_BUFFER_LENGTH);

  DAC_reset();

  //PRIORITY OVERRIDES:
  HAL_InitTick(0);
  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 1, 0);
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 2, 0);
  HAL_NVIC_SetPriority(TIM6_DAC_LPTIM1_IRQn, 2, 0);
  HAL_NVIC_SetPriority(SPI2_IRQn, 2, 0);
  HAL_NVIC_SetPriority(USART2_IRQn, 2, 0);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  DAC_cmd(RF_ATT_352_FINE + DAC_WRITE + 0xffff);
  DAC_cmd(RF_ATT_352_OFFSET + DAC_WRITE + 0xffff);
  DAC_cmd(RF_ATT_704_FINE + DAC_WRITE + 0xffff);
  DAC_cmd(RF_ATT_704_OFFSET + DAC_WRITE + 0xffff);

  ADC_update();
  PID_Init();
  Calib();

  uint32_t counter = 0;
  while (1)
  {
    if (State.cmd)
    {
      cmd();
    };
    if (State.ADC_Updated)
    {
      counter++;
      Filter();

      if (counter % (FilterLength) == 0)
      {
        if (State.ADC_Filter_Valid)
        {
          PID();
        }

        SERIAL_WRITE("\n\n")
        // SERIAL_WRITE("PD1 %i\n", State.PD1_Slope);
        // SERIAL_WRITE("PD2 %i\n", State.PD2_Slope);
        // SERIAL_WRITE("PD3 %i\n", State.PD3_Slope);
        // SERIAL_WRITE("PD4 %i\n", State.PD4_Slope);

        SERIAL_WRITE("PD1+PD2 SETPOINT: %li\n", State.PD1_PD2_SetPoint);
        SERIAL_WRITE("PID 704 Output: %li\n", State.PID_704_Output);
        SERIAL_WRITE("PID_704_P_error %i\n", State.PID_704_P_error);

        SERIAL_WRITE("PD1 Phase:\t\t");
        SERIAL_WRITE("%3i.", Phase(ADC_PD1_Phase_CH) / 1000);
        SERIAL_WRITE("%03li deg\t\t\n", Phase(ADC_PD1_Phase_CH) % 1000);

        SERIAL_WRITE("PD2 Phase:\t\t");
        SERIAL_WRITE("%3i.", Phase(ADC_PD2_Phase_CH) / 1000);
        SERIAL_WRITE("%03li deg\t\t\n\n", Phase(ADC_PD2_Phase_CH) % 1000);

        SERIAL_WRITE("PD3+PD4 SETPOINT: %li\n", State.PD3_PD4_SetPoint);
        SERIAL_WRITE("PID 352 Output: %li\n", State.PID_352_Output);
        SERIAL_WRITE("PID_352_P_error %i\n", State.PID_352_P_error);

        SERIAL_WRITE("PD3 Phase:\t\t");
        SERIAL_WRITE("%3i.", Phase(ADC_PD3_Phase_CH) / 1000);
        SERIAL_WRITE("%03li deg\t\t\n", Phase(ADC_PD3_Phase_CH) % 1000);

        SERIAL_WRITE("PD4 Phase:\t\t");
        SERIAL_WRITE("%3i.", Phase(ADC_PD4_Phase_CH) / 1000);
        SERIAL_WRITE("%03li deg\t\t\n", Phase(ADC_PD4_Phase_CH) % 1000);
      }
    };
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage 
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks 
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_LPTIM1;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  for (int i = 0; i < 10; i++)
  {
    HAL_GPIO_TogglePin(LED_PHASE_GPIO_Port, LED_PHASE_Pin);
    HAL_Delay(100);
  }
  HAL_GPIO_WritePin(LED_PHASE_GPIO_Port, LED_PHASE_Pin, 0);

  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
		 tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
