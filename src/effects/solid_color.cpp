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
#include "solid_color.h"

void SolidColor::run(CRGBSet &led_array, uint32_t led_num, uint32_t &current_mode, uint8_t led_args[])
{
    // Set current mode to none
    current_mode = 0;
    // Set strip colors
    _fillSolid(led_array, led_num, CRGB(led_args[0], led_args[1], led_args[2]));
}
