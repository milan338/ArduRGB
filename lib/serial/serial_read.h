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

#ifndef SERIALREAD_H
#define SERIALREAD_H

#include <Arduino.h>
#include <definitions.h>
#include <structs.h>

class SerialRead
{
public:
    SerialRead(bool &reading_message, LEDDict *strips);
    void clearSerial();
    void readSerial();

private:
    // Store all relevant information about each LED strip
    LEDDict *strips;
    // Determine whether to read a message or refresh effects
    bool reading_message;
    // Store mode from serial
    char serial_mode_input[EFFECT_NAME_CHARS];
    // Temporarily store effect args
    uint8_t serial_args_input[ARGS_NUM];
    // Store currently selected strip
    uint8_t current_strip = 0;
    // Store current message element to read
    uint8_t serial_counter = 0;
    // Store time for non-blocking delays
    uint32_t time_now = 0;
};

#endif // SERIALREAD_H
