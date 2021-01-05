// Libraries
#include <Arduino.h>
#include <FastLED.h>
#include <definitions.h>
#include <led_strips.h>
#include <char_hash.h>
#include <structs.h>
#include <serial_read.h>

// Effects
#include "effects/set_brightness.h"
#include "effects/rainbow_wave.h"
#include "effects/solid_color.h"
#include "effects/fade_black.h"

#define DEFAULT_BRIGHTNESS 50 // TODO replace with storing last brightness and if not found default to 50

// Physical LED strip arrays from which virtual CRGBSets are created
CRGBArray<STRIP_PHYSICAL_LEDS_0> strip_0; // Created during build

// Array for each led and associated hue values for use in effects
CRGBSet led_array_0(strip_0(0, LED_NUM_0 - 1)); // Created during build
uint8_t led_hues_0[LED_NUM_0];                  // Created during build
uint8_t led_args_0[ARGS_NUM];                   // Created during build

CRGBSet led_array_1(strip_0(LED_NUM_0, LED_NUM_0 + LED_NUM_1 - 1)); // Created during build
uint8_t led_hues_1[LED_NUM_1];                                      // Created during build
uint8_t led_args_1[ARGS_NUM];                                       // Created during build

// Store all relevant information about each LED strip
LEDDict strips[]{
    {led_array_0, led_hues_0, led_args_0, LED_NUM_0, 0, 0, 0, LED_DELAY_0, false},  // Created during build
    {led_array_1, led_hues_1, led_args_1, LED_NUM_1, 0, 0, 0, LED_DELAY_1, false}}; // Created during build

// Determine whether to read a message or refresh effects
bool reading_message = false;

// Create object of message type used to read input
#if MESSAGE_TYPE == _SERIAL
SerialRead serial_read = SerialRead(reading_message, strips);
#endif

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
  // Read message through defined input type
#if MESSAGE_TYPE == _SERIAL
  serial_read.readSerial();
#endif
  // Not reading message
  if (!reading_message)
  {
    // Update LED strips
    runEffect();
  }
}
