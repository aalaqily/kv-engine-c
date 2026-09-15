#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>
#include <stdbool.h>

typedef struct HashNode {
    char *key;
    void *value;
    struct HashNode *next;
} HashNode;

typedef struct HashMap {
    HashNode **buckets;
    size_t capacity;
    size_t size;
} HashMap;


/* Container Functions */

HashMap *hashmap_create(size_t initial_capacity);
void hashmap_destroy(HashMap *map);
bool hashmap_resize_to(HashMap *map, size_t new_capacity);

/* Element Functions  */

bool hashmap_put(HashMap *map, const char *key, void *value);
void *hashmap_get(const HashMap *map, const char *key);
bool hashmap_remove(HashMap *map, const char *key);
bool hashmap_contains(const HashMap *map, const char *key);

/* Metadata Functions  */

size_t hashmap_size(const HashMap *map);
size_t hashmap_capacity(const HashMap *map);

#endif // HASHMAP_H
