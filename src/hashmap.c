#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xxhash.h>

#define DEFAULT_CAPACITY 16
#define LOAD_FACTOR_THRESHOLD 0.75f

// Internal helper using xxHash (64-bit hash)
static size_t helper_hash(const char *key) {
    // XXH64(data, length, seed)
    return (size_t) XXH64(key, strlen(key), 0);
}

/* Container Functions */

HashMap *hashmap_create(size_t initial_capacity) {
    if (initial_capacity == 0) {
        initial_capacity = DEFAULT_CAPACITY;
    }

    HashMap *map = malloc(sizeof(HashMap));

    if (!map) 
        return NULL;

    map->capacity = initial_capacity;
    map->size = 0;
    map->buckets = calloc(map->capacity, sizeof(HashNode *));

    if (!map->buckets) {
        free(map);
        return NULL;
    }

    return map;
}

void hashmap_destroy(HashMap *map) {
    if (!map)
        return;

    for (size_t i = 0; i < map->capacity; i++) {
        HashNode *current = map->buckets[i];
        while (current != NULL) {
            HashNode *next = current->next;
            free(current->key);
            free(current);
            current = next;
        }
    }

    free(map->buckets);
    free(map);
}

bool hashmap_resize_to(HashMap *map, size_t new_capacity) {
    if (!(map && new_capacity > 0))
        return false;

    HashNode **new_buckets = calloc(new_capacity, sizeof(HashNode *));
    if (!new_buckets)
        return false;


    for (size_t i = 0; i < hashmap_capacity(map); i++) {
        HashNode *current = map->buckets[i];
        while (current != NULL) {
            HashNode *next = current->next;

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

/* Element Functions  */

#define RESIZE_FACTOR 2

bool hashmap_put(HashMap *map, const char *key, void *value) {
    if (!(map && key))
        return false;

    if (hashmap_size(map) >= 0.75 * hashmap_capacity(map))
        if (!hashmap_resize_to(map, hashmap_capacity(map) * RESIZE_FACTOR))
            return false;

    size_t index = helper_hash(key) % map->capacity;

    HashNode *current = map->buckets[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            current->value = value;
            return true;
        }
        current = current->next;
    } 

    HashNode *node = malloc(sizeof(HashNode));
    if (!node)
        return false;
    
    node->key = strdup(key);
    if (!node->key) {
        free(node);
        return false;
    }

    node->value = value;

    node->next = map->buckets[index];
    map->buckets[index] = node;
    
    map->size++;
    return true;
}

void *hashmap_get(const HashMap *map, const char *key) {
    if (!(map && key))
        return NULL;

    size_t index = helper_hash(key) % map->capacity;

    HashNode *current = map->buckets[index];
    while (current != NULL) {
        if(strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }

    return NULL;
}

bool hashmap_remove(HashMap *map, const char *key) {
    if(!(map && key))
        return false;

    size_t index = helper_hash(key) % map->capacity;

    HashNode *current = map->buckets[index];
    HashNode *prev = NULL;
    while (current != NULL) {
        if(strcmp(current->key, key) == 0) {
            if (prev == NULL) {
                // Remove node from the head of the hashmap
                map->buckets[index] = current->next;
            } else {
                // Remove node from the middle or the end of the hashmap
                prev->next = current->next;
            }

            free(current->key);
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

bool hashmap_contains(const HashMap *map, const char *key) {
    if(!(map && key))
        return false;

    size_t index = helper_hash(key) % map->capacity;

    HashNode *current = map->buckets[index];
    while(current != NULL) {
        if(strcmp(current->key, key) == 0)
            return true;
        current = current->next;
    }

    // Key not found
    return false;
}

/* Metadata Functions  */

size_t hashmap_size(const HashMap *map) {
    if (!map)
        return 0;

    return map->size;
}

size_t hashmap_capacity(const HashMap *map) {
    if (!map)
        return 0;

    return map->capacity;
}
