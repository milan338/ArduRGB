#ifndef RAINBOWCYCLE_H
#define RAINBOWCYCLE_H

#include <FastLED.h>

template <int SIZE>
void cycle(uint32_t delay, uint32_t &time_now, CRGBArray<SIZE> &led_array, uint8_t hue[], bool &setup)
{
    if (setup)
    {
        // Set initial colours that will be shifted through the main sequence
        for (int i = 0; i < SIZE; i++)
            hue[i] = 255 / SIZE * i;
        setup = false;
    }
    // Control speed of effect
    if (millis() - delay >= time_now)
    {
        time_now = millis();
        // Main sequence
        for (int i = 0; i < SIZE; i++)
            led_array[i] = CHSV(hue[i]--, 255, 255);
    }
}

#endif // RAINBOWCYCLE_H
