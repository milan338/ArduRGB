#include <Arduino.h>
#include <FastLED.h>
// #include "serial/ReadSerial.h"

// <<<<<<<<<< USER DEFINITIONS >>>>>>>>>> //

// Effect libraries
#include "effects/RainbowCycle.h"
#include "effects/SolidColour.h"

// Set up a way to track last effect

#define SERIAL_TERMINATE 255
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
byte serial_strip;
byte serial_mode;
byte last_mode = 0;

uint8_t serial_counter = 0;
bool terimated = false;

bool effect_setup = true; // Will a universal setup bool work?

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
    int serial_input = Serial.read();
    // Debug
    Serial.println(serial_input);
    // Deal with invalid reads
    if (serial_input != -1)
    {
      if (serial_input == SERIAL_TERMINATE)
      {
        Serial.println("terminate");
        // Beginning of message
        if (!terimated)
        {
          terimated = true;
        }
        // End of message
        else
        {
          Serial.println("break");
          terimated = false;
          serial_counter = 0;
        }
      }
      // Inside message contents
      else if (terimated)
      {
        Serial.println("inside");
        switch (serial_counter)
        {
        case 0:
          serial_strip = serial_input;
        case 1:
          last_mode = serial_mode;
          serial_mode = serial_input;
        }
        serial_counter++;
      }
      effect_setup = true;
    }
  }
  // Whole message has been read
  if (!terimated)
  {
    switch (serial_mode)
    {
    case 1:
      Serial.println("bright");
      effect_setup = false;
      FastLED.setBrightness(Serial.read());
      serial_mode = last_mode;
      FastLED.show();
      break;
    case 2:
      Serial.println("clear");
      FastLED.clear(true);
      serial_mode = 0;
      break;
    case 3:
      Serial.println("solid");
      setColour(LED_NUM_0, led_array_0);
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
