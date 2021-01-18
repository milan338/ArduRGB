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
#include <FastLED.h>
#include "led_util.h"
#include "fade_black.h"

void FadeBlack::run(CRGBSet &led_array, uint8_t hue_array[], uint32_t led_num, uint32_t &current_mode, bool &effect_setup)
{
    // Only run once
    if (effect_setup)
    {
        hue_array[0] = 0;
        effect_setup = false;
    }
    // Main sequence
    if (hue_array[0] < 256 / FADE_BY)
    {
        // Fade LED strip to black
        _fadeToBlackBy(led_array, led_num, FADE_BY);
        hue_array[0]++;
    }
    // Fading complete
    else
    {
        // Set current mode to none
        current_mode = 0;
        // Ensure all LEDs are off
        _fillSolid(led_array, led_num, CRGB(0, 0, 0));
    }
}
