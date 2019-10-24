//
// Created by JP Ramassini on 10/23/19.
//

#include <stdio.h>

#include "memory.h"
#include "value.h"

// The code in this section mirrors the chunk code closely.
// The same init, write, free pattern is used here.

void initValueArray(ValueArray* array){
    array->values = NULL;
    array->capacity = 0;
    array->count = 0;
}

void writeValueArray(ValueArray* array, Value value){
    if(array->capacity < array->count + 1){
        int oldCapacity = array->capacity;
        array->capacity = GROW_CAPACITY(oldCapacity);
        array->values = GROW_ARRAY(array->values, Value,
                oldCapacity, array->capacity);
    }

    array->values[array->count] = value;
    array->count++;
}

void freeValueArray(ValueArray* array){
    FREE_ARRAY(Value, array->values,array->capacity);
    initValueArray(array);
}

void printValue(Value value){
    printf("%g", value);
}