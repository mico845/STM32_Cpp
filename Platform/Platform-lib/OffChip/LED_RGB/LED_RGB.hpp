//
// Created by Whisky on 12/29/2023.
//

#ifndef CODE_LED_RGB_H
#define CODE_LED_RGB_H

#include "OnChip/GPIO/GPIO.hpp"

class LED_RGB
{
private:
    #define __INDEX_LED_R   (0)
    #define __INDEX_LED_G   (1)
    #define __INDEX_LED_B   (2)
    bool default_led_level;
    bool current_led_level[3];
    bool toggle_flag = false;
public:
    /* Variables */
    GPIO Led_R;
    GPIO Led_G;
    GPIO Led_B;
    /* Constructor */
    LED_RGB() = default;
    ~LED_RGB() = default;

    LED_RGB(GPIO LED_R, GPIO LED_G, GPIO LED_B, bool Led_Level = 0)
                    :   default_led_level(Led_Level),
                        Led_R(LED_R),
                        Led_G(LED_G),
                        Led_B(LED_B)
                    { }
    /* Functions */

    void Off(void);
    void Red(void);
    void Green(void);
    void Blue(void);
    void Yellow(void);
    void Cyan(void);
    void PinkishRed(void);
    void White(void);

    void Toggle(void);
};

#endif //CODE_LED_RGB_H
