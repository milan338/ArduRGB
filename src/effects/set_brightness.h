#ifndef SETBRIGHTNESS_H
#define SETBRIGHTNESS_H

#include <Arduino.h>
#include <FastLED.h>
#include <serial_read.h>

class SetBrightness
{
public:
    static void run(bool &effect_setup, byte &current_mode, byte last_mode, char *argv[]);
};

#endif // SETBRIGHTNESS_H