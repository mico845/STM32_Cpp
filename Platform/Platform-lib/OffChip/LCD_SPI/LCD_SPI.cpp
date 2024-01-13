//
// Created by Whisky on 1/13/2024.
//

#include "LCD_SPI.hpp"
#include "LCD_Font.hpp"

#include "Machine/Platform-Machine.hpp"
using namespace Platform::Machine;

//默认画笔颜色
Platform::HardWare::TFT_LCD::LCD_Color_TypeDef DEFAULT_PENCIL_COLOR = Platform::HardWare::TFT_LCD::CYAN;
//默认背景颜色
Platform::HardWare::TFT_LCD::LCD_Color_TypeDef DEFAULT_BACK_COLOR = Platform::HardWare::TFT_LCD::BLACK;
//默认字体大小
Platform::HardWare::TFT_LCD::LCD_FontSize_TypeDef DEFAULT_FONT_SIZE = Platform::HardWare::TFT_LCD::FontSize_24;

LCD_SPI::LCD_SPI(bool IsUseHorizontal)
        :isUseHorizontal(IsUseHorizontal)
{
    if (isUseHorizontal)
    {
        width = 240;height = 135;
    }
    else
    {
        width = 135;height = 240;
    }
    gpio_dc  = GPIO(LCD_DC_PORT, LCD_DC_PIN, out, pp, pullup, very_fast);
    gpio_rst = GPIO(LCD_RST_PORT, LCD_RST_PIN, out, pp, pullup, very_fast);
    gpio_pwr = GPIO(LCD_PWR_PORT, LCD_PWR_PIN, out, pp, pullup, very_fast);

    gpio_pwr.Low();
    delay.ms(120);
    gpio_rst.Low();
    delay.ms(120);
    gpio_rst.High();
    delay.ms(120);
    /* Sleep Out */
    Write_Cmd(0x11);
    /* wait for power stability */
    delay.ms(120);
    /* Memory Data Access Control */
    MemoryDataAccessControl();

    /* RGB 5-6-5-bit  */
    Write_Cmd(0x3A);
    Write_Data(0x05);

    /* Porch Setting */
    Write_Cmd(0xB2);
    Write_Data(0x0C);
    Write_Data(0x0C);
    Write_Data(0x00);
    Write_Data(0x33);
    Write_Data(0x33);

    /*  Gate Control */
    Write_Cmd(0xB7);
    Write_Data(0x35);

    /* VCOM Setting */
    Write_Cmd(0xBB);
    Write_Data(0x19);   //Vcom=1.625V

    /* LCM Control */
    Write_Cmd(0xC0);
    Write_Data(0x2C);

    /* VDV and VRH Command Enable */
    Write_Cmd(0xC2);
    Write_Data(0x01);

    /* VRH Set */
    Write_Cmd(0xC3);
    Write_Data(0x12);

    /* VDV Set */
    Write_Cmd(0xC4);
    Write_Data(0x20);

    /* Frame Rate Control in Normal Mode */
    Write_Cmd(0xC6);
    Write_Data(0x0F);	//60MHZ

    /* Power Control 1 */
    Write_Cmd(0xD0);
    Write_Data(0xA4);
    Write_Data(0xA1);

    /* Positive Voltage Gamma Control */
    Write_Cmd(0xE0);
    Write_Data(0xD0);
    Write_Data(0x04);
    Write_Data(0x0D);
    Write_Data(0x11);
    Write_Data(0x13);
    Write_Data(0x2B);
    Write_Data(0x3F);
    Write_Data(0x54);
    Write_Data(0x4C);
    Write_Data(0x18);
    Write_Data(0x0D);
    Write_Data(0x0B);
    Write_Data(0x1F);
    Write_Data(0x23);

    /* Negative Voltage Gamma Control */
    Write_Cmd(0xE1);
    Write_Data(0xD0);
    Write_Data(0x04);
    Write_Data(0x0C);
    Write_Data(0x11);
    Write_Data(0x13);
    Write_Data(0x2C);
    Write_Data(0x3F);
    Write_Data(0x44);
    Write_Data(0x51);
    Write_Data(0x2F);
    Write_Data(0x1F);
    Write_Data(0x1F);
    Write_Data(0x20);
    Write_Data(0x23);

    /* Display Inversion On */
    Write_Cmd(0x21);

    Write_Cmd(0x29);

    Set_Address(0, 0, width - 1, height - 1);

    gpio_pwr.High();
}

/*
 * MADCTL (36h): Memory Data Access Control
    Bit D7- Page Address Order
    “0” = Top to Bottom (When MADCTL D7=”0”).
    “1” = Bottom to Top (When MADCTL D7=”1”).
    Bit D6- Column Address Order
    “0” = Left to Right (When MADCTL D6=”0”).
    “1” = Right to Left (When MADCTL D6=”1”).
    Bit D5- Page/Column Order
    “0” = Normal Mode (When MADCTL D5=”0”).
    “1” = Reverse Mode (When MADCTL D5=”1”)
    Note: Bits D7 to D5, alse refer to section 8.12 Address Control
    Bit D4- Line Address Order
    “0” = LCD Refresh Top to Bottom (When MADCTL D4=”0”)
    “1” = LCD Refresh Bottom to Top (When MADCTL D4=”1”)
    Bit D3- RGB/BGR Order
    “0” = RGB (When MADCTL D3=”0”)
    “1” = BGR (When MADCTL D3=”1”)
    Bit D2- Display Data Latch Data Order
    “0” = LCD Refresh Left to Right (When MADCTL D2=”0”)
    “1” = LCD Refresh Right to Left (When MADCTL D2=”1”)
 * */

void LCD_SPI::MemoryDataAccessControl(void) {
    Write_Cmd(0x36);
    if(isUseHorizontal)
        Write_Data(0x70);
    else
        Write_Data(0x00);
}

void LCD_SPI::Set_Address(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    if(isUseHorizontal)
    {
        Write_Cmd(0x2a);//列地址设置
        Write_Data_HalfWord(x1+40);
        Write_Data_HalfWord(x2+40);
        Write_Cmd(0x2b);//行地址设置
        Write_Data_HalfWord(y1+53);
        Write_Data_HalfWord(y2+53);
        Write_Cmd(0x2c);//储存器写
    }
    else
    {
        Write_Cmd(0x2a);//列地址设置
        Write_Data_HalfWord(x1+52);
        Write_Data_HalfWord(x2+52);
        Write_Cmd(0x2b);//行地址设置
        Write_Data_HalfWord(y1+40);
        Write_Data_HalfWord(y2+40);
        Write_Cmd(0x2c);//储存器写
    }
}

void LCD_SPI::Fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, Platform::HardWare::TFT_LCD::LCD_Color_TypeDef color) {
    uint16_t i;
    uint32_t size, size_remain;

    size = (x_end - x_start + 1) * (y_end - y_start + 1) * 2;

    if(size > LCD_BUF_SIZE)
    {
        size_remain = size - LCD_BUF_SIZE;
        size = LCD_BUF_SIZE;
    }

    Set_Address(x_start, y_start, x_end, y_end);

    while(1)
    {
        for(i = 0; i < size / 2; i++)
        {
            lcd_buf[2 * i] = color >> 8;
            lcd_buf[2 * i + 1] = color;
        }

        gpio_dc.High();
        LCD_SPI_SendBytes(lcd_buf, size);

        if(size_remain == 0)
            break;

        if(size_remain > LCD_BUF_SIZE)
        {
            size_remain = size_remain - LCD_BUF_SIZE;
        }

        else
        {
            size = size_remain;
            size_remain = 0;
        }
    }
}

void LCD_SPI::Draw_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, Platform::HardWare::TFT_LCD::LCD_Color_TypeDef color) {
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;
    uint32_t i;

    if(y1 == y2)
    {
        /*快速画水平线*/
        Set_Address(x1, y1, x2, y2);

        for(i = 0; i < x2 - x1; i++)
        {
            lcd_buf[2 * i] = color >> 8;
            lcd_buf[2 * i + 1] = color;
        }
        gpio_dc.High();
        LCD_SPI_SendBytes(lcd_buf, (x2 - x1) * 2);
        return;
    }

    delta_x = x2 - x1;
    delta_y = y2 - y1;
    row = x1;
    col = y1;

    if(delta_x > 0)incx = 1;

    else if(delta_x == 0)incx = 0;

    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if(delta_y > 0)incy = 1;

    else if(delta_y == 0)incy = 0;

    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if(delta_x > delta_y)distance = delta_x;

    else distance = delta_y;

    for(t = 0; t <= distance + 1; t++)
    {
        Draw_Point(row, col, color);
        xerr += delta_x ;
        yerr += delta_y ;

        if(xerr > distance)
        {
            xerr -= distance;
            row += incx;
        }

        if(yerr > distance)
        {
            yerr -= distance;
            col += incy;
        }
    }
}

void LCD_SPI::Draw_Rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                             Platform::HardWare::TFT_LCD::LCD_Color_TypeDef color) {
    Draw_Line(x1, y1, x2, y1, color);
    Draw_Line(x1, y1, x1, y2, color);
    Draw_Line(x1, y2, x2, y2, color);
    Draw_Line(x2, y1, x2, y2, color);
}

void LCD_SPI::Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r, Platform::HardWare::TFT_LCD::LCD_Color_TypeDef color) {
    int a, b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r << 1);

    while(a <= b)
    {
        Draw_Point(x0 - b, y0 - a, color);
        Draw_Point(x0 + b, y0 - a, color);
        Draw_Point(x0 - a, y0 + b, color);
        Draw_Point(x0 - b, y0 - a, color);
        Draw_Point(x0 - a, y0 - b, color);
        Draw_Point(x0 + b, y0 + a, color);
        Draw_Point(x0 + a, y0 - b, color);
        Draw_Point(x0 + a, y0 + b, color);
        Draw_Point(x0 - b, y0 + a, color);
        a++;

        if(di < 0)di += 4 * a + 6;
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }

        Draw_Point(x0 + a, y0 + b, color);
    }
}

void LCD_SPI::Show_Char(uint16_t x, uint16_t y, char chr, Platform::HardWare::TFT_LCD::LCD_FontSize_TypeDef size,
                        Platform::HardWare::TFT_LCD::LCD_Color_TypeDef color,
                        Platform::HardWare::TFT_LCD::LCD_Color_TypeDef back_color)
{
    uint8_t temp, t1, t;
    uint8_t csize;		//得到字体一个字符对应点阵集所占的字节数
    uint16_t colortemp;
    uint8_t sta;

    chr = chr - ' '; //得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）

    if((x > (width - size / 2)) || (y > (height - size)))	return;

    Set_Address(x, y, x + size / 2 - 1, y + size - 1);//(x,y,x+8-1,y+16-1)

    if((size == Platform::HardWare::TFT_LCD::FontSize_16) || (size == Platform::HardWare::TFT_LCD::FontSize_32) )	//16和32号字体
    {
        csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);

        for(t = 0; t < csize; t++)
        {
            if(size == Platform::HardWare::TFT_LCD::FontSize_16)temp = asc2_1608[chr][t];	//调用1608字体
            else temp = asc2_3216[chr][t];	//调用3216字体

            for(t1 = 0; t1 < 8; t1++)
            {
                if(temp & 0x80) colortemp = color;
                else colortemp = back_color;

                Write_Data_HalfWord(colortemp);
                temp <<= 1;
            }
        }
    }

    else if  (size == Platform::HardWare::TFT_LCD::FontSize_12)	//12号字体
    {
        csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);

        for(t = 0; t < csize; t++)
        {
            temp = asc2_1206[chr][t];

            for(t1 = 0; t1 < 6; t1++)
            {
                if(temp & 0x80) colortemp = color;
                else colortemp = back_color;

                Write_Data_HalfWord(colortemp);
                temp <<= 1;
            }
        }
    }

    else if(size == Platform::HardWare::TFT_LCD::FontSize_24)		//24号字体
    {
        csize = (size * 16) / 8;

        for(t = 0; t < csize; t++)
        {
            temp = asc2_2412[chr][t];

            if(t % 2 == 0)sta = 8;
            else sta = 4;

            for(t1 = 0; t1 < sta; t1++)
            {
                if(temp & 0x80) colortemp = color;
                else colortemp = back_color;

                Write_Data_HalfWord(colortemp);
                temp <<= 1;
            }
        }
    }
    else //没有的字库
        return;
}


static uint32_t LCD_Pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;

    while(n--)result *= m;

    return result;
}

void LCD_SPI::Show_Num(uint16_t x, uint16_t y, uint32_t num, uint8_t len,
                       bool Is_ZeroFill,
                       Platform::HardWare::TFT_LCD::LCD_FontSize_TypeDef size,
                       Platform::HardWare::TFT_LCD::LCD_Color_TypeDef color,
                       Platform::HardWare::TFT_LCD::LCD_Color_TypeDef back_color)
{
    uint8_t t, temp;
    uint8_t enshow = 0;

    for(t = 0; t < len; t++)
    {
        temp = (num / LCD_Pow(10, len - t - 1)) % 10;

        if(enshow == 0 && t < (len - 1))
        {
            if(temp == 0)
            {
                if (Is_ZeroFill)
                    Show_Char(x + (size / 2)*t, y, '0', size, color, back_color);
                else
                    Show_Char(x + (size / 2)*t, y, ' ', size, color, back_color);
                continue;
            }

            else enshow = 1;
        }

        Show_Char(x + (size / 2)*t, y, temp + '0', size, color, back_color);
    }
}

void LCD_SPI::Show_String(uint16_t x, uint16_t y, const char *str, uint16_t string_width, uint16_t string_height,
                          Platform::HardWare::TFT_LCD::LCD_FontSize_TypeDef size,
                          Platform::HardWare::TFT_LCD::LCD_Color_TypeDef color,
                          Platform::HardWare::TFT_LCD::LCD_Color_TypeDef back_color)
{
    uint8_t x0 = x;
    if ((string_width!=0) && (string_height!=0))
    {
        string_width += x;
        string_height += y;
    }
    else
    {
        string_width = width;
        string_height = height;
    }

    while((*str <= '~') && (*str >= ' ')) //判断是不是非法字符!
    {
        if(x >= string_width)
        {
            x = x0;
            y += size;
        }

        if(y >= string_height)break; //退出

        Show_Char(x, y, *str, size, color, back_color);
        x += size / 2;
        str++;
    }
}

void LCD_SPI::Show_Image(uint16_t x, uint16_t y, uint16_t Image_width, uint16_t Image_height, const uint8_t *Image) {
    if(x + Image_width > width || y + Image_height > height)
        return;
    Set_Address(x, y, x + Image_width - 1, y + Image_height - 1);
    gpio_dc.High();
    LCD_SPI_SendBytes((uint8_t *)Image, Image_width * Image_height * 2);
}

LCD_SPI &LCD_SPI::operator<<(const char *pStr) {
    for(int i = 0; pStr[i] != '\0'; ++i)
    {
        if (pStr[i] == '\n')
        {
            current_x = 0;
            current_y += DEFAULT_FONT_SIZE;
            if (current_y >= height)
                current_y = 0;
        }
        else if ((pStr[i] > '~') || (pStr[i] < ' '))
            continue;
        else
        {
            Show_Char(current_x, current_y, pStr[i]);
            current_x += DEFAULT_FONT_SIZE/2;
            if (current_x >= width)
            {
                current_x = current_x - width;
                current_y += DEFAULT_FONT_SIZE;
                if (current_y >= height)
                    current_y = 0;
            }
        }
    }
    return *this;
}

LCD_SPI &LCD_SPI::operator<<(Platform::HardWare::TFT_LCD::ENDL &e) {
    current_x = 0;
    current_y += DEFAULT_FONT_SIZE;
    if (current_y >= height)
        current_y = 0;
    return *this;
}

LCD_SPI &LCD_SPI::operator<<(Platform::HardWare::TFT_LCD::CLEAR &c) {
    current_x = 0;
    current_y = 0;
    Clear();
    return *this;
}

LCD_SPI &LCD_SPI::operator<<(const char charactor) {
    if (charactor == '\n')
    {
        current_x = 0;
        current_y += DEFAULT_FONT_SIZE;
        if (current_y >= height)
            current_y = 0;
    }
    else if ((charactor > '~') || (charactor < ' '))
        return *this;
    else
    {
        Show_Char(current_x, current_y, charactor);
        current_x += DEFAULT_FONT_SIZE/2;
        if (current_x >= width)
        {
            current_x = current_x - width;
            current_y += DEFAULT_FONT_SIZE;
            if (current_y >= height)
                current_y = 0;
        }
    }
    return *this;
}

LCD_SPI &LCD_SPI::operator<<(int val) {
    uint8_t sign = 0, len = 0,data[10];
    uint8_t *p;
    if(val<0)
    {
        sign = 1;
        val = -val;
    }
    do
    {
        len++;
        data[10-len] = val%10 + '0';
        val = val/10;
    }while(val);
    if(sign==1)
        data[10-(++len)] = '-';
    p = data+10-len;
    for (int i = 0; i < len; ++i) {
        *this << (const char)(p[i]);
    }
    return *this;
}

LCD_SPI &LCD_SPI::operator<<(double val) {
    uint8_t sign = 0, len = 0,data[20];
    uint8_t *p;
    if(val<0)
    {
        sign = 1;
        val = -val;
    }
    uint8_t prec = precision;
    while(prec--)
        val *= 10;
    uint32_t t = val;
    do
    {
        if(++len==precision+1) data[20-len] = '.';
        else
        {
            data[20-len] = t%10 + '0';
            t = t/10;
        }
    }while(t || len<precision+2);
    if(sign==1)
        data[20-(++len)] = '-';
    p = data+20-len;
    for (int i = 0; i < len; ++i) {
        *this << (const char)(p[i]);
    }
    return *this;
}







