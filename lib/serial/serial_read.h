#ifndef SERIALREAD_H
#define SERIALREAD_H

#include <Arduino.h>
#include <definitions.h>
#include <structs.h>

class SerialRead
{
public:
    SerialRead(bool &reading_message, LEDDict *strips);
    void clearSerial();
    void readSerial();

private:
    // Store all relevant information about each LED strip
    LEDDict *strips;
    // Determine whether to read a message or refresh effects
    bool reading_message;
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
};

#endif // SERIALREAD_H
