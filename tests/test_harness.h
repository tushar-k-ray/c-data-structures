#ifndef TEST_HARNESS_H
#define TEST_HARNESS_H

#include <stdio.h>
#include <stdbool.h>

#define COLOR_GREEN  "\033[32m"
#define COLOR_RED    "\033[31m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_CYAN   "\033[36m"
#define COLOR_RESET  "\033[0m"

extern int g_tests_run;
extern int g_tests_passed;
extern int g_tests_failed;

#define TEST_ASSERT(cond, msg) do { \
    g_tests_run++; \
    if (cond) { \
        g_tests_passed++; \
    } else { \
        g_tests_failed++; \
        printf("  %s[FAIL]%s %s (line %d in %s)\n", COLOR_RED, COLOR_RESET, msg, __LINE__, __FILE__); \
    } \
} while (0)

#define RUN_TEST_SUITE(suite_func, name) do { \
    printf("\n%s=== Running Suite: %s ===%s\n", COLOR_CYAN, name, COLOR_RESET); \
    suite_func(); \
} while (0)

#endif /* TEST_HARNESS_H */
