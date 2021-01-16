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
#include <user_definitions.h>
#include <char_hash.h>
#include <structs.h>
// Optional dependencies
#ifdef OTA
#define HANDLE_WIFI
#include <ota.h>
#endif
#ifdef HANDLE_WIFI
#include <wifi_handler.h>
#endif

// ---------- Effects ---------- //
/*
#include "effects/effect.h"
*/
// ---------- Effects ---------- //

// ---------- Defines ---------- //
#define DEFAULT_BRIGHTNESS 50
// ---------- Defines ---------- //

// ---------- LED Definitions ---------- //
/*
// Physical LED strip arrays from which virtual CRGBSets are created
CRGBArray<STRIP_PHYSICAL_LEDS_0> strip_0;

// Array for each led and associated hue values for use in effects
CRGBSet led_array_0(strip_0(LED_NUM_0 - 1, 0));
uint8_t led_hues_0[LED_NUM_0];
uint8_t led_args_0[ARGS_NUM];

*/
// Store all relevant information about each LED strip
LEDDict strips[]{
    /*{led_array_0, led_hues_0, led_args_0, LED_NUM_0, 0, 0, 0, LED_DELAY_0, false}*/};
// ---------- LED Definitions ---------- //

// Determine whether to read a message or refresh effects
bool reading_message = false;

// Create object of message type used to read input
#ifdef _SERIAL
#include "serial/serial_read.h"
SerialRead serial_read = SerialRead(reading_message, strips);
#endif

void setup()
{
  // Power-up safety delay
  delay(2000);
  // ---------- Strip Init ---------- //
  /*
  FastLED.addLeds<STRIP_PHYSICAL_TYPE_0, STRIP_PHYSICAL_PIN_0, STRIP_PHYSICAL_ORDER_0>(strip_0, STRIP_PHYSICAL_LEDS_0);
  */
  // ---------- Strip Init ---------- //
  // Begin serial
  Serial.begin(BAUDRATE);
  Serial.setTimeout(SERIAL_TIMEOUT);
  // Recovery delay
  delay(1);
// Begin Wi-Fi handler
#ifdef HANDLE_WIFI
  beginWifi();
#endif
// Begin OTA handler
#ifdef OTA
  initOTA();
#endif
  // Set global LED brightness
  FastLED.setBrightness(DEFAULT_BRIGHTNESS);
}

// ---------- Run Effects ---------- //
void runEffect()
{
  /*
  for (uint8_t i = 0; i < STRIP_NUM; i++)
  {
    switch (strips[i].current_mode)
    {
    case hash("effectname"):
      EffectClass::run(strips[i].arg);
      break;
    }
  }
  */
  ;
}
// ---------- Run Effects ---------- //

void loop()
{
  // Handle OTA
#ifdef OTA
  handleOTA();
#endif
// Read message through defined input type
#ifdef _SERIAL
  serial_read.readSerial();
#endif
  // Not reading message
  if (!reading_message)
  {
    // Update LED strips
    runEffect();
    FastLED.show();
  }
}
