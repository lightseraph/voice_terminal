#include "oled.h"
#include "oledfont.h"

u8 OLED_GRAM[128][8]; // 定义SRAM缓存区

/**
 * @brief 画点
 * @param x: 横坐标，范围：0-127
 * @param y: 纵坐标，范围：0-63
 * @param status: 设置状态 0灭，1亮
 * @retval None
 */
void OLED_DrawPoint(u8 x, u8 y, u8 t)
{
    if (x > 127 || y > 63) // 超出范围
        return;
    u8 pos, temp = 0;
    pos = 7 - y / 8;
    temp = 1 << (7 - y % 8);
    if (t)
        OLED_GRAM[x][pos] |= temp;
    else
        OLED_GRAM[x][pos] &= ~temp;
}

/**
 * @brief  在指定位置显示一个字符（需要字库数组）
 * @param  x: 起点横坐标
 * @param  y: 起点纵坐标
 * @param  chr: 需要显示的字符串
 * @param  size: 字体大小：12/16/24
 * @retval None
 */
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 size)
{
    u8 temp, t, s;
    u8 y0 = y;
    u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); // 得到一个字符所占字节数
    chr = chr - ' ';                                           // 得到偏移后的值
    for (t = 0; t < csize; t++)
    {
        if (size == 12)
            temp = asc2_1206[chr][t]; // 调用1206字体
        else if (size == 16)
            temp = asc2_1608[chr][t]; // 调用1608字体
        else if (size == 24)
            temp = asc2_2412[chr][t]; // 调用2412字体
        else
            return; // 没有的字库
        for (s = 0; s < 8; s++)
        {
            if (temp & 0x80)
                OLED_DrawPoint(x, y, 1);
            else
                OLED_DrawPoint(x, y, 0);
            temp <<= 1;
            y++;
            if ((y - y0) == size)
            {
                y = y0;
                x++;
                break;
            }
        }
    }
}

/**
 * @brief  显示字符串
 * @param  x: 起点横坐标
 * @param  y: 起点纵坐标
 * @param  p: 要显示的字符串
 * @param  size: 字体大小
 * @retval None
 */
void OLED_ShowString(u8 x, u8 y, const u8 *p, u8 size)
{
    while ((*p <= '~') && (*p >= ' ')) // 判断是不是非法字符
    {
        if (x > (128 - (size / 2)))
        {
            x = 0;
            y += size;
        }
        if (y > (64 - size))
        {
            y = x = 0;
            OLED_Clear();
        }
        OLED_ShowChar(x, y, *p, size);
        x += size / 2;
        p++;
    }
    OLED_Refresh_GRAM();
}

/**
 * @brief  显示数字
 * @param  (x,y): 起点坐标
 * @param  num: 数值(0~999999999);
 * @param  len: 长度（即要显示的位数）
 * @retval None
 */
void OLED_ShowNum(u16 x, u16 y, u32 num, u8 len, u8 size)
{
    u8 t, temp;
    u8 enshow = 0;
    for (t = 0; t < len; t++)
    {
        temp = (num / OLED_Pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                OLED_ShowChar(x + (size / 2) * t, y, ' ', size);
                continue;
            }
            else
                enshow = 1;
        }
        OLED_ShowChar(x + (size / 2) * t, y, temp + '0', size);
    }
    OLED_Refresh_GRAM();
}

// 显示位图
void OLED_ShowBMP(uc8 BMP[])
{
    u16 x, y, j = 0;
    u8 temp = 0;
    for (y = 7; y > 0; y--)
    {
        for (x = 0; x < 128; x++)
        {
            temp = BMP[j];
            temp = ((temp >> 1) & 0x55) | ((temp & 0x55) << 1);
            temp = ((temp >> 2) & 0x33) | ((temp & 0x33) << 2);
            temp = ((temp >> 4) & 0x0F) | ((temp & 0x0F) << 4);
            OLED_GRAM[x][y] = temp;
            j++;
        }
    }
    OLED_Refresh_GRAM();
}

// 计算m^n
u32 OLED_Pow(u8 m, u8 n)
{
    u32 result = 1;
    while (n--)
    {
        result *= m;
    }
    return result;
}

// 更新显存
void OLED_Refresh_GRAM(void)
{
    u8 i, n;
    for (i = 0; i < 8; i++)
    {
        OLED_Write_Cmd(0xb0 + i); // 设置页地址（0-7）
        OLED_Write_Cmd(0x00);     // 设置显示位置-列低地址
        OLED_Write_Cmd(0x10);     // 设置显示位置-列高地址
        for (n = 0; n < 128; n++)
            OLED_Write_Data(OLED_GRAM[n][i]); // 更新到OLED
    }
}

// 开启OLED显示
void OLED_Display_On(void)
{
    OLED_Write_Cmd(0X8D); // 设置DCDC命令
    OLED_Write_Cmd(0X14); // DCDC ON
    OLED_Write_Cmd(0XAF); // DISPLAY ON
}

// 关闭OLED显示
void OLED_Display_Off(void)
{
    OLED_Write_Cmd(0X8D); // 设置DCDC命令
    OLED_Write_Cmd(0X10); // DCDC OFF
    OLED_Write_Cmd(0XAE); // DISPLAY OFF
}

// OLED清屏
void OLED_Clear(void)
{
    u8 i, n;
    for (i = 0; i < 8; i++)
    {
        for (n = 0; n < 128; n++)
            OLED_GRAM[n][i] = 0X00;
    }
    OLED_Refresh_GRAM();
}

// OLED初始化：设置SSD1306参数
void OLED_Init(void)
{
    OLED_Write_Cmd(0xAE); // 关闭显示

    OLED_Write_Cmd(0xD5); // 设置时钟分频因子，振荡频率
    OLED_Write_Cmd(0x80); //[3:0],分频因子;[7:4],振荡频率

    OLED_Write_Cmd(0xA8); // 设置驱动路数
    OLED_Write_Cmd(0X3F); // 默认0X3F(1/64)

    OLED_Write_Cmd(0xD3); // 设置显示偏移
    OLED_Write_Cmd(0X00); // 默认为0

    OLED_Write_Cmd(0x40); // 设置显示开始行[5:0],行数.

    OLED_Write_Cmd(0x8D); // 电荷泵设置
    OLED_Write_Cmd(0x14); // bit2 开启/关闭

    OLED_Write_Cmd(0x20); // 设置内存地址模式
    OLED_Write_Cmd(0x02); //[1:0],00列地址模式;01行地址模式;10,页地址模式;默认10;

    OLED_Write_Cmd(0xA1); // 段重定义设置,bit0:0,0->0;1,0->127;

    OLED_Write_Cmd(0xC0); // 设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数

    OLED_Write_Cmd(0xDA); // 设置COM口硬件引脚配置
    OLED_Write_Cmd(0x12); //[5:4]配置

    OLED_Write_Cmd(0x81); // 对比度设置
    OLED_Write_Cmd(0xEF); // 1~255;默认0X7F (亮度设置，越大越亮)

    OLED_Write_Cmd(0xD9); // 设置预充电周期
    OLED_Write_Cmd(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;

    OLED_Write_Cmd(0xDB); // 设置VCOMH 电压倍率
    OLED_Write_Cmd(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

    OLED_Write_Cmd(0xA4); // 全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
    OLED_Write_Cmd(0xA6); // 设置显示方式;bit0:1,反相显示;0,正常显示
    OLED_Write_Cmd(0xAF); // 开启显示

    OLED_Clear(); // OLED清屏
}

// 写命令
void OLED_Write_Cmd(u8 command)
{
    OLED_Start();
    OLED_SendByte(0x78); // OLED地址
    OLED_Wait_Ack();
    OLED_SendByte(0x00); // 写命令寄存器地址
    OLED_Wait_Ack();
    OLED_SendByte(command);
    OLED_Wait_Ack();
    OLED_Stop();
}

// 写数据
void OLED_Write_Data(u8 data)
{
    OLED_Start();
    OLED_SendByte(0x78); // OLED地址
    OLED_Wait_Ack();
    OLED_SendByte(0x40); // 写数据寄存器地址
    OLED_Wait_Ack();
    OLED_SendByte(data);
    OLED_Wait_Ack();
    OLED_Stop();
}
/********************OLED I2C 操作函数********************/
void OLED_Start(void)
{
    OLED_SDA(1);
    OLED_SCL(1);
    OLED_SDA(0);
    OLED_SCL(0);
}

void OLED_Stop(void)
{
    OLED_SDA(0);
    OLED_SCL(1);
    OLED_SDA(1);
}

void OLED_Wait_Ack(void)
{
    OLED_SCL(1);
    OLED_SCL(0);
}

void OLED_SendByte(u8 txd)
{
    u8 t;
    OLED_SCL(0);
    for (t = 0; t < 8; t++)
    {
        if (txd & 0x80)
            OLED_SDA(1);
        else
            OLED_SDA(0);
        OLED_SCL(1);
        txd <<= 1;
        OLED_SCL(0);
    }
}