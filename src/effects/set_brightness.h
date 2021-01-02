#ifndef SETBRIGHTNESS_H
#define SETBRIGHTNESS_H

#include <Arduino.h>
#include <FastLED.h>
#include <serial_read.h>

#define SETBRIGHT EFFECT

class SetBrightness
{
public:
    static void run(uint32_t &current_mode, uint32_t previous_mode, bool &effect_setup, uint8_t led_args[]);
};

#endif // SETBRIGHTNESS_H