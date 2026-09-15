#include "hashmap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define RUN_TEST(test_func)                                     \
    do {                                                        \
        tests_count++;                                          \
        printf("[%d] Running %s...", tests_count, #test_func);  \
        test_func();                                            \
        printf(" PASSED\n");                                    \
    } while(0)

static void test_create_destroy(void) {

    HashMap *map = hashmap_create(32);

    assert(map);

    assert(hashmap_size(map) == 0);
    assert(hashmap_capacity(map) == 32);

    hashmap_destroy(map);
}

static void test_resize_to(void) {
    HashMap *map = hashmap_create(32);
    hashmap_resize_to(map, 32);

    assert(hashmap_capacity(map) == 32);

    hashmap_destroy(map);
}

static void test_put_and_get(void) {

    HashMap *map = hashmap_create(16);

    hashmap_put(map, "John", "Doe");

    char *value = hashmap_get(map, "John");
    assert(strcmp(value, "Doe") == 0);

    hashmap_put(map, "John", "Harvard");

    value = hashmap_get(map, "John");
    assert(strcmp(value, "Harvard") == 0);

    assert(hashmap_size(map) == 1);

    hashmap_destroy(map);
}

static void test_dynamic_resize() {
    HashMap *map = hashmap_create(4);

    hashmap_put(map, "key1", (void *) 1);
    hashmap_put(map, "key2", (void *) 2);
    hashmap_put(map, "key3", (void *) 3);
    hashmap_put(map, "key4", (void *) 4);

    assert(hashmap_capacity(map) == 8);

    hashmap_destroy(map);
}

static void test_collisions(void) {

    HashMap *map = hashmap_create(2);

    hashmap_put(map, "key1", (void *) 1);
    hashmap_put(map, "key2", (void *) 2);
    hashmap_put(map, "key3", (void *) 3);
    hashmap_put(map, "key4", (void *) 4);

    assert(hashmap_get(map, "key1") == (void *) 1);
    assert(hashmap_get(map, "key2") == (void *) 2);
    assert(hashmap_get(map, "key3") == (void *) 3);
    assert(hashmap_get(map, "key4") == (void *) 4);

    assert(hashmap_contains(map, "key1"));
    assert(hashmap_contains(map, "key2"));
    assert(hashmap_contains(map, "key3"));
    assert(hashmap_contains(map, "key4"));

    assert(hashmap_size(map) == 4);

    hashmap_destroy(map);
}

static void test_remove(void) {

    HashMap *map = hashmap_create(16);
    hashmap_put(map, "key1", (void *) 1);
    hashmap_put(map, "key2", (void *) 2);
    hashmap_put(map, "key3", (void *) 3);

    bool key2_removed = hashmap_remove(map, "key2");
    bool key1_removed = hashmap_remove(map, "key1");

    assert(key1_removed);
    assert(key2_removed);

    assert(hashmap_get(map, "key1") == NULL);
    assert(hashmap_get(map, "key2") == NULL);

    assert(!hashmap_contains(map, "key1"));
    assert(!hashmap_contains(map, "key2"));

    assert(hashmap_size(map) == 1);

    bool invalid_remove = hashmap_remove(map, "key4");
    assert(!invalid_remove);

    hashmap_destroy(map);
}

static size_t tests_count = 0;

int main(void) {
    printf("=== Running HashMap Unit Tests ===\n");

    RUN_TEST(test_create_destroy);
    RUN_TEST(test_resize_to);
    RUN_TEST(test_put_and_get);
    RUN_TEST(test_dynamic_resize);
    RUN_TEST(test_collisions);
    RUN_TEST(test_remove);

    printf("=== All HashMap Unit Tests Passed ===\n");
    return 0;
}
