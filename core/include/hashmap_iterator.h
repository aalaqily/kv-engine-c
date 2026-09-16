#ifndef HASHMAP_ITER_H
#define HASHMAP_ITER_H

#include <stdbool.h>
#include <stddef.h>
#include "hashmap.h"

typedef struct HashMapIterator {
    HashMap *map;
    size_t bucket_index;
    HashNode* current;
} HashMapIterator;

/* Iterator Functions */

HashMapIterator *hashmap_iterator_create(HashMap *map);
void hashmap_iterator_destroy(HashMapIterator *iter);

/* Element Functions  */

bool hashmap_iterator_next(HashMapIterator *iter);
const char *hashmap_iterator_get_key(const HashMapIterator *iter);
const char *hashmap_iterator_get_value(const HashMapIterator *iter);


/* Metadata Functions  */

size_t hashmap_iterator_current_index(const HashMapIterator *iter);

#endif // HASHMAP_ITER_H
