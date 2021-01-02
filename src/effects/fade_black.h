#ifndef FADEBLACK_H
#define FADEBLACK_H

#include <Arduino.h>
#include <FastLED.h>
#include <definitions.h>

#define FADEBLACK EFFECT

class FadeBlack
{
public:
    static void run(CRGBSet &led_array, uint32_t led_num, uint32_t &current_mode);
};

#endif // FADEBLACK_H