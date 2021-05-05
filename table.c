//
// Created by JP Ramassini on 4/28/20.
//

#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"

#define TABLE_MAX_LOAD 0.75

void initTable(Table* table){
    table->count = 0;
    table->capacity=0;
    table->entries = NULL;
}

void freeTable(Table* table){
    FREE_ARRAY(Entry, table->entries, table->capacity);
    initTable((table));
}

static Entry* findEntry(Entry* entries, int capacity, ObjString* key){

    // @Note: Because our hash function is deterministic, these collisions will happen in the EXACT same order each time.
    // given that, we can actually use these collisions to our advantage to create an implicit linked list between collided
    // entries. Because of this, we have to be very careful when deleting entries. Deleting an earlier entry can cause
    // the loss of the rest of the probe chain, just like deleting nodes from a linked list.

    uint32_t index = key->hash % capacity;      // Choke down key's hash to ensure it fits within current entries array
    Entry* tombstone = NULL;

    for(;;) {
        Entry* entry = &entries[index];

        if(entry->key == NULL){
            if(IS_NIL(entry->value)) {
                // empty entry
                return tombstone != NULL ? tombstone : entry;  // if we passed a tombstone on the way to this empty key, return it as it can be reused.
            } else {
                // Tombstone found!
                if(tombstone == NULL) tombstone = entry;
            }
        } else if(entry->key == key) {
            // Entry found!
            return entry;
        }

        // there was something in that bucket, so increment and kick off (or continue) our linear probing.
        index = (index + 1) % capacity;
    }
}

static void adjustCapacity(Table* table, int capacity) {
    Entry* entries = ALLOCATE(Entry, capacity);
    for(int i = 0; i < capacity; i++){
        entries[i].key = NULL;
        entries[i].value = NIL_VAL;
    }

    table->count = 0;
    for(int i = 0; i < table->capacity; i++){
        Entry* entry = &table->entries[i];
        if(entry->key == NULL) continue; // If this is a tombstone or empty entry, no need to copy it over
        // We don't need to copy them because in adjusting the capacity, we rebuild the probing sequence, allowing us
        // to cull dead entries that will hurt lookup speed.
        Entry* dest = findEntry(entries, capacity, entry->key);
        dest->key = entry->key;
        dest->value = entry->value;
        table->count++;
    }

    FREE_ARRAY(Entry, table->entries, table->capacity);

    table->entries = entries;
    table->capacity = capacity;
}

bool tableGet(Table* table, ObjString* key, Value* value) {
    if(table->count == 0) return false;

    Entry* entry = findEntry(table->entries, table->capacity, key);
    if(entry->key == NULL) return false; // There wasn't anything there, so not a hit.

    // value is an output parameter here, so copy the well...value to value.
    *value = entry->value;
    return true;
}

bool tableDelete(Table* table, ObjString* key) {
    if(table->count == 0) return false;     // Can't delete something that definitely isn't there.

    // find entry
    Entry* entry = findEntry(table->entries, table->capacity, key);
    if(entry->key == NULL) return false;    // No such key exists in the table.

    // Put a tombstone in the entry
    // @Note: any representation for the tombstone is fine, so long as it can't be confused with a populated bucket.
    entry->key = NULL;
    entry->value = BOOL_VAL(true);

    return true;
}

bool tableSet(Table* table, ObjString* key, Value value) {
    if(table->count + 1 > table->capacity * TABLE_MAX_LOAD) {
        int capacity = GROW_CAPACITY(table->capacity);
        adjustCapacity(table, capacity);
    }
    Entry* entry = findEntry(table->entries, table->capacity, key);

    bool isNewKey = entry->key == NULL;
    if(isNewKey && IS_NIL(entry->value)) table->count++;

    entry->key = key;
    entry->value = value;
    return isNewKey;
}

void tableAddAll(Table* from, Table* to) {
    for(int i = 0; i < from->capacity; i++){
        Entry* entry = &from->entries[i];
        // If we come across a filled bucket, copy the value to the new table.
        if(entry->key != NULL){
            tableSet(to, entry->key, entry->value);
        }
    }
}

ObjString* tableFindString(Table* table, const char* chars, int length, uint32_t hash) {
    if(table->count == 0) return NULL;

    uint32_t index = hash % table->capacity;

    for(;;) {
        Entry* entry = &table->entries[index];

        if(entry->key == NULL) {
            // Stop if an empty non-tombstone entry is found.
            if(IS_NIL(entry->value)) return NULL;
        } else if(entry->key->length == length && entry->key->hash == hash && memcmp(entry->key->chars, chars, length) == 0) {
            // Note the order of the above checks. Avoid the memcmp unless absolutely necessary.
            // Ladies and gentlemen, we got him.
            return entry->key;
        }
        index = (index + 1) % table->capacity;
    }
}
