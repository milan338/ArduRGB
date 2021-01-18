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

#ifndef TOGGLELEDS_H
#define TOGGLELEDS_H

#include <Arduino.h>
#include <structs.h>

#define TOGGLELEDS EFFECT

class ToggleLeds
{
public:
    static bool &isOn();
    static void run(LEDDict *strips, uint32_t &current_mode, uint32_t previous_mode);
};

#endif // TOGGLELEDS_H
