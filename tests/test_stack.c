#include "test_harness.h"
#include "stack.h"

void test_stack_core_operations(void) {
    Stack* s = stack_create();
    TEST_ASSERT(s != NULL, "stack_create allocates stack");
    TEST_ASSERT(stack_is_empty(s) == true, "new stack is empty");
    TEST_ASSERT(stack_size(s) == 0, "new stack size is 0");

    int val = 0;
    TEST_ASSERT(stack_pop(s, &val) == STATUS_ERR_EMPTY, "pop on empty stack returns STATUS_ERR_EMPTY");
    TEST_ASSERT(stack_peek(s, &val) == STATUS_ERR_EMPTY, "peek on empty stack returns STATUS_ERR_EMPTY");

    // Push elements
    stack_push(s, 100);
    stack_push(s, 200);
    stack_push(s, 300);

    TEST_ASSERT(stack_size(s) == 3, "stack size is 3");
    TEST_ASSERT(stack_peek(s, &val) == STATUS_OK && val == 300, "peek yields top element 300");

    // Pop elements (LIFO order)
    TEST_ASSERT(stack_pop(s, &val) == STATUS_OK && val == 300, "pop 1 yields 300");
    TEST_ASSERT(stack_pop(s, &val) == STATUS_OK && val == 200, "pop 2 yields 200");
    TEST_ASSERT(stack_pop(s, &val) == STATUS_OK && val == 100, "pop 3 yields 100");
    TEST_ASSERT(stack_is_empty(s) == true, "stack is empty after popping all");

    // Push again and clear
    stack_push(s, 42);
    stack_push(s, 84);
    stack_clear(s);
    TEST_ASSERT(stack_is_empty(s) == true, "stack is empty after clear");

    stack_destroy(&s);
    TEST_ASSERT(s == NULL, "stack pointer is NULL after destroy");
}

void test_stack_balanced_parentheses(void) {
    bool balanced = false;

    stack_is_balanced_parentheses("{[()]}", &balanced);
    TEST_ASSERT(balanced == true, "{[()]} should be balanced");

    stack_is_balanced_parentheses("[()]{}{[()()]()}", &balanced);
    TEST_ASSERT(balanced == true, "[()]{}{[()()]()} should be balanced");

    stack_is_balanced_parentheses("(]", &balanced);
    TEST_ASSERT(balanced == false, "(] should NOT be balanced");

    stack_is_balanced_parentheses("((()", &balanced);
    TEST_ASSERT(balanced == false, "((() should NOT be balanced");

    stack_is_balanced_parentheses(")", &balanced);
    TEST_ASSERT(balanced == false, ") should NOT be balanced");

    stack_is_balanced_parentheses("", &balanced);
    TEST_ASSERT(balanced == true, "empty string should be balanced");

    stack_is_balanced_parentheses("int a = (b + c) * [x - y];", &balanced);
    TEST_ASSERT(balanced == true, "code snippet with balanced brackets");
}

void test_stack_postfix_eval(void) {
    int result = 0;

    TEST_ASSERT(stack_eval_postfix("5 3 +", &result) == STATUS_OK && result == 8, "5 3 + evaluates to 8");
    TEST_ASSERT(stack_eval_postfix("10 4 + 2 *", &result) == STATUS_OK && result == 28, "10 4 + 2 * evaluates to 28");
    TEST_ASSERT(stack_eval_postfix("50 20 - 5 /", &result) == STATUS_OK && result == 6, "50 20 - 5 / evaluates to 6");
    TEST_ASSERT(stack_eval_postfix("10 0 /", &result) == STATUS_ERR_INVALID_ARG, "divide by zero returns error");
    TEST_ASSERT(stack_eval_postfix("5 +", &result) == STATUS_ERR_INVALID_ARG, "malformed expression returns error");
}

void run_stack_tests(void) {
    test_stack_core_operations();
    test_stack_balanced_parentheses();
    test_stack_postfix_eval();
}
