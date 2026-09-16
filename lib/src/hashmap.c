#define _POSIX_C_SOURCE 200809L

#include "kv_engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xxhash.h>

// Internal helper using xxHash (64-bit hash)
static size_t helper_hash(const char *key) {
    // XXH64(data, length, seed)
    return (size_t)XXH64(key, strlen(key), 0);
}

/* == KVEHashMap Container Functions == */

KVEHashMap *kve_map_create(size_t initial_capacity) {
    if (initial_capacity == 0) {
        initial_capacity = KVE_DEFAULT_CAPACITY;
    }

    KVEHashMap *map = malloc(sizeof(KVEHashMap));

    if (!map)
        return NULL;

    map->capacity = initial_capacity;
    map->size = 0;
    map->buckets = calloc(map->capacity, sizeof(KVEHashNode *));

    if (!map->buckets) {
        free(map);
        return NULL;
    }

    return map;
}

void kve_map_destroy(KVEHashMap *map) {
    if (!map)
        return;

    for (size_t i = 0; i < map->capacity; i++) {
        KVEHashNode *current = map->buckets[i];
        while (current != NULL) {
            KVEHashNode *next = current->next;
            free(current->key);
            free(current->value);
            free(current);
            current = next;
        }
    }

    free(map->buckets);
    free(map);
}

bool kve_map_resize_to(KVEHashMap *map, size_t new_capacity) {
    if (!(map && new_capacity > 0))
        return false;

    KVEHashNode **new_buckets = calloc(new_capacity, sizeof(KVEHashNode *));
    if (!new_buckets)
        return false;

    for (size_t i = 0; i < kve_map_capacity(map); i++) {
        KVEHashNode *current = map->buckets[i];
        while (current != NULL) {
            KVEHashNode *next = current->next;

            size_t new_index = helper_hash(current->key) % new_capacity;

            current->next = new_buckets[new_index];
            new_buckets[new_index] = current;

            current = next;
        }
    }

    free(map->buckets);
    map->buckets = new_buckets;
    map->capacity = new_capacity;
    return true;
}

/* == KVEHashMap Element Functions == */

bool kve_map_put(KVEHashMap *map, const char *key, const char *value) {
    if (!(map && key))
        return false;

    if (kve_map_size(map) >= KVE_LOAD_FACTOR_THRESHOLD * kve_map_capacity(map))
        if (!kve_map_resize_to(map, kve_map_capacity(map) * KVE_RESIZE_FACTOR))
            return false;

    size_t index = helper_hash(key) % map->capacity;

    KVEHashNode *current = map->buckets[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            char *new_value = strdup(value);
            if (!new_value)
                return false;
            free(current->value);
            current->value = new_value;
            return true;
        }
        current = current->next;
    }

    KVEHashNode *node = malloc(sizeof(KVEHashNode));
    if (!node)
        return false;

    node->key = strdup(key);
    if (!node->key) {
        free(node);
        return false;
    }

    node->value = strdup(value);

    node->next = map->buckets[index];
    map->buckets[index] = node;

    map->size++;
    return true;
}

const char *kve_map_get(const KVEHashMap *map, const char *key) {
    if (!(map && key))
        return NULL;

    size_t index = helper_hash(key) % map->capacity;

    KVEHashNode *current = map->buckets[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }

    return NULL;
}

bool kve_map_remove(KVEHashMap *map, const char *key) {
    if (!(map && key))
        return false;

    size_t index = helper_hash(key) % map->capacity;

    KVEHashNode *current = map->buckets[index];
    KVEHashNode *prev = NULL;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (prev == NULL) {
                // Remove node from the head of the kve_map
                map->buckets[index] = current->next;
            } else {
                // Remove node from the middle or the end of the kve_map
                prev->next = current->next;
            }

            free(current->key);
            free(current->value);
            free(current);

            map->size--;
            return true;
        }

        prev = current;
        current = current->next;
    }

    // Key not found
    return false;
}

bool kve_map_contains(const KVEHashMap *map, const char *key) {
    if (!(map && key))
        return false;

    size_t index = helper_hash(key) % map->capacity;

    KVEHashNode *current = map->buckets[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0)
            return true;
        current = current->next;
    }

    // Key not found
    return false;
}

/* == KVEHashMap Metadata Functions == */

size_t kve_map_size(const KVEHashMap *map) {
    if (!map)
        return 0;

    return map->size;
}

size_t kve_map_capacity(const KVEHashMap *map) {
    if (!map)
        return 0;

    return map->capacity;
}
