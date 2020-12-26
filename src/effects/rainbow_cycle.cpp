#include <FastLED.h>
#include "rainbow_cycle.h"

void RainbowCycle::run(CRGBSet &led_array, uint8_t led_hue[], uint32_t led_num, bool &effect_setup, uint32_t effect_delay, uint32_t &time_now)
{
    if (effect_setup)
    {
        // Set initial colours that will be shifted through the main sequence
        for (uint32_t i = 0; i < led_num; i++)
            // led_hue[i + led_index] = 255 / led_num * i;
            led_hue[i] = 255 / led_num * i;
        effect_setup = false;
    }
    // Control speed of effect
    if (millis() - effect_delay >= time_now)
    {
        time_now = millis();
        // Main sequence
        for (uint32_t i = 0; i < led_num; i++)
            // led_array[i + led_index] = CHSV(led_hue[i + led_index]--, 255, 255);
            led_array[i] = CHSV(led_hue[i]--, 255, 255);
    }
    FastLED.show();
}
