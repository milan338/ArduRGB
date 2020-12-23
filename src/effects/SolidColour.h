#ifndef SOLIDCOLOUR_H
#define SOLIDCOLOUR_H

#include <FastLED.h>
// #include "serial/ReadSerial.h"

template <int SIZE>
void setColour(uint16_t led_num, CRGBArray<SIZE> &led_array)
{
    uint8_t R = 0;
    uint8_t G = 0;
    uint8_t B = 0;

    for (uint8_t i = 0; i < 3; i++)
    {
        switch (i)
        {
        case 0:
            R = Serial.read();
        case 1:
            G = Serial.read();
        case 2:
            B = Serial.read();
        }
    }
    fill_solid(led_array, led_num, CRGB(R, G, B));
}

#endif //SOLIDCOLOUR_H
