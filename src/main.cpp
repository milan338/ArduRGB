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

#define STRIP_NUM 1
// Number of LEDs on each strip
#define LED_NUM_0 60
// Pin each strip is connected to
#define LED_PIN_0 4

#define DEFAULT_MODE 01
#define DEFAULT_BRIGHTNESS 50

// Array for each LED strip
CRGBArray<LED_NUM_0> led_array_0;
// Hue array for each LED strip
uint8_t led_hue_0[LED_NUM_0];

// <<<<<<<<<<<<<<<<<<< >>>>>>>>>>>>>>>>>> //

uint32_t time_now_ms = 0;
uint32_t effect_delay_ms = 25;
// Store serial input
// uint8_t serial_counter = 0;
// byte serial_input;
// const byte serial_terminate = 255;
uint8_t serial_strip;
uint8_t serial_mode = 0;
uint8_t last_mode = 0;

uint8_t serial_counter = 0;
bool reading_message = false;

bool effect_setup = true;

void setup()
{
  Serial.begin(BAUDRATE);
  Serial.setTimeout(SERIAL_TIMEOUT);
  // Recovery delay
  delay(1);
  FastLED.setBrightness(DEFAULT_BRIGHTNESS);
  // Initialise each LED strip
  FastLED.addLeds<WS2812B, LED_PIN_0, COLOR_ORDER>(led_array_0, LED_NUM_0);
  // Debug
  if (Serial.available())
    Serial.println("connected");
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
      }
      // End of message
      else if (serial_input == SERIAL_TERMINATE && reading_message)
      {
        reading_message = false;
        serial_counter = 0;
      }
      // Inside message contents
      else if (reading_message)
      {
        switch (serial_counter)
        {
        case 0:
          serial_strip = serial_input;
          serial_counter++;
          break;
        case 1:
          last_mode = serial_mode;
          serial_mode = serial_input;
          break;
        }
        effect_setup = true;
      }
      // Remove serial input not part of valid message
      else
      {
        while (Serial.available())
        {
          Serial.println("clearing");
          Serial.read();
        }
      }
    }
  }
  // Not reading message
  if (!reading_message)
  {
    switch (serial_mode)
    {
    case 1:
      setBrightness(effect_setup, serial_mode, last_mode, NULL);
      break;
    case 2:
      fadeBlack(serial_mode, led_array_0);
      break;
    case 3:
      Serial.println("solid");
      setColour(led_array_0);
      serial_mode = 0;
      FastLED.show();
      break;
    case 4:
      cycle(effect_delay_ms, time_now_ms, led_array_0, led_hue_0, effect_setup);
      FastLED.show();
      break;
    }
  }
}
