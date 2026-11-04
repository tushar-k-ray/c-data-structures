#ifndef DSA_STACK_H
#define DSA_STACK_H

#include "common.h"

typedef struct StackNode {
    int data;
    struct StackNode* next;
} StackNode;

typedef struct Stack {
    StackNode* top;
    size_t size;
} Stack;

/**
 * Creates and initializes a heap-allocated dynamic pointer-based Stack.
 */
Stack* stack_create(void);

/**
 * Destroys all nodes in the stack and frees the stack itself. Sets *stack_ptr to NULL.
 */
void stack_destroy(Stack** stack_ptr);

/**
 * Removes all elements from the stack.
 */
void stack_clear(Stack* stack);

/**
 * Pushes a new element onto the top of the stack. O(1)
 */
StatusCode stack_push(Stack* stack, int value);

/**
 * Pops the top element from the stack, storing it in out_value. O(1)
 */
StatusCode stack_pop(Stack* stack, int* out_value);

/**
 * Peeks at the top element without removing it. O(1)
 */
StatusCode stack_peek(const Stack* stack, int* out_value);

/**
 * Returns true if the stack is empty or NULL.
 */
bool stack_is_empty(const Stack* stack);

/**
 * Returns the current number of elements on the stack.
 */
size_t stack_size(const Stack* stack);

/**
 * Prints the stack visually from top to bottom.
 */
void stack_print(const Stack* stack);

/* ========================================================================= */
/*                   APPLICATIONS / ALGORITHMS                               */
/* ========================================================================= */

/**
 * Verifies if parentheses/brackets in an expression are balanced.
 * Supports (), [], {}.
 * Demonstrates character stack pointer manipulation.
 */
StatusCode stack_is_balanced_parentheses(const char* expression, bool* out_is_balanced);

/**
 * Evaluates a postfix (Reverse Polish Notation) expression.
 * E.g., "10 4 + 2 *" evaluates to 28.
 */
StatusCode stack_eval_postfix(const char* postfix, int* out_result);

#endif /* DSA_STACK_H */
