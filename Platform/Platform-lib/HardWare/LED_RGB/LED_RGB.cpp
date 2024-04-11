#include "platform-lib/HardWare/LED_RGB/LED_RGB.hpp"
#include "platform-lib/System/callInitFunctions.hpp"
#include "Machine.hpp"

void LED_RGB::off(void) {
    current_led_value[__INDEX_LED_R] = !default_led_value;
    current_led_value[__INDEX_LED_G] = !default_led_value;
    current_led_value[__INDEX_LED_B] = !default_led_value;

    Led_R.setValue(current_led_value[__INDEX_LED_R]);
    Led_G.setValue(current_led_value[__INDEX_LED_G]);
    Led_B.setValue(current_led_value[__INDEX_LED_B]);
}

void LED_RGB::red(void) {
    current_led_value[__INDEX_LED_R] = default_led_value;
    current_led_value[__INDEX_LED_G] = !default_led_value;
    current_led_value[__INDEX_LED_B] = !default_led_value;

    Led_R.setValue(current_led_value[__INDEX_LED_R]);
    Led_G.setValue(current_led_value[__INDEX_LED_G]);
    Led_B.setValue(current_led_value[__INDEX_LED_B]);
}

void LED_RGB::green(void) {
    current_led_value[__INDEX_LED_R] = !default_led_value;
    current_led_value[__INDEX_LED_G] = default_led_value;
    current_led_value[__INDEX_LED_B] = !default_led_value;

    Led_R.setValue(current_led_value[__INDEX_LED_R]);
    Led_G.setValue(current_led_value[__INDEX_LED_G]);
    Led_B.setValue(current_led_value[__INDEX_LED_B]);
}

void LED_RGB::blue(void) {
    current_led_value[__INDEX_LED_R] = !default_led_value;
    current_led_value[__INDEX_LED_G] = !default_led_value;
    current_led_value[__INDEX_LED_B] = default_led_value;

    Led_R.setValue(current_led_value[__INDEX_LED_R]);
    Led_G.setValue(current_led_value[__INDEX_LED_G]);
    Led_B.setValue(current_led_value[__INDEX_LED_B]);
}

void LED_RGB::yellow(void) {
    current_led_value[__INDEX_LED_R] = default_led_value;
    current_led_value[__INDEX_LED_G] = default_led_value;
    current_led_value[__INDEX_LED_B] = !default_led_value;

    Led_R.setValue(current_led_value[__INDEX_LED_R]);
    Led_G.setValue(current_led_value[__INDEX_LED_G]);
    Led_B.setValue(current_led_value[__INDEX_LED_B]);
}

void LED_RGB::cyan(void) {
    current_led_value[__INDEX_LED_R] = !default_led_value;
    current_led_value[__INDEX_LED_G] = default_led_value;
    current_led_value[__INDEX_LED_B] = default_led_value;

    Led_R.setValue(current_led_value[__INDEX_LED_R]);
    Led_G.setValue(current_led_value[__INDEX_LED_G]);
    Led_B.setValue(current_led_value[__INDEX_LED_B]);
}

void LED_RGB::pinkishRed(void) {
    current_led_value[__INDEX_LED_R] = default_led_value;
    current_led_value[__INDEX_LED_G] = !default_led_value;
    current_led_value[__INDEX_LED_B] = default_led_value;

    Led_R.setValue(current_led_value[__INDEX_LED_R]);
    Led_G.setValue(current_led_value[__INDEX_LED_G]);
    Led_B.setValue(current_led_value[__INDEX_LED_B]);
}

void LED_RGB::white(void) {
    current_led_value[__INDEX_LED_R] = default_led_value;
    current_led_value[__INDEX_LED_G] = default_led_value;
    current_led_value[__INDEX_LED_B] = default_led_value;

    Led_R.setValue(current_led_value[__INDEX_LED_R]);
    Led_G.setValue(current_led_value[__INDEX_LED_G]);
    Led_B.setValue(current_led_value[__INDEX_LED_B]);
}

void LED_RGB::toggle(void) {
    if (!toggle_flag)
    {
        Led_R.setValue(!default_led_value);
        Led_G.setValue(!default_led_value);
        Led_B.setValue(!default_led_value);
    }
    else
    {
        Led_R.setValue(current_led_value[__INDEX_LED_R]);
        Led_G.setValue(current_led_value[__INDEX_LED_G]);
        Led_B.setValue(current_led_value[__INDEX_LED_B]);
    }
    toggle_flag = !toggle_flag;
}


LED_RGB led;

INIT_FUNCTION(LED_RGB_INIT)
{
    led = LED_RGB(	STM32::GPIO(LED_RED_PORT, LED_RED_PIN, STM32::pullno,  STM32::out),
							STM32::GPIO(LED_GREEN_PORT, LED_GREEN_PIN,STM32::pullno,   STM32::out),
							STM32::GPIO(LED_BLUE_PORT, LED_BLUE_PIN,STM32::pullno,   STM32::out));
}
