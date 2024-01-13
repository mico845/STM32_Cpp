//
// Created by Whisky on 1/13/2024.
//

#ifndef HELLOWORLD_LCD_LIB_H
#define HELLOWORLD_LCD_LIB_H

namespace Platform::HardWare::TFT_LCD{
    typedef enum LCD_Color_TypeDef
    {
        WHITE = 0xFFFF,
        BLACK = 0x0000,
        BLUE = 0x001F,
        BRED = 0XF81F,
        GRED = 0XFFE0,
        GBLUE = 0X07FF,
        RED  =  0xF800,
        MAGENTA = 0xF81F,
        GREEN = 0x07E0,
        CYAN = 0x7FFF,
        YELLOW = 0xFFE0,
        BROWN =  0XBC40,//棕色
        BRRED =  0XFC07, //棕红色
        GRAY   =  0X8430, //灰色
        //GUI颜色

        DARKBLUE  =  0X01CF,	//深蓝色
        LIGHTBLUE  =  0X7D7C,	//浅蓝色
        GRAYBLUE   =  0X5458, //灰蓝色
        //以上三色为PANEL的颜色

        LIGHTGREEN  =  0X841F, //浅绿色
//    LIGHTGRAY  =  0XEF5B, //浅灰色(PANNEL)
        LGRAY 	 =  0XC618, //浅灰色(PANNEL),窗体背景色

        LGRAYBLUE  =   0XA651, //浅灰蓝色(中间层颜色)
        LBBLUE    =    0X2B12, //浅棕蓝色(选择条目的反色)
    }LCD_Color_TypeDef;

    typedef enum LCD_FontSize_TypeDef
    {
        FontSize_12 = 12,
        FontSize_16 = 16,
        FontSize_24 = 24,
        FontSize_32 = 32,
    }LCD_FontSize_TypeDef;

    struct ENDL;
    inline struct ENDL { }endl;
    struct CLEAR;
    inline struct CLEAR { }clear;
}


#endif //HELLOWORLD_LCD_LIB_H
