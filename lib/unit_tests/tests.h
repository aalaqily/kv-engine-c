#ifndef KV_TESTS_H
#define KV_TESTS_H

#include <stddef.h>
#include <stdio.h>

static size_t tests_count = 0;

#define BLUE_TEXT "\033[34m"
#define GREEN_TEXT "\033[32m"
#define NORMAL_TEXT "\033[0m"


#define RUN_TEST(test_func)                                                                   \
    do {                                                                                      \
        tests_count++;                                                                        \
        printf("[%zu] %s %sRunning...%s\n", tests_count, #test_func, BLUE_TEXT, NORMAL_TEXT); \
        test_func();                                                                          \
        printf("[%zu] %s %sPassed%s\n", tests_count, #test_func, GREEN_TEXT, NORMAL_TEXT);    \
    } while (0)

#endif // KV_TESTS_H
