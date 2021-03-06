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

#ifndef RAINBOWCYCLE_H
#define RAINBOWCYCLE_H

#include <FastLED.h>

#define RAINBOWCYCLE EFFECT

class RainbowCycle
{
public:
    static void run(CRGBSet &led_array, uint8_t hue_array[], uint32_t led_num, bool &effect_setup, uint32_t refresh_delay, uint32_t &current_time);
};

#endif // RAINBOWCYCLE_H
