#ifndef STRUCTS_H
#define STRUCTS_H

#include <Arduino.h>
#include <FastLED.h>

// Store all relevant information about each LED strip
typedef struct LEDDict
{
    CRGBSet led_array;
    uint8_t *hue_array;
    uint8_t *led_args;
    uint16_t led_num;
    uint32_t current_mode;
    uint32_t previous_mode;
    uint32_t current_time;
    uint32_t refresh_delay;
    bool effect_setup;
} LEDDict;

#endif // STRUCTS_H