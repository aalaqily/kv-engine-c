#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_CAPACITY 16
#define LOAD_FACTOR_THRESHOLD 0.75f

typedef struct HashNode {
    char *key;
    void *value;
    struct HashNode *next;
} HashNode;


struct HashMap {
    HashMap **buckets;
    size_t capacity;
    size_t size;
};

HashMap *hashmap_create(size_t initial_capacity) {
    if (initial_capacity == 0) {
        initial_capacity = DEFAULT_CAPACITY;
    }

    HashMap *hashmap = malloc(sizeof(HashMap));

    if (!hashmap) 
        return NULL;

    hashmap->capacity = initial_capacity;
    hashmap->size = 0;
    hashmap->buckets = calloc(hashmap->capacity, sizeof(HashNode *));

    if (!hashmap->buckets) {
        free(hashmap);
        return NULL;
    }

    return hashmap;
}