#ifndef FADEBLACK_H
#define FADEBLACK_H

#include <Arduino.h>
#include <FastLED.h>

#define FADE_BY 4

template <int SIZE>
void fadeBlack(uint8_t &current_mode, CRGBArray<SIZE> &led_array)
{
    // Set current mode to none
    current_mode = 0;
    // Fade LED strip to black
    for (uint8_t i = 0; i < 256 / FADE_BY; i++)
    {
        fadeToBlackBy(led_array, SIZE, FADE_BY);
        FastLED.show();
    }
    // Ensure all LEDs cleared
    fill_solid(led_array, SIZE, CRGB(0, 0, 0));
    FastLED.show();
}

#endif // FADEBLACK_H