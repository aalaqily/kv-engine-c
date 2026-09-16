#include <assert.h>
#include <string.h>
#include "tests.h"
#include "kv_engine.h"

static void test_create_destroy(void) {
    KVEHashMap *map = kve_map_create(16);
    KVEIterator *iter = kve_iter_create(map);

    assert(iter);

    kve_map_destroy(map);
    kve_iter_destroy(iter);
}

static void test_empty_map(void) {
    KVEHashMap *map = kve_map_create(16);
    KVEIterator *iter = kve_iter_create(map);

    assert(iter);
    assert(!kve_iter_next(iter));

    kve_map_destroy(map);
    kve_iter_destroy(iter);
}

static void test_single_entry_map(void) {
    KVEHashMap *map = kve_map_create(16);
    kve_map_put(map, "key1", "value1");
    KVEIterator *iter = kve_iter_create(map);

    assert(iter);
    assert(kve_iter_next(iter));
    assert(strcmp(kve_iter_key(iter), "key1") == 0);
    assert(strcmp(kve_iter_value(iter), "value1") == 0);
    assert(!kve_iter_next(iter));

    kve_map_destroy(map);
    kve_iter_destroy(iter);
}

static void test_multi_entry_map(void) {
    KVEHashMap *map = kve_map_create(16);
    kve_map_put(map, "key1", "value1");
    kve_map_put(map, "key2", "value2");
    kve_map_put(map, "key3", "value3");
    kve_map_put(map, "key4", "value4");
    KVEIterator *iter = kve_iter_create(map);

    assert(iter);

    size_t count = 0;
    while (kve_iter_next(iter))
        count++;
    assert(count == kve_map_size(map));

    kve_map_destroy(map);
    kve_iter_destroy(iter);
}

static void test_1_capacity_map(void) {
    KVEHashMap *map = kve_map_create(1);
    kve_map_put(map, "key1", "value1");
    kve_map_resize_to(map, 1);
    printf("[DEBUG] KVEHashMap capacity = %zu\n", kve_map_capacity(map));
    KVEIterator *iter = kve_iter_create(map);

    assert(iter);
    assert(kve_iter_next(iter));
    assert(strcmp(kve_iter_key(iter), "key1") == 0);
    assert(strcmp(kve_iter_value(iter), "value1") == 0);
    assert(!kve_iter_next(iter));

    kve_map_destroy(map);
    kve_iter_destroy(iter);
}

int main(void) {
    printf("=== Running KVEIterator Unit Tests ===\n");

    RUN_TEST(test_create_destroy);
    RUN_TEST(test_empty_map);
    RUN_TEST(test_single_entry_map);
    RUN_TEST(test_multi_entry_map);
    RUN_TEST(test_1_capacity_map);

    printf("=== All KVEIterator Unit Tests Passed ===\n");
    return 0;
}
