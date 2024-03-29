#ifndef __IRDA_H_
#define __IRDA_H_

#include "main.h"
#include "tim.h"

#define REMOTE_INA_GPIO_PORT GPIOA
#define REMOTE_INB_GPIO_PORT GPIOA
#define REMOTE_INA_GPIO_PIN GPIO_PIN_0
#define REMOTE_INB_GPIO_PIN GPIO_PIN_1

#define IR_A_DATA HAL_GPIO_ReadPin(REMOTE_INA_GPIO_PORT, REMOTE_INA_GPIO_PIN)
#define IR_B_DATA HAL_GPIO_ReadPin(REMOTE_INB_GPIO_PORT, REMOTE_INB_GPIO_PIN)

extern uint8_t g_remote_sta;
extern uint32_t g_remote_data;  /* 红外接收到的数据 */
extern uint8_t g_remote_cnt;    /* 按键按下的次数 */
extern uint8_t g_valid_channel; /* 当前捕获通道 */
extern const uint8_t REMOTE_ID;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
uint8_t remote_scan(void);
uint8_t get_IR_Address(void);
void DataCollect(TIM_HandleTypeDef *htim);

#endif