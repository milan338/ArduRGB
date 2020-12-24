#include <Arduino.h>
#include <FastLED.h>
#include <serial_read.h>
#include <definitions.h>
#include "set_brightness.h"

void setBrightness(bool &effect_setup, uint8_t &current_mode, uint8_t last_mode, char *argv[])
{
    // Don't restart already running effects
    effect_setup = false;
    // Re-run last run effect to refresh with new brightness
    current_mode = last_mode;
    // Set global brightness
    uint8_t serial_buffer[2];
    if (readSerial(serial_buffer, 2) == 1)
    {
        FastLED.setBrightness(serial_buffer[0]);
        FastLED.show();
    }
    else
        Serial.println("Failed to read serial input");
}