# Core Data Structures & Pointer-Based Dynamic Memory Management in C

[![C11](https://img.shields.io/badge/Language-C11-00599C.svg)](https://en.wikipedia.org/wiki/C11_(C_standard_revision))
[![Build Status](https://img.shields.io/badge/Build-Passing-brightgreen.svg)]()
[![Memory Safety](https://img.shields.io/badge/Memory_Check-AddressSanitizer_Clean-success.svg)]()
[![Tests](https://img.shields.io/badge/Tests-133%2F133_Passed-brightgreen.svg)]()

> **Resume Project Specification:**
> *"Implementations of linked lists, stacks, queues and binary trees, with practice in pointers, dynamic memory allocation and fundamental operations."*

---

## 📌 Project Overview

This repository is an industry-grade, production-style implementation of fundamental data structures in **pure C (C11)**. It was built from the ground up to demonstrate mastery of:
- **Low-level pointer arithmetic and pointer-to-pointer manipulation (`**`)**.
- **Dynamic memory management (`malloc`, `free`)** with strictly zero memory leaks, validated using GCC's **AddressSanitizer (ASan)** and **UndefinedBehaviorSanitizer (UBSan)**.
- **Defensive software engineering**: strict status code error handling (`StatusCode`), `NULL` pointer checks, boundary validations, and encapsulation.
- **Core algorithmic operations**: cycle detection (Floyd's algorithm), in-place reversal, sorted merging, BFS level-order traversals, BST balancing logic, and in-order successor node replacement.

---

## 📂 Repository Structure

```
c-data-structures/
├── Makefile                     # Build system (all, test, demo, clean)
├── README.md                    # Comprehensive technical documentation & interview guide
├── include/                     # Public header definitions & status contracts
│   ├── common.h                 # Global status codes (StatusCode) and utility functions
│   ├── linked_list.h            # Singly (SList) and Doubly (DList) Linked List APIs
│   ├── stack.h                  # Dynamic pointer stack and practical algorithms
│   ├── queue.h                  # Dynamic FIFO queue API
│   └── binary_tree.h            # Binary Search Tree (BST) and ASCII visualization
├── src/                         # Implementations
│   ├── linked_list.c            # SList & DList implementation
│   ├── stack.c                  # Stack implementation + balanced parens + postfix evaluator
│   ├── queue.c                  # FIFO queue implementation
│   └── binary_tree.c            # BST implementation + traversals + LCA + ASCII printer
├── demo/                        # Interactive user interface
│   └── main_interactive.c      # Menu-driven visual terminal CLI
└── tests/                       # Automated unit testing suite
    ├── test_harness.h           # Lightweight test framework with colored status
    ├── test_linked_list.c       # Unit tests for Singly and Doubly linked lists
    ├── test_stack.c             # Unit tests for Stack operations & algorithms
    ├── test_queue.c             # Unit tests for Queue FIFO operations
    ├── test_binary_tree.c       # Unit tests for BST insertions, deletions, LCA, traversals
    └── test_runner.c            # Test suite coordinator (133 assertions)
```

---

## 🚀 Quick Start

### Prerequisites
- GCC / Clang compiler with C11 support
- GNU Make

### Build & Run Unit Tests (with AddressSanitizer)
```bash
make test
```
*Compiles all modules with `-Wall -Wextra -Werror -pedantic -std=c11 -fsanitize=address,undefined` and executes 133 comprehensive assertions.*

### Launch Interactive Visual Terminal Demo
```bash
make demo
```
*Launches an interactive menu to visually insert, delete, reverse, traverse, and inspect linked lists, stacks, queues, and binary search trees in real-time.*

---

## 📊 Data Structures & Complexity Cheatsheet

| Data Structure | Operation | Average Time | Worst Time | Space Complexity | Key Pointer / Memory Technique |
|---|---|---|---|---|---|
| **Singly Linked List** | `push_front` | $O(1)$ | $O(1)$ | $O(1)$ | Head pointer rewiring |
| | `push_back` | $O(n)$ | $O(n)$ | $O(1)$ | Tail traversal, single heap allocation |
| | `insert_at` / `insert_sorted` | $O(n)$ | $O(n)$ | $O(1)$ | Pointer splicing |
| | `pop_front` | $O(1)$ | $O(1)$ | $O(1)$ | Direct node deallocation |
| | `reverse` | $O(n)$ | $O(n)$ | $O(1)$ | In-place 3-pointer swap (`prev`, `curr`, `next`) |
| | `has_cycle` | $O(n)$ | $O(n)$ | $O(1)$ | Floyd's Cycle-Finding Algorithm (Tortoise & Hare) |
| **Doubly Linked List** | `push_front` / `push_back` | $O(1)$ | $O(1)$ | $O(1)$ | Dual head/tail pointer maintenance with `prev`/`next` |
| | `pop_front` / `pop_back` | $O(1)$ | $O(1)$ | $O(1)$ | O(1) detachment and free |
| **Stack (Linked)** | `push` / `pop` / `peek` | $O(1)$ | $O(1)$ | $O(1)$ | LIFO node allocation/deallocation at `top` |
| | Balanced Parentheses | $O(n)$ | $O(n)$ | $O(n)$ | Dynamic character stack matching |
| | Postfix Evaluator | $O(n)$ | $O(n)$ | $O(n)$ | Operands stack arithmetic evaluation |
| **Queue (Linked)** | `enqueue` / `dequeue` | $O(1)$ | $O(1)$ | $O(1)$ | FIFO maintenance with both `front` and `rear` pointers |
| **Binary Search Tree** | `insert` / `search` | $O(\log n)$ | $O(n)$ | $O(\log n)$ (call stack) | Recursive pointer branching |
| | `delete` | $O(\log n)$ | $O(n)$ | $O(\log n)$ | In-order successor swap + subtree reconnection |
| | Traversals (In/Pre/Post) | $O(n)$ | $O(n)$ | $O(h)$ | Recursive depth-first tree walking |
| | Level-Order (BFS) | $O(n)$ | $O(n)$ | $O(w)$ | Node pointer FIFO queue traversal |
| | Lowest Common Ancestor | $O(h)$ | $O(n)$ | $O(1)$ | BST value comparison branching |

---

## 🧠 Memory Management & Pointer Architecture Highlights

### 1. Pointer-to-Pointer (`**`) for Safe Destruction
Dangling pointers are a leading cause of Use-After-Free vulnerabilities in C. To prevent callers from accessing deallocated memory, all destructor functions accept a pointer-to-pointer and set the caller's reference to `NULL`:
```c
void slist_destroy(SList** list_ptr) {
    if (!list_ptr || !*list_ptr) return;
    slist_clear(*list_ptr);
    free(*list_ptr);
    *list_ptr = NULL; // Eliminates dangling pointer in caller scope
}
```

### 2. In-Place Singly Linked List Reversal ($O(1)$ Auxiliary Space)
Reversing a singly linked list without allocating new memory requires carefully tracking three pointers:
```c
StatusCode slist_reverse(SList* list) {
    if (!list) return STATUS_ERR_NULL_PTR;
    if (list->size <= 1) return STATUS_OK;

    SNode* prev = NULL;
    SNode* curr = list->head;
    SNode* next = NULL;

    while (curr != NULL) {
        next = curr->next; // 1. Preserve forward link
        curr->next = prev; // 2. Reverse pointer backwards
        prev = curr;       // 3. Advance prev
        curr = next;       // 4. Advance curr
    }

    list->head = prev;
    return STATUS_OK;
}
```

### 3. Binary Search Tree 3-Case Node Deletion
Deleting a node from a BST requires handling three distinct pointer topological cases:
1. **Leaf Node (0 children)**: Free the node and set parent's pointer to `NULL`.
2. **One Child**: Free the node and rewire parent's pointer directly to the single child.
3. **Two Children**: Find the **in-order successor** (smallest node in right subtree), copy its value to the current node, and recursively delete the successor node from the right subtree.

```c
static TreeNode* delete_node(TreeNode* root, int value, StatusCode* status) {
    if (root == NULL) {
        *status = STATUS_ERR_NOT_FOUND;
        return NULL;
    }
    if (value < root->data) {
        root->left = delete_node(root->left, value, status);
    } else if (value > root->data) {
        root->right = delete_node(root->right, value, status);
    } else {
        *status = STATUS_OK;
        if (root->left == NULL && root->right == NULL) { // Case 1: Leaf
            free(root);
            return NULL;
        } else if (root->left == NULL) {                 // Case 2: 1 Child (right)
            TreeNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {                // Case 2: 1 Child (left)
            TreeNode* temp = root->left;
            free(root);
            return temp;
        }
        // Case 3: 2 Children
        TreeNode* successor = find_min_node(root->right);
        root->data = successor->data;
        root->right = delete_node(root->right, successor->data, status);
    }
    return root;
}
```

---

## 🎯 Interview Talking Points & Deep Dives

When discussing this project in technical interviews, you can highlight these specific architectural decisions:

1. **How do you ensure zero memory leaks?**
   - Every `malloc` has a corresponding `free` pathway.
   - Trees are destroyed via recursive post-order traversal to ensure child nodes are freed before the parent node is deallocated.
   - Automated testing runs under GCC's **AddressSanitizer** (`-fsanitize=address,undefined`), catching any memory leak, buffer overflow, or use-after-free at runtime.

2. **Why use linked-list based stacks and queues over fixed arrays?**
   - Dynamic linked implementations provide true $O(1)$ operations with zero need for arbitrary capacity limits or expensive memory reallocations/copying (`realloc`).

3. **How does Floyd's Tortoise and Hare algorithm detect cycles in $O(1)$ space?**
   - Two pointers traverse the list: `slow` advances 1 step at a time, `fast` advances 2 steps. If a cycle exists, the distance between them decreases by 1 on each step until `slow == fast`, proving a cycle without requiring a hash set.

4. **Why is the status code enum pattern superior to returning sentinel values (like -1)?**
   - Returning `StatusCode` cleanly separates the operation's outcome from user data (which might legitimately be negative, e.g. storing `-1` in the stack). Data is passed back safely through output pointer parameters (`int* out_value`).

---

## 📜 License
MIT License. Free for educational and portfolio usage.
