#ifndef SETBRIGHTNESS_H
#define SETBRIGHTNESS_H

#include <Arduino.h>
#include <FastLED.h>
#include <ReadSerial.h>

void setBrightness(bool &effect_setup, byte &current_mode, byte last_mode, char *argv[]);

#endif // SETBRIGHTNESS_H