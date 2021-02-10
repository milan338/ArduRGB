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

#include <FastLED.h>
#include "led_util.h"

void _fillSolid(CRGBSet &led_array, uint32_t led_num, CRGB colors)
{
    for (uint32_t i = 0; i < led_num; i++)
    {
        led_array[i] = colors;
    }
}

void _fadeToBlackBy(CRGBSet &led_array, uint32_t led_num, uint8_t fadeby)
{
    _nscale8(led_array, led_num, 255 - fadeby);
}

void _nscale8(CRGBSet &led_array, uint32_t led_num, uint8_t scale)
{
    for (uint32_t i = 0; i < led_num; i++)
    {
        led_array[i].nscale8(scale);
    }
}
