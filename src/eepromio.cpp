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
#include <EEPROM.h>
#include <structs.h>
#include <definitions.h>
#include <user_definitions.h>
#include "eepromio.h"

bool updateEeprom(uint16_t &address)
{
    // Return true if number of leds changed
    uint8_t num_strips = EEPROM.read(address);
    address++;
    if (num_strips != STRIP_NUM)
    {
        EEPROM.put(address - 1, STRIP_NUM);
        return true;
    }
    else
        return false;
}

void getEeprom(LEDDict *strips)
{
    Serial.print(F("Reading data from EEPROM"));
    // Start at EEPROM address 0
    uint16_t address = 0;
    // Get last effect and effect args for each strip
    for (uint8_t i = 0; i < STRIP_NUM; i++)
    {
        // Get last strip mode
        EEPROM.get(address, strips[i].current_mode);
        address += sizeof(strips->current_mode);
        // Get last strip mode args
        uint8_t j = 0;
        do
        {
            Serial.print(".");
            EEPROM.get(address, strips[i].led_args[j]);
            address += sizeof(strips->led_args[0]);
            j++;
        } while (strips[i].led_args[j - 1] != '\0');
    }
    Serial.println(F("\nData read from EEPROM"));
}

void setEeprom(LEDDict *strips)
{
    Serial.print(F("Writing data to EEPROM"));
    // Start at EEPROM address 0
    uint16_t address = 0;
    // Set last effect and effect args for each strip
    for (uint8_t i = 0; i < STRIP_NUM; i++)
    {
        // Check if effect has stopped itself - set current effect to 0
        if (!strips[i].current_mode && strips[i].previous_mode)
            strips[i].current_mode = strips[i].previous_mode;
        // Set last strip mode
        EEPROM.put(address, strips[i].current_mode);
        address += sizeof(strips->current_mode);
        // Set last strip mode args
        uint8_t j = 0;
        do
        {
            Serial.print(".");
            EEPROM.put(address, strips[i].led_args[j]);
            address += sizeof(strips->led_args[0]);
            j++;
            // Stop reading at null-terminator and handle missing terminator
        } while (strips[i].led_args[j - 1] && j < ARGS_NUM);
    }
    // Commit EEPROM changes
#ifdef ESP_EEPROM
    EEPROM.commit();
#endif
    Serial.println(F("\nData written to EEPROM"));
}
