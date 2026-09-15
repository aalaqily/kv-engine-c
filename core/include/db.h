#ifndef KV_DB_H
#define KV_DB_H

#include "hashmap.h"
#include <stdint.h>

#define KV_MAGIC "KVDB"
#define KV_VERSION 1

// Ensure 1-byte alignment so C structs mirror on-disk layout exactly
#pragma pack(push, 1)

typedef struct {
    char magic[4];         // Must equal "KVDB"
    uint16_t version;      // Version 1
    uint16_t reserved;     // Reserved space (0x0000)
    uint64_t record_count; // Total records stored
} DBHeader;

typedef struct {
    uint16_t key_len; // Max key size: 65,535 bytes
    uint32_t val_len; // Max val size: ~4.2 GB
} RecordHeader;

#pragma pack(pop)

// Public API
bool db_save(const HashMap *map, const char *filepath);
bool db_load(HashMap *map, const char *filepath);

#endif // KV_DB_H