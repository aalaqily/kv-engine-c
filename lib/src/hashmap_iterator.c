#include <stdlib.h>
#include "kv_engine.h"

/* == KVEIterator Container Functions == */

KVEIterator *kve_iter_create(KVEHashMap *map) {
    if (!map)
        return NULL;

    KVEIterator *iter = malloc(sizeof(KVEIterator));

    if (!iter)
        return NULL;

    iter->map = map;
    iter->bucket_index = 0;
    iter->current = NULL;

    return iter;
}

void kve_iter_destroy(KVEIterator *iter) {
    free(iter);
}

/* == KVEIterator Element Functions == */

bool kve_iter_next(KVEIterator *iter) {
    if (!iter)
        return false;

    if (iter->current) {
        iter->current = iter->current->next;
    }

    while (!iter->current) {
        if (iter->bucket_index == kve_map_capacity(iter->map))
            return false;

        iter->bucket_index++;
        iter->current = iter->map->buckets[iter->bucket_index - 1];
    }

    return true;
}

const char *kve_iter_key(const KVEIterator *iter) {
    if (!iter)
        return NULL;

    if (!iter->current)
        return NULL;

    return iter->current->key;
}

const char *kve_iter_value(const KVEIterator *iter) {
    if (!iter)
        return NULL;

    if (!iter->current)
        return NULL;

    return iter->current->value;
}
