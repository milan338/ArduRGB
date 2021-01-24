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

// Global Definitions
#define FADE_BY 4            // Controls fade speed between colours
#define EFFECT_NAME_CHARS 16 // Number of chars allowed for effect mode - 1
#define ARGS_NUM 8           // Number of additional effect arguments allowed - 1

// SERIAL
#define SERIAL_BEGIN 254     // Message start byte
#define SERIAL_TERMINATE 255 // Message end byte
#define SERIAL_TIMEOUT 1000  // Serial timeout in ms
