#include "test_harness.h"

int g_tests_run = 0;
int g_tests_passed = 0;
int g_tests_failed = 0;

// Forward declarations of test suites
void run_linked_list_tests(void);
void run_stack_tests(void);
void run_queue_tests(void);
void run_binary_tree_tests(void);

int main(void) {
    printf("%s=====================================================%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s   DATA STRUCTURES IN C - COMPREHENSIVE TEST SUITE   %s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s=====================================================%s\n", COLOR_CYAN, COLOR_RESET);

    RUN_TEST_SUITE(run_linked_list_tests, "Linked Lists (SList & DList)");
    RUN_TEST_SUITE(run_stack_tests, "Stack & Applications (Parens, Postfix)");
    RUN_TEST_SUITE(run_queue_tests, "Queue (FIFO)");
    RUN_TEST_SUITE(run_binary_tree_tests, "Binary Search Tree (BST)");

    printf("\n%s=====================================================%s\n", COLOR_CYAN, COLOR_RESET);
    printf("                  TEST RESULTS SUMMARY                \n");
    printf("%s=====================================================%s\n", COLOR_CYAN, COLOR_RESET);
    printf("Total Assertions: %d\n", g_tests_run);
    printf("%sPassed:           %d%s\n", COLOR_GREEN, g_tests_passed, COLOR_RESET);

    if (g_tests_failed > 0) {
        printf("%sFailed:           %d%s\n", COLOR_RED, g_tests_failed, COLOR_RESET);
        printf("%sSOME TESTS FAILED!%s\n", COLOR_RED, COLOR_RESET);
        return 1;
    } else {
        printf("%sALL TESTS PASSED WITH ZERO LEAKS AND ZERO FAILURES!%s\n", COLOR_GREEN, COLOR_RESET);
        return 0;
    }
}
