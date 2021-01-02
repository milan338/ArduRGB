// Libraries
#include <Arduino.h>
#include <FastLED.h>
#include <serial_read.h>
#include <definitions.h>
#include <led_strips.h>

// Effects
#include "effects/set_brightness.h"
#include "effects/rainbow_wave.h"
#include "effects/solid_color.h"
#include "effects/fade_black.h"

#define DEFAULT_BRIGHTNESS 50 // TODO replace with storing last brightness and if not found default to 50

// Physical LED strip arrays from which virtual CRGBSets are created
CRGBArray<STRIP_PHYSICAL_LEDS_0> strip_0; // Created during build

// Store all relevant information about each LED strip
typedef struct LEDDict
{
  CRGBSet led_array;
  uint8_t *hue_array;
  uint8_t *led_args;
  uint16_t led_num;
  uint32_t current_mode;
  uint32_t previous_mode;
  uint32_t current_time;
  uint32_t refresh_delay;
  bool effect_setup;
} LEDDict;

// Array for each led and associated hue values for use in effects
CRGBSet led_array_0(strip_0(0, LED_NUM_0 - 1)); // Created during build
uint8_t led_hues_0[LED_NUM_0];                  // Created during build
uint8_t led_args_0[ARGS_NUM];                   // Created during build

CRGBSet led_array_1(strip_0(LED_NUM_0, LED_NUM_0 + LED_NUM_1 - 1)); // Created during build
uint8_t led_hues_1[LED_NUM_1];                                      // Created during build
uint8_t led_args_1[ARGS_NUM];                                       // Created during build

LEDDict strips[]{
    {led_array_0, led_hues_0, led_args_0, LED_NUM_0, 0, 0, 0, LED_DELAY_0, false},  // Created during build
    {led_array_1, led_hues_1, led_args_1, LED_NUM_1, 0, 0, 0, LED_DELAY_1, false}}; // Created during build

// Determine whether to read a message or refresh effects
bool reading_message = false;
// Store mode from serial
char serial_mode_input[EFFECT_NAME_CHARS];
// Temporarily store effect args
uint8_t serial_args_input[ARGS_NUM];
// Store currently selected strip
uint8_t current_strip = 0;
// Store current message element to read
uint8_t serial_counter = 0;
// Store time for non-blocking delays
uint32_t time_now = 0;

void setup()
{
  // Power-up safety delay
  delay(2000);
  // Initialise each LED strip
  FastLED.addLeds<STRIP_PHYSICAL_TYPE_0, STRIP_PHYSICAL_PIN_0, STRIP_PHYISCAL_ORDER_0>(strip_0, STRIP_PHYSICAL_LEDS_0); // Created during build
  // Begin serial
  Serial.begin(BAUDRATE);
  Serial.setTimeout(SERIAL_TIMEOUT);
  // Recovery delay
  delay(1);
  FastLED.setBrightness(DEFAULT_BRIGHTNESS);
}

constexpr inline uint32_t hash(const char *effect, uint32_t h = 0)
{
  // Create numerical hash of character array
  return !effect[h] ? 5381 : (hash(effect, h + 1) * 33) ^ effect[h];
}

void clearSerial()
{
  // Clear available serial input
  while (Serial.available())
  {
    Serial.read();
  }
  // Ready to read new message
  reading_message = false;
  serial_counter = 0;
}

void runEffect() // Created during build
{
  for (uint8_t i = 0; i < STRIP_NUM; i++)
  {
    switch (strips[i].current_mode)
    {
    case hash("fadeblack"):
      FadeBlack::run(strips[i].led_array, strips[i].led_num, strips[i].current_mode);
      break;
    case hash("setbright"):
      SetBrightness::run(strips[i].current_mode, strips[i].previous_mode, strips[i].effect_setup, strips[i].led_args);
      break;
    case hash("solidcolor"):
      SolidColor::run(strips[i].led_array, strips[i].led_num, strips[i].current_mode, strips[i].led_args);
      break;
    case hash("rainbowwave"):
      RainbowWave::run(strips[i].led_array, strips[i].hue_array, strips[i].led_num, strips[i].effect_setup, strips[i].refresh_delay, strips[i].current_time);
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
        // Clear input buffers
        memset(serial_mode_input, 0, sizeof(serial_mode_input));
        memset(serial_args_input, 0, sizeof(serial_args_input));
        serial_counter = 0;
      }
      // End of message
      else if (serial_input == SERIAL_TERMINATE && reading_message)
      {
        clearSerial();
        // Setup effect before looping
        strips[current_strip].effect_setup = true;
        // Apply effect
        strips[current_strip].previous_mode = strips[current_strip].current_mode;
        strips[current_strip].current_mode = hash(serial_mode_input);
        // Apply new effect arguments
        for (uint8_t i = 0; i < ARGS_NUM; i++)
          strips[current_strip].led_args[i] = serial_args_input[i];
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
            // Add terminating byte
            serial_mode_input[serial_input] = '\0';
          }
          else
          {
            clearSerial();
            Serial.print(F("Char input stream not of expected size: "));
            Serial.println(serial_input);
          }
          break;
        case 2:
          // Store effect args
          if (Serial.readBytes(serial_args_input, serial_input) == (uint32_t)serial_input)
          {
            // Add terminating byte
            serial_args_input[serial_input] = '\0';
          }
          else
          {
            clearSerial();
            Serial.print(F("Arguments input stream not of expected size: "));
            Serial.println(serial_input);
          }
          break;
        }
        // Read next part of message
        serial_counter++;
      }
      // Remove serial input not part of valid message
      else
        clearSerial();
    }
  }
  // Handle serial delay
  else if (serial_counter > 1)
  {
    // Wait for serial
    time_now = millis();
    while (millis() - time_now < SERIAL_TIMEOUT)
    {
      if (Serial.available())
        break;
    }
    // Terminate byte not sent
    if (!Serial.available())
    {
      clearSerial();
      Serial.println(F("Did not receive expected terminate byte"));
    }
  }
  // Not reading message
  if (!reading_message)
  {
    runEffect();
  }
}
