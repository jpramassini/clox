//
// Created by JP Ramassini on 10/23/19.
//

#include <stdio.h>

#include "debug.h"

// Iterate through items in chunk (after printing a lovely header)
void disassembleChunk(Chunk* chunk, const char* name){
    printf("== %s ==\n", name);

    for(int offset = 0; offset < chunk -> count;){
        // This increments offset, as the offset may be different based on the chunk!
        offset = disassembleInstruction(chunk, offset);
    }
}

static int simpleInstruction(const char* name, int offset){
    printf("%s\n", name);
    return offset + 1;
}

int disassembleInstruction(Chunk* chunk, int offset){
    printf("%04d ", offset);    // Print byte offset

    uint8_t instruction = chunk->code[offset];
    switch(instruction) {
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }
}
