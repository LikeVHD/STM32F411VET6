/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/
#define SECTOR1			0x08004000

#define	FLASH_CR 		(*((uint32_t*)0x40023c10))	// Flash control register
#define	FLASH_SR 		(*((uint32_t*)0x40023c0c))	// Flash control register
#define	FLASH_KEYR 		(*((uint32_t*)0x40023c04))	// Flash control register

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void Custom_Flash_Erase_Sector(uint8_t);
void Custom_Flash_Program(uint32_t, uint8_t);
/* Private user code ---------------------------------------------------------*/

void Custom_Flash_Erase_Sector(uint8_t u8SectorNumber)
{
	if ((FLASH_CR >> 31) & 1)		// Kiem tra bit LOCK
	{
		// Unlock Flash
		FLASH_KEYR = 0x45670123;
		FLASH_KEYR = 0xCDEF89AB;
	}
	// 1.
	while ((FLASH_SR >> 16) & 1);		// Wait until BSY is clean
	// 2.
	FLASH_CR |= (1U << 1);				// Sector erase activated
	FLASH_CR |= (u8SectorNumber << 3);	// "u8SectorNumber" Erase
	// 3.
	FLASH_CR |= (1U << 16);				// Start erase bit
	// 4.
	while ((FLASH_SR >> 16) & 1);		// Wait until BSY is clean
	FLASH_CR &= ~(1U << 1);				// Sector erase deactivated
}

void Custom_Flash_Program(uint32_t u32Addr, uint8_t u8Data)
{
	if ((FLASH_CR >> 31) & 1)		// Kiem tra bit LOCK
	{
		// Unlock Flash
		FLASH_KEYR = 0x45670123;
		FLASH_KEYR = 0xCDEF89AB;
	}
	// 1.
	while ((FLASH_SR >> 16) & 1);		// Wait until BSY is clean
	// 2.
	FLASH_CR |= (1U << 0);				// Flash programming activated
	// 3.
	uint8_t *pu8Temp = (uint8_t*)u32Addr;
	*pu8Temp = u8Data;
	// 4.
	while ((FLASH_SR >> 16) & 1);		// Wait until BSY is clean
	// 2.
	FLASH_CR &= ~(1U << 0);				// Flash programming activated
}
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();
  /* Initialize all configured peripherals */
  MX_GPIO_Init();

  Custom_Flash_Erase_Sector(1);
  Custom_Flash_Program(SECTOR1, '9');

  while (1)
  {

  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

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
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
