// This file is part of ArduRGB, an app to control lighting devices.
// Copyright (C) 2021 Milan Sorak.
//
// ArduRGB is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// ArduRGB is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with ArduRGB.  If not, see <https://www.gnu.org/licenses/>.

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
    uint32_t led_num;
    uint32_t current_mode;
    uint32_t previous_mode;
    uint32_t current_time;
    uint32_t refresh_delay;
    bool effect_setup;
} LEDDict;

#endif // STRUCTS_H
