#ifndef SOLIDCOLOR_H
#define SOLIDCOLOR_H

#include <FastLED.h>
#include <serial_Read.h>
#include <definitions.h>

#define SOLIDCOLOR EFFECT

class SolidColor
{
public:
    static void run(CRGBSet &led_array, uint32_t led_num, uint32_t &serial_mode);
};

#endif // SOLIDCOLOR_H
