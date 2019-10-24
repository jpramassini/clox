//
// Created by JP Ramassini on 10/23/2019.
//

#ifndef CLOX_CHUNK_H
#define CLOX_CHUNK_H

#include "common.h"

typedef enum {
    OP_RETURN,      // Used to tell VM to return from current function.
} OpCode;

typedef struct {
    int count;      // How many elements are in use.
    int capacity;
    uint8_t* code;  // Note: We don't know how much memory a chunk will require on instantiation,
} Chunk;            // hence the pointer to a dynamic array.

void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte);

#endif // CLOX_CHUNK_H
