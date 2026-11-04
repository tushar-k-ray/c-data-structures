#ifndef DSA_LINKED_LIST_H
#define DSA_LINKED_LIST_H

#include "common.h"

/* ========================================================================= */
/*                      SINGLY LINKED LIST                                  */
/* ========================================================================= */

typedef struct SNode {
    int data;
    struct SNode* next;
} SNode;

typedef struct SList {
    SNode* head;
    size_t size;
} SList;

/**
 * Creates and initializes a new empty singly linked list on the heap.
 * Returns pointer to allocated SList, or NULL on allocation failure.
 */
SList* slist_create(void);

/**
 * Frees all nodes in the list and the list container itself.
 * Sets *list_ptr to NULL to prevent dangling pointers.
 */
void slist_destroy(SList** list_ptr);

/**
 * Removes all nodes from the list without freeing the list container.
 */
void slist_clear(SList* list);

/**
 * Prepends a new node with value to the beginning of the list. O(1)
 */
StatusCode slist_push_front(SList* list, int value);

/**
 * Appends a new node with value to the end of the list. O(n)
 */
StatusCode slist_push_back(SList* list, int value);

/**
 * Inserts a new node at the specified 0-based index. O(n)
 * index == 0 is front; index == size is back.
 */
StatusCode slist_insert_at(SList* list, size_t index, int value);

/**
 * Inserts value in ascending sorted order assuming list is sorted. O(n)
 */
StatusCode slist_insert_sorted(SList* list, int value);

/**
 * Removes the first node and returns its value through out_value. O(1)
 */
StatusCode slist_pop_front(SList* list, int* out_value);

/**
 * Removes the last node and returns its value through out_value. O(n)
 */
StatusCode slist_pop_back(SList* list, int* out_value);

/**
 * Deletes the node at index, returning its value in out_value. O(n)
 */
StatusCode slist_delete_at(SList* list, size_t index, int* out_value);

/**
 * Deletes the first occurrence of value in the list. O(n)
 */
StatusCode slist_delete_value(SList* list, int value);

/**
 * Finds the first occurrence of value and stores its 0-based index. O(n)
 */
StatusCode slist_find(const SList* list, int value, size_t* out_index);

/**
 * Retrieves the value at the specified 0-based index without removal. O(n)
 */
StatusCode slist_get(const SList* list, size_t index, int* out_value);

/**
 * Returns the number of nodes in the list. O(1)
 */
size_t slist_size(const SList* list);

/**
 * Returns true if list is empty or NULL.
 */
bool slist_is_empty(const SList* list);

/**
 * Reverses the singly linked list in-place using pointer rewiring. O(n) time, O(1) space.
 */
StatusCode slist_reverse(SList* list);

/**
 * Detects if the list contains a cycle using Floyd's Tortoise and Hare algorithm.
 */
bool slist_has_cycle(const SList* list);

/**
 * Merges two sorted lists into a newly allocated sorted list.
 * Original lists are not destroyed. O(n + m) time, O(n + m) space.
 */
StatusCode slist_merge_sorted(const SList* list1, const SList* list2, SList** out_merged);

/**
 * Prints the list elements in format: [head] -> 1 -> 2 -> 3 -> NULL
 */
void slist_print(const SList* list);


/* ========================================================================= */
/*                      DOUBLY LINKED LIST                                  */
/* ========================================================================= */

typedef struct DNode {
    int data;
    struct DNode* prev;
    struct DNode* next;
} DNode;

typedef struct DList {
    DNode* head;
    DNode* tail;
    size_t size;
} DList;

/**
 * Creates and initializes an empty doubly linked list.
 */
DList* dlist_create(void);

/**
 * Frees all nodes and the list container itself. Sets *list_ptr to NULL.
 */
void dlist_destroy(DList** list_ptr);

/**
 * Prepends a new node to the front of the doubly linked list. O(1)
 */
StatusCode dlist_push_front(DList* list, int value);

/**
 * Appends a new node to the back of the doubly linked list. O(1)
 */
StatusCode dlist_push_back(DList* list, int value);

/**
 * Removes front node, returning value in out_value. O(1)
 */
StatusCode dlist_pop_front(DList* list, int* out_value);

/**
 * Removes back node, returning value in out_value. O(1)
 */
StatusCode dlist_pop_back(DList* list, int* out_value);

/**
 * Deletes first node with specified value from the doubly linked list. O(n)
 */
StatusCode dlist_delete_value(DList* list, int value);

/**
 * Returns the size of the doubly linked list.
 */
size_t dlist_size(const DList* list);

/**
 * Returns true if empty or NULL.
 */
bool dlist_is_empty(const DList* list);

/**
 * Prints doubly linked list from head to tail.
 */
void dlist_print_forward(const DList* list);

/**
 * Prints doubly linked list from tail to head.
 */
void dlist_print_backward(const DList* list);

#endif /* DSA_LINKED_LIST_H */
