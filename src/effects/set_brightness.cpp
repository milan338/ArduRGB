#include <Arduino.h>
#include <FastLED.h>
#include <serial_read.h>
#include "set_brightness.h"

void SetBrightness::run(uint32_t &current_mode, uint32_t previous_mode, bool &effect_setup, uint8_t led_args[])
{
    // Don't restart already running effects
    effect_setup = false;
    // Re-run last run effect to refresh with new brightness
    current_mode = previous_mode;
    // Set global brightness
    FastLED.setBrightness(led_args[0]);
    FastLED.show();
}