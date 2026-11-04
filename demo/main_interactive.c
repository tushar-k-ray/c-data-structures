#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"
#include "stack.h"
#include "queue.h"
#include "binary_tree.h"

static void print_node_val(int val) {
    printf("%d ", val);
}

static void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* ========================================================================= */
/*                          LINKED LIST MENU                                 */
/* ========================================================================= */
static void menu_linked_list(void) {
    SList* list = slist_create();
    int choice = 0;
    int val = 0;
    size_t idx = 0;

    while (1) {
        printf("\n========================================\n");
        printf("       SINGLY LINKED LIST MENU          \n");
        printf("========================================\n");
        slist_print(list);
        printf("\n1. Push Front\n");
        printf("2. Push Back\n");
        printf("3. Insert at Index\n");
        printf("4. Insert in Sorted Order\n");
        printf("5. Pop Front\n");
        printf("6. Pop Back\n");
        printf("7. Delete by Value\n");
        printf("8. Reverse List (In-Place)\n");
        printf("9. Check for Cycle\n");
        printf("0. Return to Main Menu\n");
        printf("Select option: ");

        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }

        if (choice == 0) break;

        switch (choice) {
            case 1:
                printf("Enter value to push front: ");
                if (scanf("%d", &val) == 1) {
                    slist_push_front(list, val);
                }
                break;
            case 2:
                printf("Enter value to push back: ");
                if (scanf("%d", &val) == 1) {
                    slist_push_back(list, val);
                }
                break;
            case 3:
                printf("Enter index: ");
                if (scanf("%zu", &idx) == 1) {
                    printf("Enter value: ");
                    if (scanf("%d", &val) == 1) {
                        StatusCode sc = slist_insert_at(list, idx, val);
                        if (sc != STATUS_OK) printf("Error: %s\n", status_to_string(sc));
                    }
                }
                break;
            case 4:
                printf("Enter value to insert sorted: ");
                if (scanf("%d", &val) == 1) {
                    slist_insert_sorted(list, val);
                }
                break;
            case 5:
                if (slist_pop_front(list, &val) == STATUS_OK) {
                    printf("Popped front value: %d\n", val);
                } else {
                    printf("List is empty!\n");
                }
                break;
            case 6:
                if (slist_pop_back(list, &val) == STATUS_OK) {
                    printf("Popped back value: %d\n", val);
                } else {
                    printf("List is empty!\n");
                }
                break;
            case 7:
                printf("Enter value to delete: ");
                if (scanf("%d", &val) == 1) {
                    StatusCode sc = slist_delete_value(list, val);
                    if (sc == STATUS_OK) {
                        printf("Deleted value %d\n", val);
                    } else {
                        printf("Value %d not found in list\n", val);
                    }
                }
                break;
            case 8:
                slist_reverse(list);
                printf("Reversed list in-place!\n");
                break;
            case 9:
                printf("Floyd's Cycle Detection: %s\n",
                       slist_has_cycle(list) ? "Cycle detected!" : "No cycle detected (safe).");
                break;
            default:
                printf("Invalid option.\n");
        }
    }

    slist_destroy(&list);
}

/* ========================================================================= */
/*                             STACK MENU                                    */
/* ========================================================================= */
static void menu_stack(void) {
    Stack* s = stack_create();
    int choice = 0;
    int val = 0;
    char buffer[256];

    while (1) {
        printf("\n========================================\n");
        printf("               STACK MENU               \n");
        printf("========================================\n");
        stack_print(s);
        printf("\n1. Push\n");
        printf("2. Pop\n");
        printf("3. Peek\n");
        printf("4. Clear\n");
        printf("5. Balanced Parentheses Checker\n");
        printf("6. Evaluate Postfix Expression\n");
        printf("0. Return to Main Menu\n");
        printf("Select option: ");

        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }

        if (choice == 0) break;

        switch (choice) {
            case 1:
                printf("Enter value to push: ");
                if (scanf("%d", &val) == 1) {
                    stack_push(s, val);
                }
                break;
            case 2:
                if (stack_pop(s, &val) == STATUS_OK) {
                    printf("Popped: %d\n", val);
                } else {
                    printf("Stack is empty!\n");
                }
                break;
            case 3:
                if (stack_peek(s, &val) == STATUS_OK) {
                    printf("Top element: %d\n", val);
                } else {
                    printf("Stack is empty!\n");
                }
                break;
            case 4:
                stack_clear(s);
                printf("Stack cleared.\n");
                break;
            case 5:
                printf("Enter expression with brackets (e.g. {[()]}): ");
                clear_input_buffer();
                if (fgets(buffer, sizeof(buffer), stdin)) {
                    buffer[strcspn(buffer, "\r\n")] = 0;
                    bool balanced = false;
                    stack_is_balanced_parentheses(buffer, &balanced);
                    printf("Result: Expression '%s' is %s\n",
                           buffer, balanced ? "BALANCED (Valid)" : "NOT BALANCED (Invalid)");
                }
                break;
            case 6:
                printf("Enter postfix expression (e.g. 10 4 + 2 *): ");
                clear_input_buffer();
                if (fgets(buffer, sizeof(buffer), stdin)) {
                    buffer[strcspn(buffer, "\r\n")] = 0;
                    int result = 0;
                    StatusCode sc = stack_eval_postfix(buffer, &result);
                    if (sc == STATUS_OK) {
                        printf("Postfix evaluation result: %d\n", result);
                    } else {
                        printf("Evaluation error: %s\n", status_to_string(sc));
                    }
                }
                break;
            default:
                printf("Invalid option.\n");
        }
    }

    stack_destroy(&s);
}

/* ========================================================================= */
/*                             QUEUE MENU                                    */
/* ========================================================================= */
static void menu_queue(void) {
    Queue* q = queue_create();
    int choice = 0;
    int val = 0;

    while (1) {
        printf("\n========================================\n");
        printf("               QUEUE MENU               \n");
        printf("========================================\n");
        queue_print(q);
        printf("\n1. Enqueue (Push to rear)\n");
        printf("2. Dequeue (Pop from front)\n");
        printf("3. Peek Front\n");
        printf("4. Clear\n");
        printf("0. Return to Main Menu\n");
        printf("Select option: ");

        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }

        if (choice == 0) break;

        switch (choice) {
            case 1:
                printf("Enter value to enqueue: ");
                if (scanf("%d", &val) == 1) {
                    queue_enqueue(q, val);
                }
                break;
            case 2:
                if (queue_dequeue(q, &val) == STATUS_OK) {
                    printf("Dequeued: %d\n", val);
                } else {
                    printf("Queue is empty!\n");
                }
                break;
            case 3:
                if (queue_peek(q, &val) == STATUS_OK) {
                    printf("Front element: %d\n", val);
                } else {
                    printf("Queue is empty!\n");
                }
                break;
            case 4:
                queue_clear(q);
                printf("Queue cleared.\n");
                break;
            default:
                printf("Invalid option.\n");
        }
    }

    queue_destroy(&q);
}

/* ========================================================================= */
/*                          BINARY TREE MENU                                 */
/* ========================================================================= */
static void menu_binary_tree(void) {
    BSTree* tree = bst_create();
    int choice = 0;
    int val = 0, val2 = 0;

    // Prepopulate with a balanced tree demo for convenience
    bst_insert(tree, 50);
    bst_insert(tree, 30);
    bst_insert(tree, 70);
    bst_insert(tree, 20);
    bst_insert(tree, 40);
    bst_insert(tree, 60);
    bst_insert(tree, 80);

    while (1) {
        printf("\n========================================\n");
        printf("        BINARY SEARCH TREE MENU         \n");
        printf("========================================\n");
        bst_print_ascii(tree);
        printf("Tree size: %zu | Height: %d\n", bst_size(tree), bst_height(tree));
        printf("\n1. Insert Value\n");
        printf("2. Delete Value\n");
        printf("3. Search Value\n");
        printf("4. In-Order Traversal (Sorted)\n");
        printf("5. Pre-Order Traversal\n");
        printf("6. Post-Order Traversal\n");
        printf("7. Level-Order Traversal (BFS)\n");
        printf("8. Find Min and Max\n");
        printf("9. Find Lowest Common Ancestor (LCA)\n");
        printf("0. Return to Main Menu\n");
        printf("Select option: ");

        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }

        if (choice == 0) break;

        switch (choice) {
            case 1:
                printf("Enter integer value to insert: ");
                if (scanf("%d", &val) == 1) {
                    StatusCode sc = bst_insert(tree, val);
                    if (sc == STATUS_OK) {
                        printf("Inserted %d successfully.\n", val);
                    } else {
                        printf("Insert error: %s\n", status_to_string(sc));
                    }
                }
                break;
            case 2:
                printf("Enter integer value to delete: ");
                if (scanf("%d", &val) == 1) {
                    StatusCode sc = bst_delete(tree, val);
                    if (sc == STATUS_OK) {
                        printf("Deleted %d successfully.\n", val);
                    } else {
                        printf("Delete error: %s\n", status_to_string(sc));
                    }
                }
                break;
            case 3:
                printf("Enter value to search: ");
                if (scanf("%d", &val) == 1) {
                    bool found = bst_search(tree, val);
                    printf("Value %d is %s in the BST.\n", val, found ? "FOUND" : "NOT FOUND");
                }
                break;
            case 4:
                printf("In-Order:   [ ");
                bst_inorder(tree, print_node_val);
                printf("]\n");
                break;
            case 5:
                printf("Pre-Order:  [ ");
                bst_preorder(tree, print_node_val);
                printf("]\n");
                break;
            case 6:
                printf("Post-Order: [ ");
                bst_postorder(tree, print_node_val);
                printf("]\n");
                break;
            case 7:
                printf("Level-Order (BFS): [ ");
                bst_levelorder(tree, print_node_val);
                printf("]\n");
                break;
            case 8: {
                int min_v = 0, max_v = 0;
                if (bst_min(tree, &min_v) == STATUS_OK && bst_max(tree, &max_v) == STATUS_OK) {
                    printf("Min value = %d | Max value = %d\n", min_v, max_v);
                } else {
                    printf("Tree is empty.\n");
                }
                break;
            }
            case 9:
                printf("Enter two node values (e.g. 20 40): ");
                if (scanf("%d %d", &val, &val2) == 2) {
                    int lca = 0;
                    StatusCode sc = bst_lowest_common_ancestor(tree, val, val2, &lca);
                    if (sc == STATUS_OK) {
                        printf("Lowest Common Ancestor of %d and %d is: %d\n", val, val2, lca);
                    } else {
                        printf("LCA error: %s\n", status_to_string(sc));
                    }
                }
                break;
            default:
                printf("Invalid option.\n");
        }
    }

    bst_destroy(&tree);
}

/* ========================================================================= */
/*                                MAIN                                       */
/* ========================================================================= */
int main(void) {
    int choice = 0;

    while (1) {
        printf("\n=======================================================\n");
        printf("   CORE DATA STRUCTURES IN C (POINTERS & DYNAMIC MEM)  \n");
        printf("=======================================================\n");
        printf("1. Singly & Doubly Linked Lists\n");
        printf("2. Stack (LIFO) & Applications\n");
        printf("3. Queue (FIFO)\n");
        printf("4. Binary Search Tree (BST) & Visualizer\n");
        printf("0. Exit\n");
        printf("Choose data structure to explore: ");

        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }

        if (choice == 0) {
            printf("\nExiting. Thank you!\n");
            break;
        }

        switch (choice) {
            case 1: menu_linked_list(); break;
            case 2: menu_stack(); break;
            case 3: menu_queue(); break;
            case 4: menu_binary_tree(); break;
            default: printf("Invalid choice.\n");
        }
    }

    return 0;
}
