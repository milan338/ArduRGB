#ifndef SOLIDCOLOUR_H
#define SOLIDCOLOUR_H

#include <FastLED.h>
#include <serial_Read.h>
#include <definitions.h>

class SolidColor
{
public:
    template <int SIZE>
    static void run(CRGBArray<SIZE> &led_array)
    {
        uint8_t serial_buffer[4];
        if (readSerial(serial_buffer, 4) == 1)
        {
            uint8_t R = serial_buffer[0];
            uint8_t G = serial_buffer[1];
            uint8_t B = serial_buffer[2];
            fill_solid(led_array, SIZE, CRGB(R, G, B));
            FastLED.show();
        }
        else
            Serial.println("Failed to read serial input");
    }
};

#endif // SOLIDCOLOUR_H
