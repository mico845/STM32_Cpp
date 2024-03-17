#include "platform-lib/HardWare/LCD_SPI/LCD_SPI.hpp"
#include "platform-lib/HardWare/LCD_SPI/LCD_Font.hpp"
#include "platform-drivers/STM32/Delay/Delay.hpp"

using namespace STM32;

//默认画笔颜色
LCD_Color_TypeDef DEFAULT_PENCIL_COLOR = CYAN;
//默认背景颜色
LCD_Color_TypeDef DEFAULT_BACK_COLOR = BLACK;
//默认字体大小
LCD_FontSize_TypeDef DEFAULT_FONT_SIZE = FontSize_24;

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

    gpio_pwr.low();
    delay_ms(120);
    gpio_rst.low();
    delay_ms(120);
    gpio_rst.high();
    delay_ms(120);
    /* Sleep Out */
    writeCmd(0x11);
    /* wait for power stability */
    delay_ms(120);
    /* Memory Data Access Control */
    memoryDataAccessControl();

    /* RGB 5-6-5-bit  */
    writeCmd(0x3A);
    writeData(0x05);

    /* Porch Setting */
    writeCmd(0xB2);
    writeData(0x0C);
    writeData(0x0C);
    writeData(0x00);
    writeData(0x33);
    writeData(0x33);

    /*  Gate Control */
    writeCmd(0xB7);
    writeData(0x35);

    /* VCOM Setting */
    writeCmd(0xBB);
    writeData(0x19);   //Vcom=1.625V

    /* LCM Control */
    writeCmd(0xC0);
    writeData(0x2C);

    /* VDV and VRH Command Enable */
    writeCmd(0xC2);
    writeData(0x01);

    /* VRH Set */
    writeCmd(0xC3);
    writeData(0x12);

    /* VDV Set */
    writeCmd(0xC4);
    writeData(0x20);

    /* Frame Rate Control in Normal Mode */
    writeCmd(0xC6);
    writeData(0x0F);	//60MHZ

    /* Power Control 1 */
    writeCmd(0xD0);
    writeData(0xA4);
    writeData(0xA1);

    /* Positive Voltage Gamma Control */
    writeCmd(0xE0);
    writeData(0xD0);
    writeData(0x04);
    writeData(0x0D);
    writeData(0x11);
    writeData(0x13);
    writeData(0x2B);
    writeData(0x3F);
    writeData(0x54);
    writeData(0x4C);
    writeData(0x18);
    writeData(0x0D);
    writeData(0x0B);
    writeData(0x1F);
    writeData(0x23);

    /* Negative Voltage Gamma Control */
    writeCmd(0xE1);
    writeData(0xD0);
    writeData(0x04);
    writeData(0x0C);
    writeData(0x11);
    writeData(0x13);
    writeData(0x2C);
    writeData(0x3F);
    writeData(0x44);
    writeData(0x51);
    writeData(0x2F);
    writeData(0x1F);
    writeData(0x1F);
    writeData(0x20);
    writeData(0x23);

    /* Display Inversion On */
    writeCmd(0x21);

    writeCmd(0x29);

    setAddress(0, 0, width - 1, height - 1);

    gpio_pwr.high();
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

void LCD_SPI::memoryDataAccessControl(void) {
    writeCmd(0x36);
    if(isUseHorizontal)
        writeData(0x70);
    else
        writeData(0x00);
}

void LCD_SPI::setAddress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    if(isUseHorizontal)
    {
        writeCmd(0x2a);//列地址设置
        writeDataHalfWord(x1 + 40);
        writeDataHalfWord(x2 + 40);
        writeCmd(0x2b);//行地址设置
        writeDataHalfWord(y1 + 53);
        writeDataHalfWord(y2 + 53);
        writeCmd(0x2c);//储存器写
    }
    else
    {
        writeCmd(0x2a);//列地址设置
        writeDataHalfWord(x1 + 52);
        writeDataHalfWord(x2 + 52);
        writeCmd(0x2b);//行地址设置
        writeDataHalfWord(y1 + 40);
        writeDataHalfWord(y2 + 40);
        writeCmd(0x2c);//储存器写
    }
}

void LCD_SPI::fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, LCD_Color_TypeDef color) {
    uint16_t i;
    uint32_t size, size_remain;

    size = (x_end - x_start + 1) * (y_end - y_start + 1) * 2;

    if(size > LCD_BUF_SIZE)
    {
        size_remain = size - LCD_BUF_SIZE;
        size = LCD_BUF_SIZE;
    }

    setAddress(x_start, y_start, x_end, y_end);

    while(1)
    {
        for(i = 0; i < size / 2; i++)
        {
            lcd_buf[2 * i] = color >> 8;
            lcd_buf[2 * i + 1] = color;
        }

        gpio_dc.high();
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

void LCD_SPI::draw_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, LCD_Color_TypeDef color) {
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;
    uint32_t i;

    if(y1 == y2)
    {
        /*快速画水平线*/
        setAddress(x1, y1, x2, y2);

        for(i = 0; i < x2 - x1; i++)
        {
            lcd_buf[2 * i] = color >> 8;
            lcd_buf[2 * i + 1] = color;
        }
        gpio_dc.high();
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
        draw_Point(row, col, color);
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

void LCD_SPI::draw_Rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                             LCD_Color_TypeDef color) {
    draw_Line(x1, y1, x2, y1, color);
    draw_Line(x1, y1, x1, y2, color);
    draw_Line(x1, y2, x2, y2, color);
    draw_Line(x2, y1, x2, y2, color);
}

void LCD_SPI::draw_Circle(uint16_t x0, uint16_t y0, uint8_t r, LCD_Color_TypeDef color) {
    int a, b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r << 1);

    while(a <= b)
    {
        draw_Point(x0 - b, y0 - a, color);
        draw_Point(x0 + b, y0 - a, color);
        draw_Point(x0 - a, y0 + b, color);
        draw_Point(x0 - b, y0 - a, color);
        draw_Point(x0 - a, y0 - b, color);
        draw_Point(x0 + b, y0 + a, color);
        draw_Point(x0 + a, y0 - b, color);
        draw_Point(x0 + a, y0 + b, color);
        draw_Point(x0 - b, y0 + a, color);
        a++;

        if(di < 0)di += 4 * a + 6;
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }

        draw_Point(x0 + a, y0 + b, color);
    }
}

void LCD_SPI::show_Char(uint16_t x, uint16_t y, char chr, LCD_FontSize_TypeDef size,
                        LCD_Color_TypeDef color,
                        LCD_Color_TypeDef back_color)
{
    uint8_t temp, t1, t;
    uint8_t csize;		//得到字体一个字符对应点阵集所占的字节数
    uint16_t colortemp;
    uint8_t sta;

    chr = chr - ' '; //得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）

    if((x > (width - size / 2)) || (y > (height - size)))	return;

    setAddress(x, y, x + size / 2 - 1, y + size - 1);//(x,y,x+8-1,y+16-1)

    if((size == FontSize_16) || (size == FontSize_32) )	//16和32号字体
    {
        csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);

        for(t = 0; t < csize; t++)
        {
            if(size == FontSize_16)temp = asc2_1608[chr][t];	//调用1608字体
            else temp = asc2_3216[chr][t];	//调用3216字体

            for(t1 = 0; t1 < 8; t1++)
            {
                if(temp & 0x80) colortemp = color;
                else colortemp = back_color;

                writeDataHalfWord(colortemp);
                temp <<= 1;
            }
        }
    }

    else if  (size == FontSize_12)	//12号字体
    {
        csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);

        for(t = 0; t < csize; t++)
        {
            temp = asc2_1206[chr][t];

            for(t1 = 0; t1 < 6; t1++)
            {
                if(temp & 0x80) colortemp = color;
                else colortemp = back_color;

                writeDataHalfWord(colortemp);
                temp <<= 1;
            }
        }
    }

    else if(size == FontSize_24)		//24号字体
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

                writeDataHalfWord(colortemp);
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

void LCD_SPI::show_Num(uint16_t x, uint16_t y, uint32_t num, uint8_t len,
                       bool Is_ZeroFill,
                       LCD_FontSize_TypeDef size,
                       LCD_Color_TypeDef color,
                       LCD_Color_TypeDef back_color)
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
                    show_Char(x + (size / 2) * t, y, '0', size, color, back_color);
                else
                    show_Char(x + (size / 2) * t, y, ' ', size, color, back_color);
                continue;
            }

            else enshow = 1;
        }

        show_Char(x + (size / 2) * t, y, temp + '0', size, color, back_color);
    }
}

void LCD_SPI::show_String(uint16_t x, uint16_t y, const char *str, uint16_t string_width, uint16_t string_height,
                          LCD_FontSize_TypeDef size,
                          LCD_Color_TypeDef color,
                          LCD_Color_TypeDef back_color)
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

        show_Char(x, y, *str, size, color, back_color);
        x += size / 2;
        str++;
    }
}

void LCD_SPI::show_Image(uint16_t x, uint16_t y, uint16_t Image_width, uint16_t Image_height, const uint8_t *Image) {
    if(x + Image_width > width || y + Image_height > height)
        return;
    setAddress(x, y, x + Image_width - 1, y + Image_height - 1);
    gpio_dc.high();
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
            show_Char(current_x, current_y, pStr[i]);
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

LCD_SPI &LCD_SPI::operator<<(STM32::ENDL &e) {
    current_x = 0;
    current_y += DEFAULT_FONT_SIZE;
    if (current_y >= height)
        current_y = 0;
    return *this;
}

LCD_SPI &LCD_SPI::operator<<(CLEAR &c) {
    current_x = 0;
    current_y = 0;
    clear();
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
        show_Char(current_x, current_y, charactor);
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









