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

#ifndef HASH_H
#define HASH_H

#include <Arduino.h>

constexpr inline uint32_t hash(const char *str, uint32_t i = 0)
{
    // Create numerical hash of character array
    return !str[i] ? 5381 : (hash(str, i + 1) * 33) ^ str[i];
}

#endif //HASH_H
