#ifndef __HARDWARE_LCD_SPI_H__
#define __HARDWARE_LCD_SPI_H__

#include "platform-drivers/STM32/GPIO/GPIO.hpp"
#include "platform-lib/HardWare/LCD_SPI/LCD_Lib.hpp"
#include "platform-drivers/STM32/System/Sys.hpp"
#include "platform-drivers/STM32/SPI/SPI.hpp"

#ifndef LCD_SPI_HANDEL
    #define LCD_SPI_HANDEL  (SPI6)   /**< LCD使用的SPI指针 */
#endif

#ifndef LCD_DC_GPIO
    #define LCD_DC_GPIO
    #define LCD_DC_PORT     (GPIOB) /**< LCD_DC的GPIO端口 */
    #define LCD_DC_PIN      (4)     /**< LCD_DC的GPIO引脚号 */
#endif

#ifndef LCD_RST_GPIO
    #define LCD_RST_GPIO
    #define LCD_RST_PORT    (GPIOD) /**< LCD_RST的GPIO端口 */
    #define LCD_RST_PIN     (4)     /**< LCD_RST的GPIO引脚号 */
#endif

#ifndef LCD_PWR_GPIO
    #define LCD_PWR_GPIO
    #define LCD_PWR_PORT    (GPIOD) /**< LCD_PWR的GPIO引脚号 */
    #define LCD_PWR_PIN     (3)     /**< LCD_PWR的GPIO引脚号 */
#endif

#ifndef LCD_TOTAL_BUF_SIZE
    #define LCD_TOTAL_BUF_SIZE	(240*135*2)     /**< LCD缓存大小设置 */
#endif
#ifndef LCD_BUF_SIZE
    #define LCD_BUF_SIZE (1152)
#endif

extern LCD_Color_TypeDef 		DEFAULT_PENCIL_COLOR;   /**< LCD默认画笔颜色 */

extern LCD_Color_TypeDef 		DEFAULT_BACK_COLOR;     /**< LCD默认背景颜色 */

extern LCD_FontSize_TypeDef 	DEFAULT_FONT_SIZE;      /**< LCD默认字体大小 */

#ifndef LCD_STREAM_FLOAT_PRECISION
#define LCD_STREAM_FLOAT_PRECISION    (3)                   /**< LCD输出流的小数显示位数 */
#endif

/**
 * @brief LCD_SPI类
 *
 * 基于SPI通信的LCD屏幕的驱动。
 * ST7789V3
 * 240RGB x 320 dot 262K Color with Frame Memory Single-Chip TFT Controller/Driver
 *
 */
class LCD_SPI
{
private:
    /* Variables */
    STM32::GPIO gpio_dc;
    STM32::GPIO gpio_rst;
    STM32::GPIO gpio_pwr;

    bool isUseHorizontal;

    uint8_t width;
    uint8_t height;
    uint8_t lcd_buf[LCD_BUF_SIZE];

    uint8_t current_x = 0;
    uint8_t current_y = 0;
    unsigned char precision = LCD_STREAM_FLOAT_PRECISION; // 小数显示位数
    /* Functions */
    __INLINE void writeCmd(uint8_t cmd);
    __INLINE void writeData(uint8_t data);
    __INLINE void writeDataHalfWord(uint16_t data);

    void memoryDataAccessControl(void);
    void setAddress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
public:
    /* Variables */

    /* Constructor */
    LCD_SPI() = default;
    ~LCD_SPI() = default;
    /**
    * @brief LCD_SPI类构造函数
    * @param IsUseHorizontal 调节屏幕方向为水平方向，如果true则水平显示，如果false则竖直显示
    */
    LCD_SPI(bool IsUseHorizontal);

    /* Functions */
    /**
     * @brief 清空LCD屏幕
     * @param color 清空时填充的颜色，默认为DEFAULT_BACK_COLOR
     */
    __INLINE void clear(LCD_Color_TypeDef color = DEFAULT_BACK_COLOR);

    /**
     * @brief 填充LCD指定区域的颜色
     * @param x_start 填充区域左上角x坐标
     * @param y_start 填充区域左上角y坐标
     * @param x_end 填充区域右下角x坐标
     * @param y_end 填充区域右下角y坐标
     * @param color 填充的颜色，默认为DEFAULT_PENCIL_COLOR
     */
    void fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end,
              LCD_Color_TypeDef color = DEFAULT_PENCIL_COLOR);

    /**
     * @brief 在LCD屏幕上绘制一个点
     * @param x 点的x坐标
     * @param y 点的y坐标
     * @param color 点的颜色，默认为DEFAULT_PENCIL_COLOR
     */
    __INLINE void draw_Point(uint16_t x, uint16_t y,
                             LCD_Color_TypeDef color = DEFAULT_PENCIL_COLOR);

    /**
     * @brief 绘制一条直线
     * @param x1 起始点x坐标
     * @param y1 起始点y坐标
     * @param x2 结束点x坐标
     * @param y2 结束点y坐标
     * @param color 线条颜色，默认为DEFAULT_PENCIL_COLOR
     */
    void draw_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                   LCD_Color_TypeDef color = DEFAULT_PENCIL_COLOR);

    /**
     * @brief 绘制矩形
     * @param x1 左上角x坐标
     * @param y1 左上角y坐标
     * @param x2 右下角x坐标
     * @param y2 右下角y坐标
     * @param color 矩形填充颜色，默认为DEFAULT_PENCIL_COLOR
     */
    void draw_Rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                        LCD_Color_TypeDef color = DEFAULT_PENCIL_COLOR);
    /**
     * @brief 绘制圆形
     * @param x0 圆心x坐标
     * @param y0 圆心y坐标
     * @param r 圆半径
     * @param color 圆的颜色，默认为DEFAULT_PENCIL_COLOR
     */
    void draw_Circle(uint16_t x0, uint16_t y0, uint8_t r,
                     LCD_Color_TypeDef color = DEFAULT_PENCIL_COLOR);

    /**
     * @brief 在LCD屏幕上显示一个字符
     * @param x 字符左上角x坐标
     * @param y 字符左上角y坐标
     * @param chr 要显示的字符
     * @param size 字体大小，默认为DEFAULT_FONT_SIZE
     * @param color 字体颜色，默认为DEFAULT_PENCIL_COLOR
     * @param back_color 背景颜色，默认为DEFAULT_BACK_COLOR
     */
    void show_Char(uint16_t x, uint16_t y, char chr,
                   LCD_FontSize_TypeDef size = DEFAULT_FONT_SIZE,
                   LCD_Color_TypeDef color = DEFAULT_PENCIL_COLOR,
                   LCD_Color_TypeDef back_color = DEFAULT_BACK_COLOR);

    /**
     * @brief 在LCD屏幕上显示一个数字
     * @param x 数字左上角x坐标
     * @param y 数字左上角y坐标
     * @param num 要显示的数字
     * @param len 数字长度
     * @param Is_ZeroFill 空位是否以0填充，默认为false
     * @param size 字体大小，默认为DEFAULT_FONT_SIZE
     * @param color 字体颜色，默认为DEFAULT_PENCIL_COLOR
     * @param back_color 背景颜色，默认为DEFAULT_BACK_COLOR
     */
    void show_Num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, bool Is_ZeroFill = false,
                  LCD_FontSize_TypeDef size = DEFAULT_FONT_SIZE,
                  LCD_Color_TypeDef color = DEFAULT_PENCIL_COLOR,
                  LCD_Color_TypeDef back_color = DEFAULT_BACK_COLOR);
    /**
     * @brief 在LCD屏幕上显示一个字符串
     *
     * 如果字符串宽度或者高度为0，就默认字符串以（x，y）和（width，height）为文本框，注width为屏幕宽度，height为屏幕高度
     *
     * @param x 字符串左上角x坐标
     * @param y 字符串左上角y坐标
     * @param str 要显示的字符串
     * @param string_width 字符串宽度，默认为0
     * @param string_height 字符串高度，默认为0
     * @param size 字体大小，默认为DEFAULT_FONT_SIZE
     * @param color 字体颜色，默认为DEFAULT_PENCIL_COLOR
     * @param back_color 背景颜色，默认为DEFAULT_BACK_COLOR
     */
    void show_String(uint16_t x, uint16_t y, const char *str,
                     uint16_t string_width = 0,
                     uint16_t string_height = 0,
                     LCD_FontSize_TypeDef size = DEFAULT_FONT_SIZE,
                     LCD_Color_TypeDef color = DEFAULT_PENCIL_COLOR,
                     LCD_Color_TypeDef back_color = DEFAULT_BACK_COLOR);
    /**
     * @brief 在LCD屏幕上显示一幅图像
     *
     * Image2Lcd取模方式：	C语言数据/水平扫描/16位真彩色(RGB565)/高位在前		其他的不要选
     *
     * @param x 图像左上角x坐标
     * @param y 图像左上角y坐标
     * @param Image_width 图像宽度
     * @param Image_height 图像高度
     * @param Image 图像数据
     */
    void show_Image(uint16_t x, uint16_t y, uint16_t Image_width, uint16_t Image_height, const uint8_t *Image);


    //LCD支持流操作
    /**
     * @brief 重载操作符<<，用于将字符串输出到LCD屏幕
     * @param pStr 要输出的字符串
     * @return LCD_SPI& 返回LCD_SPI对象的引用
     */
    LCD_SPI& operator <<(const char* pStr);
    /**
     * @brief 重载操作符<<，用于换行输出
     * @param e 换行对象
     * @return LCD_SPI& 返回LCD_SPI对象的引用
     */
    LCD_SPI& operator <<(STM32::ENDL& e);
    /**
     * @brief 重载操作符<<，用于清屏
     * @param c 清屏对象
     * @return LCD_SPI& 返回LCD_SPI对象的引用
     */
    LCD_SPI& operator <<(STM32::CLEAR& c);
    /**
     * @brief 重载操作符<<，用于将字符输出到LCD屏幕
     * @param charactor 要输出的字符
     * @return LCD_SPI& 返回LCD_SPI对象的引用
     */
    LCD_SPI& operator <<(const char charactor);
    /**
     * @brief 重载操作符<<，用于将整数输出到LCD屏幕
     * @param val 要输出的整数
     * @return LCD_SPI& 返回LCD_SPI对象的引用
     */
    LCD_SPI& operator <<(int val);
    /**
     * @brief 重载操作符<<，用于将浮点数输出到LCD屏幕
     *
     * 修改宏LCD_STREAM_FLOAT_PRECISION，可以扩展小数显示位数
     *
     * @param val 要输出的浮点数
     * @return LCD_SPI& 返回LCD_SPI对象的引用
     */
    LCD_SPI& operator <<(double val);

};

__STATIC_INLINE void LCD_SPI_SendBytes(const uint8_t* data, uint16_t size)
{
    SPI_SendBytes(LCD_SPI_HANDEL, data, size);
}

__STATIC_INLINE void LCD_SPI_SendByte(const uint8_t data)
{
    SPI_SendByte(LCD_SPI_HANDEL, data);
}


__INLINE void LCD_SPI::writeCmd(uint8_t cmd)
{
    gpio_dc.low();
    LCD_SPI_SendByte(cmd);
}

__INLINE void LCD_SPI::writeData(uint8_t data)
{
    gpio_dc.high();
    LCD_SPI_SendByte(data);
}

__INLINE void LCD_SPI::writeDataHalfWord(uint16_t data)
{
    uint8_t buffer[2] = {0};
    buffer[0] = data >> 8;
    buffer[1] = data;
    gpio_dc.high();
    LCD_SPI_SendBytes(buffer, 2);
}

__INLINE void LCD_SPI::draw_Point(uint16_t x, uint16_t y, LCD_Color_TypeDef color)
{
    setAddress(x, y, x, y);
    writeDataHalfWord(color);
}

__INLINE void LCD_SPI::clear(LCD_Color_TypeDef color) {
    fill(0, 0, width, height, color);
}


#endif //__HARDWARE_LCD_SPI_H__
