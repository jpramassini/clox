//
// Created by JP Ramassini on 10/23/19.
//

#ifndef CLOX_VALUE_H
#define CLOX_VALUE_H

#include "common.h"

typedef double Value;   // Using typedef to support changing data representation globally easily.

typedef struct {
    int capacity;
    int count;
    Value* values;
} ValueArray;

void initValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value value);
void freeValueArray(ValueArray* array);
void printValue(Value value);

#endif //CLOX_VALUE_H
