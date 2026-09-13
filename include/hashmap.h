#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>
#include <stdbool.h>

typedef struct HashMap HashMap;

/* Helper Functions */

HashMap *hashmap_create(size_t initial_capacity);
void hashmap_destroy(HashMap *map);

/* Core Functions */

bool hashmap_put(HashMap *map, const char *key, void *value);
void *hashmap_get(const HashMap *map, const char *key);
bool hashmap_remove(HashMap *map, const char *key);
bool hashmap_contains(const HashMap *map, const char *key);

/* Metadata */

size_t hashmap_size(const HashMap *map);

#endif // HASHMAP_H
