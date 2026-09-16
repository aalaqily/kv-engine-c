#include <stdlib.h>
#include "hashmap_iterator.h"

/* Iterator Functions */

HashMapIterator *hashmap_iterator_create(HashMap *map) {
    if (!map)
        return NULL;

    HashMapIterator *iter = malloc(sizeof(HashMapIterator));
    iter->map = map;
    iter->bucket_index = 0;
    iter->current = iter->map->buckets[iter->bucket_index];

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
        if(iter->bucket_index == hashmap_capacity(iter->map) - 1)
            return false;
        
        iter->bucket_index++;
        iter->current = iter->map->buckets[iter->bucket_index];
    }

    return true;
}

const char *hashmap_iterator_get_key(const HashMapIterator *iter) {
    if(!iter)
        return NULL;

    return iter->current->key;
}

const char *hashmap_iterator_get_value(const HashMapIterator *iter) {
    if(!iter)
        return NULL;

    return iter->current->value;
}


/* Metadata Functions  */

size_t hashmap_iterator_current_index(const HashMapIterator *iter) {
    if (!iter)
        return 0;
    
    return iter->bucket_index;
}
