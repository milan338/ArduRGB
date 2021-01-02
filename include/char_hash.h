#ifndef HASH_H
#define HASH_H

#include <Arduino.h>

constexpr inline uint32_t hash(const char *effect, uint32_t h = 0)
{
    // Create numerical hash of character array
    return !effect[h] ? 5381 : (hash(effect, h + 1) * 33) ^ effect[h];
}

#endif //HASH_H