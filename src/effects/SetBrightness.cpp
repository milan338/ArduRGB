#include <Arduino.h>
#include <FastLED.h>
#include <ReadSerial.h>
#include "SetBrightness.h"

void setBrightness(bool &effect_setup, byte &current_mode, byte last_mode, char *argv[])
{
    // Don't restart already running effects
    effect_setup = false;
    // Re-run last run effect to refresh with new brightness
    current_mode = last_mode;
    // Set global brightness
    int brightness = readSerial();
    if (brightness != -1)
    {
        FastLED.setBrightness(brightness);
        FastLED.show();
    }
    else
    {
        Serial.println("Failed to read serial input");
    }
}