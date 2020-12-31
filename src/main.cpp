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
} CRGBHueDict;

// Check if multiple led arrays work
// i.e. try making two arrays, and see if my one led still works as it should
// also add support for virtual led strips on the same strip
CRGBSet led_array_0(led_array(0, LED_NUM_0)); // Created during build
uint8_t led_hues_0[LED_NUM_0];                // Created during build

CRGBHueDict led_arrays[]{// Created during build
                         {led_array_0, led_hues_0, LED_NUM_0}};

uint8_t led_hue[LEDS_NUM];
uint32_t strip_lengths[1] = {60}; // Created during build

uint8_t current_strip = 0;
uint8_t serial_mode = 0;
uint8_t last_mode = 0;
uint32_t time_now = 0;
uint32_t effect_delay = 25;

char serial_mode_input[16];
char current_mode[16];
char previous_mode[16];

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
  Serial.println("clearing");
  while (Serial.available())
  {
    Serial.println("clearing");
    Serial.read();
  }
  reading_message = false;
  effect_setup = true;
}

void copyCharArray(char from[], char to[], int length)
{
  for (int i = 0; i < length; i++)
    to[i] = from[i];
  to[length] = '\0';
}

void runEffect() // Created during build
{
  switch (hash(current_mode))
  {
  case hash("fadeblack"):
    FadeBlack::run(led_arrays[current_strip].crgb_array, led_arrays[current_strip].strip_len, serial_mode);
    break;
  case hash("setbright"):
    SetBrightness::run(serial_mode, last_mode, effect_setup);
    break;
  case hash("solidcolor"):
    SolidColor::run(led_arrays[current_strip].crgb_array, led_arrays[current_strip].strip_len);
    break;
  case hash("rainbowcycle"):
    RainbowCycle::run(led_arrays[current_strip].crgb_array, led_arrays[current_strip].hue_array, led_arrays[current_strip].strip_len, effect_setup, effect_delay, time_now);
    break;
  }
}

void loop()
{
  // Read one message byte
  if (Serial.available())
  {
    int serial_input = Serial.read();
    // Debug
    Serial.println(serial_input);
    // Deal with invalid reads
    if (serial_input != -1)
    {
      // Beginning of message
      if (serial_input == SERIAL_BEGIN && !reading_message)
      {
        reading_message = true;
        serial_counter = 0;
        memset(serial_mode_input, 0, sizeof(serial_mode_input));
      }
      // End of message
      else if (serial_input == SERIAL_TERMINATE && reading_message)
      {
        clearSerial();
        // reading_message = false;
      }
      // Inside message contents
      else if (reading_message)
      {
        switch (serial_counter)
        {
        case 0:
          // Set currently selected strip
          current_strip = serial_input;
          serial_counter++;
          break;
        case 1:
          // Store char array input as current mode
          if (Serial.readBytes(serial_mode_input, serial_input) != (uint32_t)serial_input)
            clearSerial();
          else
          {
            // Store previous mode
            copyCharArray(current_mode, previous_mode, serial_input);
            // Store current mode
            copyCharArray(serial_mode_input, current_mode, serial_input);
            Serial.println("current mode");
            Serial.println(current_mode);
            // last_mode = serial_mode;
            // serial_mode = serial_input;
          }
          serial_counter++;
          break;
        }
      }
      // Remove serial input not part of valid message
      else
        clearSerial();
    }
  }
  // Not reading message
  if (!reading_message)
  {
    runEffect();
    // uint32_t led_num = strip_lengths[0];
  }
}
