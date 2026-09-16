#include "hashmap_iterator.h"
#include "tests.h"
#include <assert.h>
#include <string.h>

static void test_create_destroy(void) {
    HashMap *map = hashmap_create(16);
    HashMapIterator *iter = hashmap_iterator_create(map);

    assert(iter);

    hashmap_destroy(map);
    hashmap_iterator_destroy(iter);
}

static void test_empty_map(void) {
    HashMap *map = hashmap_create(16);
    HashMapIterator *iter = hashmap_iterator_create(map);

    assert(iter);
    assert(!hashmap_iterator_next(iter));

    hashmap_destroy(map);
    hashmap_iterator_destroy(iter);
}

static void test_single_entry_map(void) {
    HashMap *map = hashmap_create(16);
    hashmap_put(map, "key1", "value1");
    HashMapIterator *iter = hashmap_iterator_create(map);

    assert(iter);
    assert(hashmap_iterator_next(iter));
    assert(strcmp(hashmap_iterator_current_key(iter), "key1") == 0);
    assert(strcmp(hashmap_iterator_current_value(iter), "value1") == 0);
    assert(!hashmap_iterator_next(iter));

    hashmap_destroy(map);
    hashmap_iterator_destroy(iter);
}

static void test_multi_entry_map(void) {
    HashMap *map = hashmap_create(16);
    hashmap_put(map, "key1", "value1");
    hashmap_put(map, "key2", "value2");
    hashmap_put(map, "key3", "value3");
    hashmap_put(map, "key4", "value4");
    HashMapIterator *iter = hashmap_iterator_create(map);

    assert(iter);

    size_t count = 0;
    while (hashmap_iterator_next(iter))
        count++;
    assert(count == hashmap_size(map));

    hashmap_destroy(map);
    hashmap_iterator_destroy(iter);
}

static void test_1_capacity_map(void) {
    HashMap *map = hashmap_create(1);
    hashmap_put(map, "key1", "value1");
    hashmap_resize_to(map, 1);
    printf("[DEBUG] HashMap capacity = %li\n", hashmap_capacity(map));
    HashMapIterator *iter = hashmap_iterator_create(map);

    assert(iter);
    assert(hashmap_iterator_next(iter));
    assert(strcmp(hashmap_iterator_current_key(iter), "key1") == 0);
    assert(strcmp(hashmap_iterator_current_value(iter), "value1") == 0);
    assert(!hashmap_iterator_next(iter));

    hashmap_destroy(map);
    hashmap_iterator_destroy(iter);
}

int main(void) {
    printf("=== Running HashMapIterator Unit Tests ===\n");

    RUN_TEST(test_create_destroy);
    RUN_TEST(test_empty_map);
    RUN_TEST(test_single_entry_map);
    RUN_TEST(test_multi_entry_map);
    RUN_TEST(test_1_capacity_map);

    printf("=== All HashMapIterator Unit Tests Passed ===\n");
    return 0;
}
