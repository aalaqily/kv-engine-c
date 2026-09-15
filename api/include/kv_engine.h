#ifndef KV_ENGINE_H
#define KV_ENGINE_H

#include <stdbool.h>
#include <stddef.h>

// Opaque handle - hides internal HashMap structures from API consumers
typedef struct KVEngine KVEngine;

/* == Engine Lifecycle == */
KVEngine *kv_engine_create(void);
void kv_engine_destroy(KVEngine *engine);

/* == Key-Value Operations == */
bool kv_engine_put(KVEngine *engine, const char *key, const char *value);
const char *kv_engine_get(const KVEngine *engine, const char *key);
bool kv_engine_remove(KVEngine *engine, const char *key);
bool kv_engine_contains(const KVEngine *engine, const char *key);

/* == Metadata == */
size_t kv_engine_size(const KVEngine *engine);
size_t kv_engine_capacity(const KVEngine *engine);

/* == Binary Persistence == */
bool kv_engine_save(const KVEngine *engine, const char *filepath);
bool kv_engine_load(KVEngine *engine, const char *filepath);

#endif // KV_ENGINE_H