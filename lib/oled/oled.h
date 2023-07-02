#ifndef __OLED_H_
#define __OLED_H_

#include "main.h"

#define OLED_SCL(n) (n ? HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET) : HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET)) // SCL
#define OLED_SDA(n) (n ? HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET) : HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET)) // SDA

u32 OLED_Pow(u8 m, u8 n);
void OLED_DrawPoint(u8 x, u8 y, u8 t);
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 size);
void OLED_ShowString(u8 x, u8 y, const u8 *p, u8 size);
void OLED_ShowNum(u16 x, u16 y, u32 num, u8 len, u8 size);
void OLED_ShowBMP(uc8 BMP[]);
void OLED_Refresh_GRAM(void);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Clear(void);
void OLED_Init(void);
void OLED_Write_Cmd(u8 command);
void OLED_Write_Data(u8 data);
void OLED_Start(void);
void OLED_Stop(void);
void OLED_Wait_Ack(void);
void OLED_SendByte(u8 txd);
#endif
