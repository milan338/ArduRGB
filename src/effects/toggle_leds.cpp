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

#include <Arduino.h>
#include <structs.h>
#include <user_definitions.h>
#include "eepromio.h"
#include "fade_black.h"
#include "toggle_leds.h"

void ToggleLeds::run(LEDDict *strips, uint32_t &current_mode, uint32_t previous_mode)
{
    // Set initial state as 'on'
    static uint8_t is_on = true;
    // Set current mode for selected strip as previous mode
    current_mode = previous_mode;
    // Turn off sequence
    if (is_on)
    {
        // Save current state to EEPROM
        setEeprom(strips);
        // Set all strips to effect setup state for fading to black
        for (uint8_t i = 0; i < STRIP_NUM; i++)
            strips[i].effect_setup = true;
        // Fade all strips to black
        for (uint8_t i = 0; i <= 256 / FADE_BY; i++)
        {
            for (uint8_t j = 0; j < STRIP_NUM; j++)
            {
                FadeBlack::run(strips[j].led_array, strips[j].hue_array, strips[j].led_num, strips[j].current_mode, strips[j].effect_setup);
            }
            FastLED.show();
        }
        // Reset current effects
        for (uint8_t i = 0; i < STRIP_NUM; i++)
        {
            strips[i].effect_setup = true;
            strips[i].previous_mode = strips[i].current_mode;
            strips[i].current_mode = 0;
        }
    }
    // Turn on sequence
    else
    {
        // Restore last state
        getEeprom(strips);
    }
    is_on = !is_on;
}
