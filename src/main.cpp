#include <Arduino.h>
#include <FastLED.h>
#include <serial_read.h>
#include <definitions.h>

// <<<<<<<<<< USER DEFINITIONS >>>>>>>>>> //

// Effect libraries
#include "effects/set_brightness.h"
#include "effects/rainbow_cycle.h"
#include "effects/solid_color.h"
#include "effects/fade_black.h"

// Set up a way to track last effect

#define SERIAL_MAX_ATTEMPTS 10

#define BAUDRATE 9600

#define DEFAULT_MODE 01
#define DEFAULT_BRIGHTNESS 50

// // Array for each LED strip
// CRGBArray<LED_NUM_0> led_array_0;
// // Hue array for each LED strip
// uint8_t led_hue_0[LED_NUM_0];

// <<<<<<<<<<<<<<<<<<< >>>>>>>>>>>>>>>>>> //
// Main LED array with all LEDs
CRGBArray<LEDS_NUM> led_array;

typedef struct CRGBHueDict
{
  CRGBSet crgb_array;
  uint8_t *hue_array;
  uint16_t strip_len;
  uint32_t current_mode;
  uint32_t previous_mode;
} CRGBHueDict;

// Check if multiple led arrays work
// i.e. try making two arrays, and see if my one led still works as it should
// also add support for virtual led strips on the same strip
CRGBSet led_array_0(led_array(0, LED_NUM_0)); // Created during build
uint8_t led_hues_0[LED_NUM_0];                // Created during build
uint32_t current_mode_0;
uint32_t previous_mode_0;

CRGBHueDict led_arrays[]{
    {led_array_0, led_hues_0, LED_NUM_0, current_mode_0, previous_mode_0}};

uint8_t led_hue[LEDS_NUM];
uint32_t strip_lengths[1] = {60}; // Created during build

uint8_t current_strip = 0;
uint32_t time_now = 0;
uint32_t effect_delay = 25;

char serial_mode_input[16];
// uint32_t current_mode;
// uint32_t previous_mode;

uint8_t serial_counter = 0;
bool reading_message = false;

bool effect_setup = true;

void setup()
{
  // Power-up safety delay
  delay(2000);
  // Initialise each LED strip
  // FastLED.addLeds<WS2812B, LED_PIN_0, LED_ORDER_0>(led_array_0, LED_NUM_0);
  FastLED.addLeds<WS2812B, LED_PIN_0, LED_ORDER_0>(led_array_0, LED_NUM_0); // Created during build
  // Begin serial
  Serial.begin(BAUDRATE);
  Serial.setTimeout(SERIAL_TIMEOUT);
  // Recovery delay
  delay(1);
  FastLED.setBrightness(DEFAULT_BRIGHTNESS);
}

constexpr inline uint32_t hash(const char *effect, uint32_t h = 0)
{
  return !effect[h] ? 5381 : (hash(effect, h + 1) * 33) ^ effect[h];
}

void clearSerial()
{
  // Clear available serial input
  while (Serial.available())
  {
    Serial.read();
  }
  // Reset running effects
  reading_message = false;
  effect_setup = true;
  serial_counter = 0;
}

void runEffect() // Created during build
{
  for (uint8_t i = 0; i < STRIP_NUM; i++)
  {
    switch (led_arrays[i].current_mode)
    {
    case hash("fadeblack"):
      FadeBlack::run(led_arrays[current_strip].crgb_array, led_arrays[current_strip].strip_len, led_arrays[i].current_mode);
      break;
    case hash("setbright"):
      SetBrightness::run(led_arrays[i].current_mode, led_arrays[i].previous_mode, effect_setup);
      break;
    case hash("solidcolor"):
      SolidColor::run(led_arrays[current_strip].crgb_array, led_arrays[current_strip].strip_len, led_arrays[i].current_mode);
      break;
    case hash("rainbowcycle"):
      RainbowCycle::run(led_arrays[current_strip].crgb_array, led_arrays[current_strip].hue_array, led_arrays[current_strip].strip_len, effect_setup, effect_delay, time_now);
      break;
    }
  }
}

void loop()
{
  // Read one message byte
  if (Serial.available())
  {
    int serial_input = Serial.read();
    Serial.println(serial_input);
    // Handle invalid reads
    if (serial_input != -1)
    {
      // Beginning of message
      if (serial_input == SERIAL_BEGIN && !reading_message)
      {
        // Reset serial read states
        reading_message = true;
        // Clear input buffer
        memset(serial_mode_input, 0, sizeof(serial_mode_input));
      }
      // End of message
      else if (serial_input == SERIAL_TERMINATE && reading_message)
      {
        clearSerial();
      }
      // Inside message contents
      else if (reading_message)
      {
        switch (serial_counter)
        {
        // Get selected strip
        case 0:
          // Ensure strip selected is valid
          if (serial_input < STRIP_NUM && serial_input >= 0)
          {
            // Set currently selected strip
            current_strip = serial_input;
          }
          else
          {
            clearSerial();
            Serial.print(F("Invalid LED strip selected: "));
            Serial.println(serial_input);
          }
          break;
        // Get selected mode
        case 1:
          // Store char array input as current mode
          if (Serial.readBytes(serial_mode_input, serial_input) == (uint32_t)serial_input)
          {
            // Store previous mode
            led_arrays[current_strip].previous_mode = led_arrays[current_strip].current_mode;
            // Store current mode
            serial_mode_input[serial_input] = '\0';
            led_arrays[current_strip].current_mode = hash(serial_mode_input);
          }
          else
          {
            clearSerial();
            Serial.print(F("Char input stream not of expected size: "));
            Serial.println(serial_input);
          }
          break;
        }
        serial_counter++;
      }
      // Remove serial input not part of valid message
      else
        clearSerial();
    }
  }
  // Handle missing terminate byte
  else if (serial_counter > 1)
  {
    clearSerial();
    Serial.println(F("Did not receive expected terminate byte"));
  }
  // Not reading message
  if (!reading_message)
  {
    runEffect();
  }
}
