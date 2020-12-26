#ifndef FADEBLACK_H
#define FADEBLACK_H

#include <Arduino.h>
#include <FastLED.h>

#define FADE_BY 4

class FadeBlack
{
public:
    static void run(CRGBSet &led_array, uint32_t led_num, uint8_t &serial_mode);
};

#endif // FADEBLACK_H