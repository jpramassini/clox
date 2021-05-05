//
// Created by JP Ramassini on 10/23/19.
//

#include <stdlib.h>

#include "common.h"
#include "memory.h"
#include "vm.h"

void* reallocate(void* previous, size_t oldSize, size_t newSize){
    if(newSize == 0){   // Handling deallocation
        free(previous);
        return NULL;
    }
    // Otherwise, realloc with more/less memory as appropriate.
    // If previous > newSize -> realloc with less memory.
    // If previous < newSize -> realloc with more memory.
    return realloc(previous, newSize);
}

static void freeObject(Obj* object){
    switch(object-> type){
        case OBJ_STRING: {
            ObjString* string = (ObjString*)object;
            FREE_ARRAY(char, string->chars, string->length + 1);
            FREE(ObjString, object);
            break;
        }
    }
}

void freeObjects(){
    Obj* object = vm.objects;
    while(object != NULL){
        Obj* next = object->next;
        freeObject(object);
        object = next;
    }
}
