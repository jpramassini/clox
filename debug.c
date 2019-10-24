//
// Created by JP Ramassini on 10/23/19.
//

#include <stdio.h>

#include "debug.h"
#include "value.h"

// Iterate through items in chunk (after printing a lovely header)
void disassembleChunk(Chunk* chunk, const char* name){
    printf("== %s ==\n", name);

    for(int offset = 0; offset < chunk -> count;){
        // This increments offset (not the loop), as the offset may be different based on the chunk!
        offset = disassembleInstruction(chunk, offset);
    }
}

static int simpleInstruction(const char* name, int offset){
    printf("%s\n", name);
    return offset + 1;
}

static int constantInstruction(const char* name, Chunk* chunk, int offset){
    uint8_t constant = chunk->code[offset + 1];
    printf("%-16s %4d '", name, constant);            // Print the opcode type and the offset of the constant.
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 2;      // Note: Need to return offset + 2 as we've grabbed 2 bytes in this function.
}

int disassembleInstruction(Chunk* chunk, int offset){
    printf("%04d ", offset);    // Print byte offset, helpful for determining WHERE in the chunk a code is.
    if(offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1]) {    // If the last two lines are equal, show that with a tab + | in output.
        printf("    | ");
    } else {
        printf("%4d ", chunk->lines[offset]);
    }

    uint8_t instruction = chunk->code[offset];  // Grab the opcode at the offset.
    switch(instruction) {
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        default:
            // This isn't a recognized opcode, likely an interpreter error.
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }
}
