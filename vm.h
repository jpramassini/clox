//
// Created by JP Ramassini on 11/2/19.
//

#ifndef CLOX_VM_H
#define CLOX_VM_H

#include "chunk.h"
#include "value.h"
#include "table.h"

#define STACK_MAX 256

typedef struct {
    Chunk* chunk;
    uint8_t* ip;    // NOTE: Point directly to instruction in bytecode array, as dereferencing is faster than an indexed array look. Also, ip stands for Instruction Pointer, just a heads up.
    Value stack[STACK_MAX];
    Value* stackTop;
    Table strings;

    Obj* objects;   // This points to the head of the linked list of dynamically allocated objects.
} VM;

// NOTE: This is used to set the exit code of the interpret process.
typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

extern VM vm;

void initVM();
void freeVM();
InterpretResult interpret(const char* source);    // NOTE: This is the main entrypoint to VM
void push(Value value);
Value pop();

#endif //CLOX_VM_H
