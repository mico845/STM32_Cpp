//
// Created by Whisky on 1/13/2024.
//

#ifndef HELLOWORLD_LCD_SPI_H
#define HELLOWORLD_LCD_SPI_H
#include "Platform-STM32.h"
#include "OnChip/GPIO/GPIO.hpp"
#include "LCD_Lib.hpp"

// 定义LCD所使用的SPI
#ifndef LCD_SPI_HANDEL
    #define LCD_SPI_HANDEL  (SPI6)
#endif
//定义LCD_DC
#ifndef LCD_DC_GPIO
    #define LCD_DC_GPIO
    #define LCD_DC_PORT     (STM32::STM32_GPIO::B)
    #define LCD_DC_PIN      (4)
#endif
//定义LCD_RST
#ifndef LCD_RST_GPIO
    #define LCD_RST_GPIO
    #define LCD_RST_PORT    (STM32::STM32_GPIO::D)
    #define LCD_RST_PIN     (4)
#endif
//定义LCD_PWR
#ifndef LCD_PWR_GPIO
    #define LCD_PWR_GPIO
    #define LCD_PWR_PORT    (STM32::STM32_GPIO::D)
    #define LCD_PWR_PIN     (3)
#endif

//SPI超时时间
#ifndef LCD_SPI_TIMEOUT
    #define LCD_SPI_TIMEOUT (200)
#endif

//LCD缓存大小设置
#ifndef LCD_TOTAL_BUF_SIZE
    #define LCD_TOTAL_BUF_SIZE	(240*135*2)
#endif
#ifndef LCD_BUF_SIZE
    #define LCD_BUF_SIZE (1152)
#endif

//默认画笔颜色
extern Platform::HardWare::TFT_LCD::LCD_Color_TypeDef DEFAULT_PENCIL_COLOR;
//默认背景颜色
extern Platform::HardWare::TFT_LCD::LCD_Color_TypeDef DEFAULT_BACK_COLOR;
//默认字体大小
extern Platform::HardWare::TFT_LCD::LCD_FontSize_TypeDef DEFAULT_FONT_SIZE;

class LCD_SPI
{
private:
    /* Variables */
    GPIO gpio_dc;
    GPIO gpio_rst;
    GPIO gpio_pwr;

    bool isUseHorizontal;

    uint8_t width;
    uint8_t height;
    uint8_t lcd_buf[LCD_BUF_SIZE];

    uint8_t current_x = 0;
    uint8_t current_y = 0;
    unsigned char precision = 3; // 小数显示位数
    /* Functions */
    __INLINE void Write_Cmd(uint8_t cmd);
    __INLINE void Write_Data(uint8_t data);
    __INLINE void Write_Data_HalfWord(uint16_t data);

    void MemoryDataAccessControl(void);
    void Set_Address(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
public:
    /* Variables */

    /* Constructor */
    LCD_SPI() = default;
    ~LCD_SPI() = default;
    LCD_SPI(bool IsUseHorizontal);

    /* Functions */
    __INLINE void Clear(Platform::HardWare::TFT_LCD::LCD_Color_TypeDef color = DEFAULT_BACK_COLOR);
    void Fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end,
                                Platform::HardWare::TFT_LCD::LCD_Color_TypeDef color = DEFAULT_PENCIL_COLOR);
    __INLINE void Draw_Point(uint16_t x, uint16_t y,
                                Platform::HardWare::TFT_LCD::LCD_Color_TypeDef color = DEFAULT_PENCIL_COLOR);
    void Draw_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                                Platform::HardWare::TFT_LCD::LCD_Color_TypeDef color = DEFAULT_PENCIL_COLOR);
    void Draw_Rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                                Platform::HardWare::TFT_LCD::LCD_Color_TypeDef color = DEFAULT_PENCIL_COLOR);
    void Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r,
                                Platform::HardWare::TFT_LCD::LCD_Color_TypeDef color = DEFAULT_PENCIL_COLOR);
    void Show_Char(uint16_t x, uint16_t y, char chr,
                                Platform::HardWare::TFT_LCD::LCD_FontSize_TypeDef size = DEFAULT_FONT_SIZE,
                                Platform::HardWare::TFT_LCD::LCD_Color_TypeDef color = DEFAULT_PENCIL_COLOR,
                                Platform::HardWare::TFT_LCD::LCD_Color_TypeDef back_color = DEFAULT_BACK_COLOR);
    void Show_Num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, bool Is_ZeroFill = false,
                                Platform::HardWare::TFT_LCD::LCD_FontSize_TypeDef size = DEFAULT_FONT_SIZE,
                                Platform::HardWare::TFT_LCD::LCD_Color_TypeDef color = DEFAULT_PENCIL_COLOR,
                                Platform::HardWare::TFT_LCD::LCD_Color_TypeDef back_color = DEFAULT_BACK_COLOR);
    //如果字符串宽度或者高度为0，就默认字符串以（x，y）和（width，height）为文本框，注width为屏幕宽度，height为屏幕高度
    void Show_String(uint16_t x, uint16_t y, const char *str,
                                uint16_t string_width = 0,
                                uint16_t string_height = 0,
                                Platform::HardWare::TFT_LCD::LCD_FontSize_TypeDef size = DEFAULT_FONT_SIZE,
                                Platform::HardWare::TFT_LCD::LCD_Color_TypeDef color = DEFAULT_PENCIL_COLOR,
                                Platform::HardWare::TFT_LCD::LCD_Color_TypeDef back_color = DEFAULT_BACK_COLOR);
    //Image2Lcd取模方式：	C语言数据/水平扫描/16位真彩色(RGB565)/高位在前		其他的不要选
    void Show_Image(uint16_t x, uint16_t y, uint16_t Image_width, uint16_t Image_height, const uint8_t *Image);


    //LCD支持流操作
    LCD_SPI& operator <<(const char* pStr);
    LCD_SPI& operator <<(Platform::HardWare::TFT_LCD::ENDL& e);     //换行
    LCD_SPI& operator <<(Platform::HardWare::TFT_LCD::CLEAR& c);    //清屏，并当前位置清空
    LCD_SPI& operator <<(const char charactor);
    LCD_SPI& operator <<(int val);
    LCD_SPI& operator <<(double val);

};

__STATIC_INLINE void LCD_SPI_SendBytes(const uint8_t* data, uint16_t size)
{
    LL_SPI_SetHalfDuplexDirection(LCD_SPI_HANDEL, LL_SPI_HALF_DUPLEX_TX);
    LL_SPI_SetTransferSize(LCD_SPI_HANDEL, size);
    LL_SPI_Enable(LCD_SPI_HANDEL);
    LL_SPI_StartMasterTransfer(LCD_SPI_HANDEL);
    while (size > 0U)
    {
        if (LL_SPI_IsActiveFlag_TXP(LCD_SPI_HANDEL))
        {
              LL_SPI_TransmitData8(LCD_SPI_HANDEL, *((const uint8_t *)data));
            data += sizeof(uint8_t);
            size--;
        }
    }
    while (LL_SPI_IsActiveFlag_EOT(LCD_SPI_HANDEL) == RESET);
    LL_SPI_ClearFlag_EOT(LCD_SPI_HANDEL);
    LL_SPI_ClearFlag_TXTF(LCD_SPI_HANDEL);
    LL_SPI_Disable(LCD_SPI_HANDEL);
}

__STATIC_INLINE void LCD_SPI_SendByte(const uint8_t data)
{
    LCD_SPI_SendBytes(&data, 1);
}


__INLINE void LCD_SPI::Write_Cmd(uint8_t cmd)
{
    gpio_dc.Low();
    LCD_SPI_SendByte(cmd);
}

__INLINE void LCD_SPI::Write_Data(uint8_t data)
{
    gpio_dc.High();
    LCD_SPI_SendByte(data);
}

__INLINE void LCD_SPI::Write_Data_HalfWord(uint16_t data)
{
    uint8_t buffer[2] = {0};
    buffer[0] = data >> 8;
    buffer[1] = data;
    gpio_dc.High();
    LCD_SPI_SendBytes(buffer, 2);
}

__INLINE void LCD_SPI::Draw_Point(uint16_t x, uint16_t y, Platform::HardWare::TFT_LCD::LCD_Color_TypeDef color)
{
    Set_Address(x, y, x, y);
    Write_Data_HalfWord(color);
}

__INLINE void LCD_SPI::Clear(Platform::HardWare::TFT_LCD::LCD_Color_TypeDef color) {
    Fill(0,0,width,height, color);
}


#endif //HELLOWORLD_LCD_SPI_H
