#include <FastLED.h>
#include <definitions.h>
#include "solid_color.h"

void SolidColor::run(CRGBSet &led_array, uint32_t led_num, uint32_t &current_mode, uint8_t led_args[])
{
    // Set current mode to none
    current_mode = 0;
    // Set strip colors
    fill_solid(led_array, led_num, CRGB(led_args[0], led_args[1], led_args[2]));
    FastLED.show();
}