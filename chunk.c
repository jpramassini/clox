//
// Created by JP Ramassini on 10/23/2019.
//

#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

void initChunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->lines = NULL;
    initValueArray(&chunk->constants);
}

void freeChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(int, chunk->lines, chunk->capacity);
    freeValueArray(&chunk->constants);
    initChunk(chunk);   // Re-init chunk to leave it in a nice defined but empty state.
}

void writeChunk(Chunk* chunk, uint8_t byte, int line) {   // Note: we check the current count against capacity, increasing capacity if needed.
    if(chunk->capacity < chunk->count + 1){
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(chunk->code, uint8_t, oldCapacity, chunk->capacity);
        chunk->lines = GROW_ARRAY(chunk->lines, int, oldCapacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->lines[chunk->count] = line;
    chunk->count++;
}

int addConstant(Chunk* chunk, Value value){
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;  // Note: Return index where it was appended for later reference.
}
