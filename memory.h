//
// Created by JP Ramassini on 10/23/19.
//

#ifndef CLOX_MEMORY_H
#define CLOX_MEMORY_H

#include "object.h"

#define ALLOCATE(type, count) \
    (type*)reallocate(NULL, 0, sizeof(type) * (count))

// Just reuses the reallocate logic to "resize" to zero bytes.
// Sending this through reallocate allows us to keep track of how much memory is actively being used.
#define FREE(type, pointer) \
    reallocate(pointer, sizeof(type), 0)

#define GROW_CAPACITY(capacity) \
    ((capacity) < 8 ? 8 : (capacity) * 2)   // Note: Double capacity each time we need to increase. (Except for initial grow, which should be 8.)

// A convenience macro to grab the type size and cast the resulting void* to the right type.
#define GROW_ARRAY(previous, type, oldCount, count) \
    (type*)reallocate(previous, sizeof(type) * (oldCount), sizeof(type) * (count))

#define FREE_ARRAY(type, pointer, oldCount) \
    reallocate(pointer, sizeof(type) * (oldCount), 0)

void* reallocate(void* previous, size_t oldSize, size_t newSize);
void freeObjects();

#endif //CLOX_MEMORY_H
