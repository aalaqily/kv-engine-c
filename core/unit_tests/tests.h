#ifndef KV_TESTS_H
#define KV_TESTS_H

#include <stddef.h>
#include <stdio.h>

static size_t tests_count = 0;

#define RUN_TEST(test_func)                                                    \
    do {                                                                       \
        tests_count++;                                                         \
        printf("[%zu] Running %s...", tests_count, #test_func);                \
        test_func();                                                           \
        printf(" PASSED\n");                                                   \
    } while (0)

#endif // KV_TESTS_H