//
// Created by JP Ramassini on 11/2/19.
//

#include <stdio.h>

#include "common.h"
#include "vm.h"
#include "debug.h"

VM vm;
/*
 * NOTE: it would be cleaner to pass a VM pointer to all of the below functions.
 * This choice was made for the purposes of the book format (and keeping things small).
 * This would be a good thing to come back and refactor for more flexibility and less global variables.
 * */

static void resetStack() {
    vm.stackTop = vm.stack;
}

void initVM() {
    resetStack();
}

void freeVM() {
}

void push(Value value) {
    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop() {
    vm.stackTop--;
    return *vm.stackTop;
}

// This is the most important function for the whole interpreter!
// This function could also be faster, but it requires non-standard C or assembly.
static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)  // Dereference current ip and then increment.
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])  // If a constant instruction, the next byte will be
                                                                    // the address of the constant, so go grab it.
    for(;;){
#ifdef DEBUG_TRACE_EXECUTION
        printf("          ");
        for(Value* slot = vm.stack; slot < vm.stackTop; slot++){
            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }
        printf("\n");
        disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
        uint8_t instruction;
        switch(instruction = READ_BYTE()) { // NOTE: Switch on opcode. The first byte of any instruction is the opcode.
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(constant);
                printValue(constant);
                printf("\n");
                break;
            }
            case OP_NEGATE: {
                push(-pop());   // Grab the value on the stack and push back the negated version.
                break;
            }
            case OP_RETURN: {
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;
            }
        }
    }

#undef READ_CONSTANT
#undef READ_BYTE
}

InterpretResult interpret(Chunk* chunk) {
    vm.chunk = chunk;       // Store current chunk being interpreted.
    vm.ip = vm.chunk->code; // Store location of current instruction being executed.
    return run();
}
