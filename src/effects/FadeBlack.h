#ifndef FADEBLACK_H
#define FADEBLACK_H

#include <Arduino.h>
#include <FastLED.h>

#define FADE_BY 4

template <int SIZE>
void fadeBlack(byte &current_mode, CRGBArray<SIZE> &led_array)
{
    // Set current mode to none
    current_mode = 0;
    // Fade LED strip to black
    for (int i = 0; i < 256 / FADE_BY; i++)
    {
        fadeToBlackBy(led_array, SIZE, FADE_BY);
        FastLED.show();
    }
    // Clear LED strip
    FastLED.clear(true);
}

#endif // FADEBLACK_H