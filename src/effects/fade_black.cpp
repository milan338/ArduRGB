#include <Arduino.h>
#include <FastLED.h>
#include "fade_black.h"

void FadeBlack::run(CRGBSet &led_array, uint32_t led_num, uint8_t &serial_mode)
{
    // Set current mode to none
    serial_mode = 0;
    // Fade LED strip to black
    for (uint8_t i = 0; i < 256 / FADE_BY; i++)
    {
        fadeToBlackBy(led_array, led_num, FADE_BY);
        FastLED.show();
    }
    // Ensure all LEDs cleared
    fill_solid(led_array, led_num, CRGB(0, 0, 0));
    FastLED.show();
}