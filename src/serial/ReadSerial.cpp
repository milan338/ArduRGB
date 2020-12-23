#include <Arduino.h>
#include "ReadSerial.h"

#define TIMEOUT_MS 2000

int readSerial()
{
    if (!Serial.available())
    {
        uint32_t time_now_ms = millis();
        while (millis() - time_now_ms < TIMEOUT_MS)
        {
            if (Serial.available())
                return Serial.read();
        }
    }
    return -1;
}