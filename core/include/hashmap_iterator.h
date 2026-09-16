#ifndef HASHMAP_ITER_H
#define HASHMAP_ITER_H

#include <stdbool.h>
#include <stddef.h>
#include "hashmap.h"

/*
    Modifying a HashMap while a HashMapIterator is active is undefined behavior.
    A HashMapIterator MUST BE recereated after any write. 
*/

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
const char *hashmap_iterator_current_key(const HashMapIterator *iter);
const char *hashmap_iterator_current_value(const HashMapIterator *iter);

#endif // HASHMAP_ITER_H
