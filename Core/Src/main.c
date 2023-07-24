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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bk9535.h"
#include "syscall.h"
#include "key.h"
#include "config.h"
#include "irda.h"
#include "irtx.h"
#include "eeprom.h"
#include "oled.h"
#include "stdio.h"
#include "stdlib.h"
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
vu32 time_delay;
uint8_t local_id = 0;

const uint8_t LOCAL_ID[16] = {
    0x10,
    0x11,
    0x12,
    0x13,
    0x14,
    0x15,
    0x16,
    0x17,
    0x18,
    0x19,
    0x1A,
    0x1B,
    0x1C,
    0x1E,
    0x1F,
    0x20};
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */
  // USER_DATA.rUserFreqIndex = DEFAULT_FREQ;
  USER_DATA.UserId.dword = DEF_USER_ID;
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
  MX_USART1_UART_Init();
  MX_TIM6_Init();
  MX_TIM2_Init();
  MX_TIM21_Init();
  MX_TIM22_Init();
  /* USER CODE BEGIN 2 */

  HAL_TIM_Base_Start_IT(&htim2);
  KEY_Config();

  HAL_Delay(100);

  rWorkChannel = CHA;
  EEPROM_Read_W_CHECK(FREQ_ADDR, &USER_DATA.rUserFreqIndex, 1);
  EEPROM_Read_W_CHECK(LOCAL_ID_ADDR, &local_id, 1);
  Flash_LED(LED_RED, 50, 5, LIGHT_ON);
  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, SET);

  if (BK_Init())
    Flash_LED(LED_GREEN, 50, 5, LIGHT_ON);
  OLED_Init();
  t_PCMCfg cfg;
  cfg.bclk = PCM_SCK_I;
  cfg.dat = PCM_SDA_I;
  cfg.ch = RIGHT_CHANNEL;
  cfg.mode = PCM_SLAVE;
  cfg.lrck = PCM_LRCK_I;
  BK_Tx_I2SOpen(cfg);
  SwitchFreqByIndex(USER_DATA.rUserFreqIndex);
  OLED_ShowString(35, 23, (u8 *)"FREQ:", 12);
  OLED_ShowString(35, 45, (u8 *)"ID:", 12);
  char freq_char[9];
  float freq_num = FreqTableA[USER_DATA.rUserFreqIndex] / 10.0;
  sprintf(freq_char, "%.1f", freq_num);
  OLED_ShowString(35, 21, (u8 *)freq_char, 16);
  OLED_ShowString(77, 25, (u8 *)"MHz", 12);
  OLED_ShowString(35, 43, (u8 *)"ID:", 16);
  Disp_ID(local_id);
  //  OLED_ShowNum(50, 40, 12, 2, 16);
  //        TX_WriteID(USER_DATA.UserId.dword);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    static uint8_t interval = 0;
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    delay_nms(50);
    KEY_Scan();
    if (interval > 20)
    {
      IR_PostData(0x02);
      interval = 0;
    }
    // HAL_GPIO_WritePin(IR_GPIO_Port, IR_Pin, SET);   限流电阻15欧，红外发射工作电流130mA
    if (remote_scan() == 0x02)
      Flash_LED(LED_RED, 50, 1, FOLLOW_PREVIOUS);

    interval++;
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_8;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void Init_Param(void)
{
  uint8_t temp = 0;
  EEPROM_WRITE_W_CHECK(LOCAL_ID_ADDR, &temp, 1); // 默认出厂设置id索引号
  temp = 1;
  EEPROM_WRITE_W_CHECK(FREQ_ADDR, &temp, 1); // 默认出厂设置频点索引号
}

void Disp_Freq(u8 freq_index)
{
  char freq_char[5];
  float freq_num = FreqTableA[freq_index] / 10.0;
  sprintf(freq_char, "%.1f", freq_num);
  OLED_ShowString(35, 21, (u8 *)freq_char, 16);
}

void Disp_ID(u8 id_index)
{
  char id_char[5];
  sprintf(id_char, "%#X", LOCAL_ID[id_index]);
  OLED_ShowString(62, 43, (u8 *)id_char, 16);
}
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
