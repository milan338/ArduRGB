#ifndef SOLIDCOLOUR_H
#define SOLIDCOLOUR_H

#include <FastLED.h>
#include <serial_Read.h>
#include <definitions.h>

class SolidColor
{
public:
    static void run(CRGBSet &led_array, uint32_t led_num);
};

#endif // SOLIDCOLOUR_H
