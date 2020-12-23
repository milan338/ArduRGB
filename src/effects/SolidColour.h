#ifndef SOLIDCOLOUR_H
#define SOLIDCOLOUR_H

#include <FastLED.h>
#include "serial/ReadSerial.h"

template <int SIZE>
void setColour(uint16_t led_num, CRGBArray<SIZE> &led_array, uint16_t timeout, uint8_t &R, uint8_t &G, uint8_t &B)
{
    for (uint8_t i = 0; i < 2; i++)
    {
        int col = readSerial(timeout);
        if (col >= 0)
        {
            switch (i)
            {
            case 0:
                R = readSerial(timeout);
            case 1:
                G = readSerial(timeout);
            case 2:
                B = readSerial(timeout);
            }
        }
        else
        {
            R = 0;
            G = 0;
            B = 0;
            // Debug
            Serial.println("timed out waiting for response");
        }
    }
    fill_solid(led_array, led_num, CRGB(R, G, B));
}

#endif //SOLIDCOLOUR_H
