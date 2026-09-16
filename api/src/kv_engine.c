#include "kv_engine.h"

#include <stdlib.h>

#include "db.h"
#include "hashmap.h"
#include "hashmap_iterator.h"

typedef struct KVEngine {
    HashMap *map;
} KVEngine;

typedef struct KVIterator {
    HashMapIterator *map_iter;
} KVIterator;

KVEngine *kv_engine_create(void) {
    KVEngine *engine = malloc(sizeof(KVEngine));
    if (!engine)
        return NULL;

    engine->map = hashmap_create(16);
    if (!engine->map) {
        free(engine);
        return NULL;
    }
    return engine;
}

void kv_engine_destroy(KVEngine *engine) {
    if (!engine)
        return;
    hashmap_destroy(engine->map);
    free(engine);
}

bool kv_engine_put(KVEngine *engine, const char *key, const char *value) {
    if (!engine)
        return false;
    return hashmap_put(engine->map, key, value);
}

const char *kv_engine_get(const KVEngine *engine, const char *key) {
    if (!engine)
        return NULL;
    return hashmap_get(engine->map, key);
}

bool kv_engine_remove(KVEngine *engine, const char *key) {
    if (!engine)
        return false;
    return hashmap_remove(engine->map, key);
}

bool kv_engine_contains(const KVEngine *engine, const char *key) {
    if (!engine)
        return false;
    return hashmap_contains(engine->map, key);
}

size_t kv_engine_size(const KVEngine *engine) { return engine ? hashmap_size(engine->map) : 0; }

size_t kv_engine_capacity(const KVEngine *engine) {
    return engine ? hashmap_capacity(engine->map) : 0;
}

bool kv_engine_save(const KVEngine *engine, const char *filepath) {
    if (!engine)
        return false;
    return db_save(engine->map, filepath);
}

bool kv_engine_load(KVEngine *engine, const char *filepath) {
    if (!engine)
        return false;
    return db_load(engine->map, filepath);
}

KVIterator *kv_iter_create(KVEngine *engine) {
    if(!engine)
        return NULL;

    KVIterator *iter = malloc(sizeof(KVIterator));
    if (!iter)
        return NULL;

    iter->map_iter = hashmap_iterator_create(engine->map);
    if(!iter->map_iter)
        return NULL;

    return iter;
}

void kv_iter_destroy(KVIterator *iter) {
        if (!iter)
            return;
    
    hashmap_iterator_destroy(iter->map_iter);
    free(iter);
}

bool kv_iter_next(KVIterator *iter) {
    if(!iter)
        return false;
    
    return hashmap_iterator_next(iter->map_iter);
}

const char *kv_iter_current_key(const KVIterator *iter) {
    if(!iter)
        return NULL;
    
    return hashmap_iterator_current_key(iter->map_iter);
}

const char *kv_iter_current_value(const KVIterator *iter) {
    if (!iter)
        return NULL;
    
    return hashmap_iterator_current_value(iter->map_iter);
}
