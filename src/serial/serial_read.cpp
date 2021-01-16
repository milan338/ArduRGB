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
#include <definitions.h>
#include <user_definitions.h>
#include <char_hash.h>
#include <structs.h>
#include "serial_read.h"

SerialRead::SerialRead(bool &reading_message, LEDDict *strips)
{
    this->reading_message = reading_message;
    this->strips = strips;
}

void SerialRead::clearSerial()
{
    // Clear available serial input
    while (Serial.available())
    {
        Serial.read();
    }
    // Ready to read new message
    reading_message = false;
    serial_counter = 0;
}

void SerialRead::readSerial()
{
    // Read one message byte
    if (Serial.available())
    {
        int serial_input = Serial.read();
        Serial.println(serial_input);
        // Handle invalid reads
        if (serial_input != -1)
        {
            // Beginning of message
            if (serial_input == SERIAL_BEGIN && !reading_message)
            {
                // Reset serial read states
                reading_message = true;
                // Clear input buffers
                memset(serial_mode_input, 0, sizeof(serial_mode_input));
                memset(serial_args_input, 0, sizeof(serial_args_input));
                serial_counter = 0;
            }
            // End of message
            else if (serial_input == SERIAL_TERMINATE && reading_message)
            {
                clearSerial();
                // Setup effect before looping
                strips[current_strip].effect_setup = true;
                // Apply effect
                strips[current_strip].previous_mode = strips[current_strip].current_mode;
                strips[current_strip].current_mode = hash(serial_mode_input);
                // Apply new effect arguments
                for (uint8_t i = 0; i < ARGS_NUM; i++)
                    strips[current_strip].led_args[i] = serial_args_input[i];
            }
            // Inside message contents
            else if (reading_message)
            {
                switch (serial_counter)
                {
                // Get selected strip
                case 0:
                    // Ensure strip selected is valid
                    if (serial_input < STRIP_NUM && serial_input >= 0)
                    {
                        // Set currently selected strip
                        current_strip = serial_input;
                    }
                    else
                    {
                        clearSerial();
                        Serial.print(F("Invalid LED strip selected: "));
                        Serial.println(serial_input);
                    }
                    break;
                // Get selected mode
                case 1:
                    // Store char array input as current mode
                    if (Serial.readBytes(serial_mode_input, serial_input) == (uint32_t)serial_input)
                    {
                        // Add terminating byte
                        serial_mode_input[serial_input] = '\0';
                    }
                    else
                    {
                        clearSerial();
                        Serial.print(F("Char input stream not of expected size: "));
                        Serial.println(serial_input);
                    }
                    break;
                case 2:
                    // Store effect args
                    if (Serial.readBytes(serial_args_input, serial_input) == (uint32_t)serial_input)
                    {
                        // Add terminating byte
                        serial_args_input[serial_input] = '\0';
                    }
                    else
                    {
                        clearSerial();
                        Serial.print(F("Arguments input stream not of expected size: "));
                        Serial.println(serial_input);
                    }
                    break;
                }
                // Read next part of message
                serial_counter++;
            }
            // Remove serial input not part of valid message
            else
                clearSerial();
        }
    }
    // Handle serial delay
    else if (serial_counter > 1)
    {
        // Wait for serial
        time_now = millis();
        while (millis() - time_now < SERIAL_TIMEOUT)
        {
            if (Serial.available())
                break;
        }
        // Terminate byte not sent
        if (!Serial.available())
        {
            clearSerial();
            Serial.println(F("Did not receive expected terminate byte"));
        }
    }
}
