#include <Arduino.h>
#include <FastLED.h>
#include "serial/ReadSerial.h"

// <<<<<<<<<< USER DEFINITIONS >>>>>>>>>> //

// Effect libraries
#include "effects/RainbowCycle.h"
#include "effects/SolidColour.h"

// Effect specific definitions
// bool rainbow_cycle_setup = true; // Make it so when the mode is changed the setup variable is always reset

// Set up a way to track last effect

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
uint32_t time_now = 0;
uint32_t effect_delay = 25;
byte serial_input;
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
  if (Serial.available())
  {
    serial_input = Serial.read();
    // Debug
    Serial.println(serial_input);
    effect_setup = true;
  }

  switch (serial_input)
  {
  case 1:
    effect_setup = false;
    setBrightness(readSerial(serial_timeout_ms));
  case 2:
    setOff();
  case 3: // TODO sending int 3 freezes communication
    setColour(LED_NUM_0, led_array_0, serial_timeout_ms, R, G, B);
  case 4:
    cycle(effect_delay, time_now, led_array_0, led_hue_0, effect_setup);
  }

  FastLED.show();
}
