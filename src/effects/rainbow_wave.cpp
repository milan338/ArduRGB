#include <FastLED.h>
#include "rainbow_wave.h"

void RainbowWave::run(CRGBSet &led_array, uint8_t hue_array[], uint16_t led_num, bool &effect_setup, uint32_t refresh_delay, uint32_t &current_time)
{
    if (effect_setup)
    {
        // Set initial colours that will be shifted through the main sequence
        for (uint16_t i = 0; i < led_num; i++)
            // led_hue[i + led_index] = 255 / led_num * i;
            hue_array[i] = 255 / led_num * i;
        effect_setup = false;
    }
    // Control speed of effect
    if (millis() - refresh_delay > current_time)
    {
        current_time = millis();
        // Main sequence
        for (uint16_t i = 0; i < led_num; i++)
            // led_array[i + led_index] = CHSV(led_hue[i + led_index]--, 255, 255);
            led_array[i] = CHSV(hue_array[i]--, 255, 255);
    }
    FastLED.show();
}
