#include <Arduino.h>
#include <FastLED.h>
#include "serial/ReadSerial.h"

// <<<<<<<<<< USER DEFINITIONS >>>>>>>>>> //

// Effect libraries
#include "effects/RainbowCycle.h"
#include "effects/SolidColour.h"

// Set up a way to track last effect

#define SERIAL_TERMINATE 255

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

uint8_t R = 0;
uint8_t G = 0;
uint8_t B = 0;

uint16_t serial_timeout_ms = 2000;
uint32_t time_now_ms = 0;
uint32_t effect_delay_ms = 25;
// Store serial input
uint8_t serial_counter = 0;
// byte serial_input;
// const byte serial_terminate = 255;
byte serial_strip;
byte serial_mode;

bool effect_setup = true; // Will a universal setup bool work?

void setBrightness(uint8_t brightness)
{
  FastLED.setBrightness(brightness);
}

void setOff()
{
  FastLED.clear();
}

void setup()
{
  Serial.begin(BAUDRATE);
  // Recovery delay
  delay(1);
  FastLED.setBrightness(DEFAULT_BRIGHTNESS);
  // Initialise each LED strip
  FastLED.addLeds<NEOPIXEL, LED_PIN_0>(led_array_0, LED_NUM_0);
  // Debug
  if (Serial.available())
    Serial.println("connected");
}

void loop()
{
  while (Serial.available())
  {
    int serial_input = readSerial(serial_timeout_ms);
    // Debug
    Serial.println(serial_input);
    // Handle timeout
    if (serial_input == -1)
    {
      serial_strip = 0;
      serial_mode = 0;
      serial_counter = 0;
    }
    else if (serial_input == SERIAL_TERMINATE)
    {
      serial_counter = 0;
    }
    else
    {
      switch (serial_counter)
      {
      case 0:
        serial_strip = (byte)serial_input;
      case 1:
        serial_mode = (byte)serial_input;
      }
    }
    effect_setup = true;
  }

  // switch (serial_input)
  switch (serial_mode)
  {
  case 1:
    effect_setup = false;
    setBrightness(readSerial(serial_timeout_ms));
  case 2:
    setOff();
  case 3: // TODO sending int 3 freezes communication
    setColour(LED_NUM_0, led_array_0, serial_timeout_ms, R, G, B);
  case 4:
    cycle(effect_delay_ms, time_now_ms, led_array_0, led_hue_0, effect_setup);
  }

  FastLED.show();
}
