//
// Created by JP Ramassini on 4/26/20.
//

#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "value.h"
#include "vm.h"
#include "table.h"

// This macro is mostly to avoid a bunch of casting a void* to the right type.
// Also handily keeps the allocateObject function a bit cleaner by just passing it a size_t.
#define ALLOCATE_OBJ(type, objectType) \
    (type*)allocateObject(sizeof(type), objectType)

static Obj* allocateObject(size_t size, ObjType type) {
    // Allocates the space for the base object's state as well as any extra fields that the object variant requires.
    Obj* object = (Obj*)reallocate(NULL, 0, size);
    object->type = type;
    object->next = vm.objects;
    vm.objects = object;
    return object;
}

static ObjString* allocateString(char* chars, int length, uint32_t hash) {
    ObjString* string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string->length = length;
    string->chars = chars;
    string->hash = hash;

    tableSet(&vm.strings, string, NIL_VAL);

    return string;
}

static uint32_t hashString(const char* key, int length) {
    // @Note: this function uses the FNV-1a hashing algorithm.
    uint32_t hash = 2166136261u;

    for(int i = 0; i < length; i++){
        hash ^= key[i];
        hash *= 16777619;
    }

    return hash;
}

/*
 * @Note: takeString and copyString do somewhat similar things, but are based on two different assumptions of ownership.
 * In the context of copyString, we're using it to store literals from a larger string that belongs to the source code,
 * so passing around a pointer directly to that is very dangerous and would mutate the end user's source code if
 * mutations were performed on that pointer. takeString is for situations where we're concatenating and thus already
 * owners of the memory. (It also means we don't have to remember to free multiple strings).
 */

ObjString* takeString(char* chars, int length) {
    uint32_t hash = hashString(chars, length);
    ObjString* interned = tableFindString(&vm.strings, chars, length, hash);

    if(interned != NULL) return interned;

    return allocateString(chars, length, hash);
}

ObjString* copyString(const char* chars, int length) {

    uint32_t hash = hashString(chars, length);
    ObjString* interned = tableFindString(&vm.strings, chars, length, hash);

    if(interned != NULL) return interned;

    char* heapChars = ALLOCATE(char, length + 1);
    memcpy(heapChars, chars, length);
    heapChars[length] = '\0';

    return allocateString(heapChars, length, hash);
}

void printObject(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_STRING:
            // The null termination before lets us lean on the standard lib here.
            printf("%s", AS_CSTRING(value));
            break;
    }
}