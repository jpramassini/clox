//
// Created by JP Ramassini on 10/23/19.
//

#include <stdlib.h>

#include "common.h"
#include "memory.h"

void* reallocate(void* previous, size_t oldSize, size_t newSize){
    if(newSize == 0){   // Handling deallocation
        free(previous);
        return NULL;
    }
    // Otherwise, realloc with more/less memory as appropriate.
    // If previous > newSize -> realloc with less memory.
    // If previous < newSize -> realloc with more memory.
    return realloc(previous, newSize);
}
