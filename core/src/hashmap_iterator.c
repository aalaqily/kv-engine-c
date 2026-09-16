#include <stdlib.h>
#include "hashmap_iterator.h"

/* Iterator Functions */

HashMapIterator *hashmap_iterator_create(HashMap *map) {
    if (!map)
        return NULL;

    HashMapIterator *iter = malloc(sizeof(HashMapIterator));

    if(!iter)
        return NULL;

    iter->map = map;
    iter->bucket_index = 0;
    iter->current = NULL;

    return iter;
}

void hashmap_iterator_destroy(HashMapIterator *iter) {
    free(iter);
}

/* Element Functions  */

bool hashmap_iterator_next(HashMapIterator *iter) {
    if(!iter)
        return false;

    if(iter->current) {
        iter->current = iter->current->next;
    }

    while(!iter->current) {
        if(iter->bucket_index == hashmap_capacity(iter->map))
            return false;
        
        iter->bucket_index++;
        iter->current = iter->map->buckets[iter->bucket_index - 1];
    }

    return true;
}

const char *hashmap_iterator_current_key(const HashMapIterator *iter) {
    if(!iter)
        return NULL;

    if(!iter->current)
        return NULL;

    return iter->current->key;
}

const char *hashmap_iterator_current_value(const HashMapIterator *iter) {
    if(!iter)
        return NULL;

    if(!iter->current)
        return NULL;

    return iter->current->value;
}
