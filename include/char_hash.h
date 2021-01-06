#ifndef HASH_H
#define HASH_H

#include <Arduino.h>

constexpr inline uint32_t hash(const char *str, uint32_t i = 0)
{
    // Create numerical hash of character array
    return !str[i] ? 5381 : (hash(str, i + 1) * 33) ^ str[i];
}

#endif //HASH_H