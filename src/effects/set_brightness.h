#ifndef SETBRIGHTNESS_H
#define SETBRIGHTNESS_H

#include <Arduino.h>
#include <FastLED.h>
#include <serial_read.h>

class SetBrightness
{
public:
    static void run(byte &serial_mode, byte last_mode, bool &effect_setup);
};

#endif // SETBRIGHTNESS_H