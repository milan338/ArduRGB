#include <Arduino.h>
#include <definitions.h>
#include "serial_read.h"

uint8_t readSerial(uint8_t serial_buffer[], uint8_t buffer_size)
{
    if (Serial.readBytes(serial_buffer, buffer_size) != 0)
    {
        // Check for end-of-message byte
        if (serial_buffer[buffer_size - 1] == SERIAL_TERMINATE)
            return 1;
        else
            return 0;
    }
    else
        return 0;
}