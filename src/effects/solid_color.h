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

#ifndef SOLIDCOLOR_H
#define SOLIDCOLOR_H

#include <FastLED.h>
#include <definitions.h>

#define SOLIDCOLOR EFFECT

class SolidColor
{
public:
    static void run(CRGBSet &led_array, uint32_t led_num, uint8_t led_args[], bool &effect_setup);
};

#endif // SOLIDCOLOR_H
