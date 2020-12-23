#include <Arduino.h>
#include "ReadSerial.h"

int readSerial(uint16_t timeout)
{
    if (!Serial.available())
    {
        uint16_t time = millis();
        while (timeout - time > 0)
        {
            if (Serial.available())
                return Serial.read();
            else
                time = millis();
        }
    }
    if (Serial.available())
        return Serial.read();
    else
        return -1;
}