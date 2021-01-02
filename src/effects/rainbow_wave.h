#ifndef RAINBOWWAVE_H
#define RAINBOWWAVE_H

#include <FastLED.h>

#define RAINBOWWAVE EFFECT

class RainbowWave
{
public:
    static void run(CRGBSet &led_array, uint8_t hue_array[], uint32_t led_num, bool &effect_setup, uint32_t refresh_delay, uint32_t &current_time);
};

#endif // RAINBOWWAVE_H
