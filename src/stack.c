#include "stack.h"
#include <ctype.h>

Stack* stack_create(void) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (!stack) return NULL;
    stack->top = NULL;
    stack->size = 0;
    return stack;
}

void stack_clear(Stack* stack) {
    if (!stack) return;

    StackNode* curr = stack->top;
    while (curr != NULL) {
        StackNode* temp = curr;
        curr = curr->next;
        free(temp);
    }

    stack->top = NULL;
    stack->size = 0;
}

void stack_destroy(Stack** stack_ptr) {
    if (!stack_ptr || !*stack_ptr) return;
    stack_clear(*stack_ptr);
    free(*stack_ptr);
    *stack_ptr = NULL;
}

StatusCode stack_push(Stack* stack, int value) {
    if (!stack) return STATUS_ERR_NULL_PTR;

    StackNode* new_node = (StackNode*)malloc(sizeof(StackNode));
    if (!new_node) return STATUS_ERR_ALLOC;

    new_node->data = value;
    new_node->next = stack->top;
    stack->top = new_node;
    stack->size++;

    return STATUS_OK;
}

StatusCode stack_pop(Stack* stack, int* out_value) {
    if (!stack) return STATUS_ERR_NULL_PTR;
    if (stack->top == NULL) return STATUS_ERR_EMPTY;

    StackNode* temp = stack->top;
    if (out_value) {
        *out_value = temp->data;
    }
    stack->top = temp->next;
    free(temp);
    stack->size--;

    return STATUS_OK;
}

StatusCode stack_peek(const Stack* stack, int* out_value) {
    if (!stack || !out_value) return STATUS_ERR_NULL_PTR;
    if (stack->top == NULL) return STATUS_ERR_EMPTY;

    *out_value = stack->top->data;
    return STATUS_OK;
}

bool stack_is_empty(const Stack* stack) {
    return !stack || stack->size == 0;
}

size_t stack_size(const Stack* stack) {
    return stack ? stack->size : 0;
}

void stack_print(const Stack* stack) {
    if (!stack) {
        printf("(null stack)\n");
        return;
    }
    if (stack->top == NULL) {
        printf("[Empty Stack]\n");
        return;
    }

    printf("--- TOP ---\n");
    const StackNode* curr = stack->top;
    while (curr != NULL) {
        printf("  | %5d |\n", curr->data);
        curr = curr->next;
    }
    printf("--- BOTTOM --- (size: %zu)\n", stack->size);
}

/* ========================================================================= */
/*                      APPLICATIONS                                         */
/* ========================================================================= */

StatusCode stack_is_balanced_parentheses(const char* expression, bool* out_is_balanced) {
    if (!expression || !out_is_balanced) return STATUS_ERR_NULL_PTR;

    Stack* s = stack_create();
    if (!s) return STATUS_ERR_ALLOC;

    bool balanced = true;

    for (size_t i = 0; expression[i] != '\0'; i++) {
        char ch = expression[i];

        if (ch == '(' || ch == '{' || ch == '[') {
            stack_push(s, (int)ch);
        } else if (ch == ')' || ch == '}' || ch == ']') {
            if (stack_is_empty(s)) {
                balanced = false;
                break;
            }
            int top_ch = 0;
            stack_pop(s, &top_ch);

            if ((ch == ')' && top_ch != '(') ||
                (ch == '}' && top_ch != '{') ||
                (ch == ']' && top_ch != '[')) {
                balanced = false;
                break;
            }
        }
    }

    if (!stack_is_empty(s)) {
        balanced = false;
    }

    stack_destroy(&s);
    *out_is_balanced = balanced;
    return STATUS_OK;
}

StatusCode stack_eval_postfix(const char* postfix, int* out_result) {
    if (!postfix || !out_result) return STATUS_ERR_NULL_PTR;

    Stack* s = stack_create();
    if (!s) return STATUS_ERR_ALLOC;

    const char* p = postfix;
    while (*p != '\0') {
        if (isspace((unsigned char)*p)) {
            p++;
            continue;
        }

        // Check if number (could be negative if followed by digit)
        if (isdigit((unsigned char)*p) || (*p == '-' && isdigit((unsigned char)*(p + 1)))) {
            char* endptr = NULL;
            long val = strtol(p, &endptr, 10);
            p = endptr;
            stack_push(s, (int)val);
        } else if (*p == '+' || *p == '-' || *p == '*' || *p == '/') {
            char op = *p;
            p++;

            int op2 = 0, op1 = 0;
            if (stack_pop(s, &op2) != STATUS_OK || stack_pop(s, &op1) != STATUS_OK) {
                stack_destroy(&s);
                return STATUS_ERR_INVALID_ARG;
            }

            int res = 0;
            switch (op) {
                case '+': res = op1 + op2; break;
                case '-': res = op1 - op2; break;
                case '*': res = op1 * op2; break;
                case '/':
                    if (op2 == 0) {
                        stack_destroy(&s);
                        return STATUS_ERR_INVALID_ARG; // divide by zero
                    }
                    res = op1 / op2;
                    break;
            }
            stack_push(s, res);
        } else {
            stack_destroy(&s);
            return STATUS_ERR_INVALID_ARG;
        }
    }

    if (stack_size(s) != 1) {
        stack_destroy(&s);
        return STATUS_ERR_INVALID_ARG;
    }

    stack_pop(s, out_result);
    stack_destroy(&s);
    return STATUS_OK;
}
