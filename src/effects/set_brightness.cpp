#include <Arduino.h>
#include <FastLED.h>
#include <serial_read.h>
#include "set_brightness.h"

void SetBrightness::run(uint32_t &serial_mode, uint32_t last_mode, bool &effect_setup)
{
    // Don't restart already running effects
    effect_setup = false;
    // Re-run last run effect to refresh with new brightness
    serial_mode = last_mode;
    // Set global brightness
    uint8_t serial_buffer[2];
    if (readSerial(serial_buffer, 2) == 1)
    {
        FastLED.setBrightness(serial_buffer[0]);
        FastLED.show();
    }
    else
        Serial.println(F("Failed to read serial input"));
}