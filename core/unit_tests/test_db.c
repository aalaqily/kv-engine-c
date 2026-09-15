#include "db.h"
#include "hashmap.h"
#include "tests.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define TEST_DB_FILE "test_kv.db"
#define CORRUPT_DB_FILE "corrupt_kv.db"

static void test_save_and_load() {
    HashMap *map = hashmap_create(16);
    assert(map != NULL);

    assert(hashmap_put(map, "firstname", "Gottfried"));
    assert(hashmap_put(map, "middlename", "Wilhelm"));
    assert(hashmap_put(map, "lastname", "Leibniz"));
    assert(hashmap_size(map) == 3);

    assert(db_save(map, TEST_DB_FILE));

    HashMap *loaded_map = hashmap_create(16);
    assert(loaded_map != NULL);
    assert(db_load(loaded_map, TEST_DB_FILE));

    assert(strcmp((const char *)hashmap_get(loaded_map, "firstname"), "Gottfried") == 0);
    assert(strcmp((const char *)hashmap_get(loaded_map, "middlename"), "Wilhelm") == 0);
    assert(strcmp((const char *)hashmap_get(loaded_map, "lastname"), "Leibniz") == 0);

    hashmap_destroy(map);
    hashmap_destroy(loaded_map);
    remove(TEST_DB_FILE);
}

static void test_load_nonexistent_file() {
    HashMap *map = hashmap_create(16);
    assert(map != NULL);

    assert(!db_load(map, TEST_DB_FILE));

    hashmap_destroy(map);
}

static void test_load_invalid_magic_bytes(void) {
    // Write a file with invalid magic bytes ("FAIL")
    FILE *fp = fopen(CORRUPT_DB_FILE, "wb");
    assert(fp != NULL);

    DBHeader bad_header = {
        .magic = {'F', 'A', 'I', 'L'}, .version = KV_VERSION, .reserved = 0, .record_count = 0};
    fwrite(&bad_header, sizeof(DBHeader), 1, fp);
    fclose(fp);

    HashMap *map = hashmap_create(16);
    assert(map != NULL);

    // Should fail header validation check
    assert(db_load(map, CORRUPT_DB_FILE) == false);

    hashmap_destroy(map);
    remove(CORRUPT_DB_FILE);
}

static void test_load_truncated_file(void) {
    // Write incomplete header (partial bytes)
    FILE *fp = fopen(CORRUPT_DB_FILE, "wb");
    assert(fp != NULL);
    char partial_data[] = {'K', 'V'};
    fwrite(partial_data, 1, sizeof(partial_data), fp);
    fclose(fp);

    HashMap *map = hashmap_create(16);
    assert(map != NULL);

    // Should fail due to fread short read
    assert(!db_load(map, CORRUPT_DB_FILE));

    hashmap_destroy(map);
    remove(CORRUPT_DB_FILE);
}

int main(void) {
    printf("=== Running DB Unit Tests ===\n");

    RUN_TEST(test_save_and_load);
    RUN_TEST(test_load_nonexistent_file);
    RUN_TEST(test_load_invalid_magic_bytes);
    RUN_TEST(test_load_truncated_file);

    printf("=== All DB Unit Tests Passed ===\n");
    return 0;
}