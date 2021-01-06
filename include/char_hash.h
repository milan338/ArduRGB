#ifndef HASH_H
#define HASH_H

#include <Arduino.h>

constexpr inline uint32_t hash(const char *effect, uint32_t i = 0)
{
    // Create numerical hash of character array
    return !effect[i] ? 5381 : (hash(effect, i + 1) * 33) ^ effect[i];
}

#endif //HASH_H