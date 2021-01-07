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
#include <definitions.h>
#include <led_strips.h>
#include <char_hash.h>
#include <structs.h>
#include <serial_read.h>

// ---------- Effects ---------- //
// ---------- Effects ---------- //

#define DEFAULT_BRIGHTNESS 50 // TODO replace with storing last brightness and if not found default to 50

// ---------- LED Definitions ---------- //
// Store all relevant information about each LED strip
LEDDict strips[]{}; // TEMPORARY
// ---------- LED Definitions ---------- //

// Determine whether to read a message or refresh effects
bool reading_message = false;

// Create object of message type used to read input
#if MESSAGE_TYPE == _SERIAL
SerialRead serial_read = SerialRead(reading_message, strips);
#endif

void setup()
{
  // Power-up safety delay
  delay(2000);
  // ---------- Strip Init ---------- //
  // ---------- Strip Init ---------- //
  // Begin serial
  Serial.begin(BAUDRATE);
  Serial.setTimeout(SERIAL_TIMEOUT);
  // Recovery delay
  delay(1);
  FastLED.setBrightness(DEFAULT_BRIGHTNESS);
}

// ---------- Run Effects ---------- //
void runEffect() { ; }; // TEMPORARY
// ---------- Run Effects ---------- //

void loop()
{
  // Read message through defined input type
#if MESSAGE_TYPE == _SERIAL
  serial_read.readSerial();
#endif
  // Not reading message
  if (!reading_message)
  {
    // Update LED strips
    runEffect();
  }
}
