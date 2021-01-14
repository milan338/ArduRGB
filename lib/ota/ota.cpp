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

#ifdef OTA

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <user_definitions.h>
#include "ota.h"

void initOTA()
{
    ArduinoOTA.setPassword(OTA_PASSWORD);
    ArduinoOTA.onStart([]() {
        Serial.println(F("Start updating"));
    });
    ArduinoOTA.onEnd([]() {
        Serial.println(F("\nEnd"));
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR)
        {
            Serial.println(F("Auth Failed"));
        }
        else if (error == OTA_BEGIN_ERROR)
        {
            Serial.println(F("Begin Failed"));
        }
        else if (error == OTA_CONNECT_ERROR)
        {
            Serial.println(F("Connect Failed"));
        }
        else if (error == OTA_RECEIVE_ERROR)
        {
            Serial.println(F("Receive Failed"));
        }
        else if (error == OTA_END_ERROR)
        {
            Serial.println(F("End Failed"));
        }
    });
    ArduinoOTA.begin();
    Serial.println(F("Ready"));
}

void handleOTA()
{
    ArduinoOTA.handle();
}

#endif
