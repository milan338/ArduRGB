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
#include "fade_black.h"

void FadeBlack::run(CRGBSet &led_array, uint32_t led_num, uint32_t &current_mode)
{
    // Set current mode to none
    current_mode = 0;
    // Fade LED strip to black
    for (uint8_t i = 0; i < 256 / FADE_BY; i++)
    {
        fadeToBlackBy(led_array, led_num, FADE_BY);
        FastLED.show();
    }
    // Ensure all LEDs cleared
    fill_solid(led_array, led_num, CRGB(0, 0, 0));
    FastLED.show();
}