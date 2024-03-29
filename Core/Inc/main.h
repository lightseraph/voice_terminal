/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define IR_Pin GPIO_PIN_3
#define IR_GPIO_Port GPIOA
#define LED_RED_Pin GPIO_PIN_5
#define LED_RED_GPIO_Port GPIOA
#define LED_GREEN_Pin GPIO_PIN_6
#define LED_GREEN_GPIO_Port GPIOA
#define SW_MInus_Pin GPIO_PIN_0
#define SW_MInus_GPIO_Port GPIOB
#define SW_MInus_EXTI_IRQn EXTI0_1_IRQn
#define SW_Plus_Pin GPIO_PIN_1
#define SW_Plus_GPIO_Port GPIOB
#define SW_Plus_EXTI_IRQn EXTI0_1_IRQn
#define CE_Pin GPIO_PIN_12
#define CE_GPIO_Port GPIOA
#define I2C_SCK_Pin GPIO_PIN_6
#define I2C_SCK_GPIO_Port GPIOB
#define I2C_SDA_Pin GPIO_PIN_7
#define I2C_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
  // 定义一些常用的数据类型短关键字

  typedef const int32_t sc32;
  typedef const int16_t sc16;
  typedef const int8_t sc8;

  typedef __IO int32_t vs32;
  typedef __IO int16_t vs16;
  typedef __IO int8_t vs8;

  typedef __I int32_t vsc32;
  typedef __I int16_t vsc16;
  typedef __I int8_t vsc8;

  typedef uint32_t u32;
  typedef uint16_t u16;
  typedef uint8_t u8;

  typedef const uint32_t uc32;
  typedef const uint16_t uc16;
  typedef const uint8_t uc8;

  typedef __IO uint32_t vu32;
  typedef __IO uint16_t vu16;
  typedef __IO uint8_t vu8;

  typedef __I uint32_t vuc32;
  typedef __I uint16_t vuc16;
  typedef __I uint8_t vuc8;

  extern vu32 time_delay;
  extern const uint8_t LOCAL_ID[16];
  extern uint8_t local_id;

  void Init_Param(void);
  void Disp_Freq(u8 freq_index);
  void Disp_ID(u8 id_index);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
