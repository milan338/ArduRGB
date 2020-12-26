#ifndef RAINBOWCYCLE_H
#define RAINBOWCYCLE_H

#include <FastLED.h>

class RainbowCycle
{
public:
    static void run(CRGBSet &led_array, uint8_t led_hue[], uint32_t led_num, bool &effect_setup, uint32_t effect_delay, uint32_t &time_now);
};

#endif // RAINBOWCYCLE_H
