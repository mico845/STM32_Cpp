//
// Created by Whisky on 12/29/2023.
//

#include "LED_RGB.hpp"

void LED_RGB::Off(void) {
    current_led_level[__INDEX_LED_R] = !default_led_level;
    current_led_level[__INDEX_LED_G] = !default_led_level;
    current_led_level[__INDEX_LED_B] = !default_led_level;

    Led_R.SetLeveL(current_led_level[__INDEX_LED_R]);
    Led_G.SetLeveL(current_led_level[__INDEX_LED_G]);
    Led_B.SetLeveL(current_led_level[__INDEX_LED_B]);
}

void LED_RGB::Red(void) {
    current_led_level[__INDEX_LED_R] = default_led_level;
    current_led_level[__INDEX_LED_G] = !default_led_level;
    current_led_level[__INDEX_LED_B] = !default_led_level;

    Led_R.SetLeveL(current_led_level[__INDEX_LED_R]);
    Led_G.SetLeveL(current_led_level[__INDEX_LED_G]);
    Led_B.SetLeveL(current_led_level[__INDEX_LED_B]);
}

void LED_RGB::Green(void) {
    current_led_level[__INDEX_LED_R] = !default_led_level;
    current_led_level[__INDEX_LED_G] = default_led_level;
    current_led_level[__INDEX_LED_B] = !default_led_level;

    Led_R.SetLeveL(current_led_level[__INDEX_LED_R]);
    Led_G.SetLeveL(current_led_level[__INDEX_LED_G]);
    Led_B.SetLeveL(current_led_level[__INDEX_LED_B]);
}

void LED_RGB::Blue(void) {
    current_led_level[__INDEX_LED_R] = !default_led_level;
    current_led_level[__INDEX_LED_G] = !default_led_level;
    current_led_level[__INDEX_LED_B] = default_led_level;

    Led_R.SetLeveL(current_led_level[__INDEX_LED_R]);
    Led_G.SetLeveL(current_led_level[__INDEX_LED_G]);
    Led_B.SetLeveL(current_led_level[__INDEX_LED_B]);
}

void LED_RGB::Yellow(void) {
    current_led_level[__INDEX_LED_R] = default_led_level;
    current_led_level[__INDEX_LED_G] = default_led_level;
    current_led_level[__INDEX_LED_B] = !default_led_level;

    Led_R.SetLeveL(current_led_level[__INDEX_LED_R]);
    Led_G.SetLeveL(current_led_level[__INDEX_LED_G]);
    Led_B.SetLeveL(current_led_level[__INDEX_LED_B]);
}

void LED_RGB::Cyan(void) {
    current_led_level[__INDEX_LED_R] = !default_led_level;
    current_led_level[__INDEX_LED_G] = default_led_level;
    current_led_level[__INDEX_LED_B] = default_led_level;

    Led_R.SetLeveL(current_led_level[__INDEX_LED_R]);
    Led_G.SetLeveL(current_led_level[__INDEX_LED_G]);
    Led_B.SetLeveL(current_led_level[__INDEX_LED_B]);
}

void LED_RGB::PinkishRed(void) {
    current_led_level[__INDEX_LED_R] = default_led_level;
    current_led_level[__INDEX_LED_G] = !default_led_level;
    current_led_level[__INDEX_LED_B] = default_led_level;

    Led_R.SetLeveL(current_led_level[__INDEX_LED_R]);
    Led_G.SetLeveL(current_led_level[__INDEX_LED_G]);
    Led_B.SetLeveL(current_led_level[__INDEX_LED_B]);
}

void LED_RGB::White(void) {
    current_led_level[__INDEX_LED_R] = default_led_level;
    current_led_level[__INDEX_LED_G] = default_led_level;
    current_led_level[__INDEX_LED_B] = default_led_level;

    Led_R.SetLeveL(current_led_level[__INDEX_LED_R]);
    Led_G.SetLeveL(current_led_level[__INDEX_LED_G]);
    Led_B.SetLeveL(current_led_level[__INDEX_LED_B]);
}

void LED_RGB::Toggle(void) {
    if (!toggle_flag)
    {
        Led_R.SetLeveL(!default_led_level);
        Led_G.SetLeveL(!default_led_level);
        Led_B.SetLeveL(!default_led_level);
    }
    else
    {
        Led_R.SetLeveL(current_led_level[__INDEX_LED_R]);
        Led_G.SetLeveL(current_led_level[__INDEX_LED_G]);
        Led_B.SetLeveL(current_led_level[__INDEX_LED_B]);
    }
    toggle_flag = !toggle_flag;
}



