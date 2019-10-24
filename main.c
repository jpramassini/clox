#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, const char* argv[]){
    Chunk chunk;
    initChunk(&chunk);

    int constant = addConstant(&chunk, 1.2);
    writeChunk(&chunk, OP_CONSTANT, 123);    // Note: first, write the opcode.
    writeChunk(&chunk, constant, 123);            // Second, write the operand (the offset of the constant)

    int other_constant = addConstant(&chunk, 2.7);
    writeChunk(&chunk, OP_CONSTANT, 124);
    writeChunk(&chunk, other_constant, 124);

    writeChunk(&chunk, OP_RETURN, 124);

    disassembleChunk(&chunk, "test chunk");
    freeChunk(&chunk);
    return 0;
}