#include <assert.h>
#include <string.h>
#include "tests.h"
#include "kv_engine.h"

static void test_create_destroy(void) {

    KVEHashMap *map = kve_map_create(32);

    assert(map);

    assert(kve_map_size(map) == 0);
    assert(kve_map_capacity(map) == 32);

    kve_map_destroy(map);
}

static void test_resize_to(void) {
    KVEHashMap *map = kve_map_create(32);
    kve_map_resize_to(map, 32);

    assert(kve_map_capacity(map) == 32);

    kve_map_destroy(map);
}

static void test_put_and_get(void) {

    KVEHashMap *map = kve_map_create(16);

    kve_map_put(map, "John", "Doe");

    const char *value = kve_map_get(map, "John");
    assert(strcmp(value, "Doe") == 0);

    kve_map_put(map, "John", "Harvard");

    value = kve_map_get(map, "John");
    assert(strcmp(value, "Harvard") == 0);

    assert(kve_map_size(map) == 1);

    kve_map_destroy(map);
}

static void test_dynamic_resize() {
    KVEHashMap *map = kve_map_create(4);

    kve_map_put(map, "key1", "value1");
    kve_map_put(map, "key2", "value2");
    kve_map_put(map, "key3", "value3");
    kve_map_put(map, "key4", "value4");

    assert(kve_map_capacity(map) == 8);

    kve_map_destroy(map);
}

static void test_collisions(void) {

    KVEHashMap *map = kve_map_create(2);

    kve_map_put(map, "key1", "value1");
    kve_map_put(map, "key2", "value2");
    kve_map_put(map, "key3", "value3");
    kve_map_put(map, "key4", "value4");

    assert(strcmp(kve_map_get(map, "key1"), "value1") == 0);
    assert(strcmp(kve_map_get(map, "key2"), "value2") == 0);
    assert(strcmp(kve_map_get(map, "key3"), "value3") == 0);
    assert(strcmp(kve_map_get(map, "key4"), "value4") == 0);

    assert(kve_map_contains(map, "key1"));
    assert(kve_map_contains(map, "key2"));
    assert(kve_map_contains(map, "key3"));
    assert(kve_map_contains(map, "key4"));

    assert(kve_map_size(map) == 4);

    kve_map_destroy(map);
}

static void test_remove(void) {

    KVEHashMap *map = kve_map_create(16);
    kve_map_put(map, "key1", "value1");
    kve_map_put(map, "key2", "value2");
    kve_map_put(map, "key3", "value3");

    bool key2_removed = kve_map_remove(map, "key2");
    bool key1_removed = kve_map_remove(map, "key1");

    assert(key1_removed);
    assert(key2_removed);

    assert(kve_map_get(map, "key1") == NULL);
    assert(kve_map_get(map, "key2") == NULL);

    assert(!kve_map_contains(map, "key1"));
    assert(!kve_map_contains(map, "key2"));

    assert(kve_map_size(map) == 1);

    bool invalid_remove = kve_map_remove(map, "key4");
    assert(!invalid_remove);

    kve_map_destroy(map);
}

int main(void) {
    printf("=== Running KVEHashMap Unit Tests ===\n");

    RUN_TEST(test_create_destroy);
    RUN_TEST(test_resize_to);
    RUN_TEST(test_put_and_get);
    RUN_TEST(test_dynamic_resize);
    RUN_TEST(test_collisions);
    RUN_TEST(test_remove);

    printf("=== All KVEHashMap Unit Tests Passed ===\n");
    return 0;
}
