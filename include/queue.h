#ifndef DSA_QUEUE_H
#define DSA_QUEUE_H

#include "common.h"

typedef struct QueueNode {
    int data;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
    size_t size;
} Queue;

/**
 * Creates and initializes an empty dynamic pointer-based Queue (FIFO).
 */
Queue* queue_create(void);

/**
 * Destroys all nodes in the queue and frees the container. Sets *queue_ptr to NULL.
 */
void queue_destroy(Queue** queue_ptr);

/**
 * Removes all elements from the queue.
 */
void queue_clear(Queue* queue);

/**
 * Enqueues a value at the rear of the queue. O(1)
 */
StatusCode queue_enqueue(Queue* queue, int value);

/**
 * Dequeues a value from the front of the queue, storing it in out_value. O(1)
 */
StatusCode queue_dequeue(Queue* queue, int* out_value);

/**
 * Peeks at the front element without removing it. O(1)
 */
StatusCode queue_peek(const Queue* queue, int* out_value);

/**
 * Returns true if queue is empty or NULL.
 */
bool queue_is_empty(const Queue* queue);

/**
 * Returns the current number of elements in the queue.
 */
size_t queue_size(const Queue* queue);

/**
 * Prints the queue elements from front to rear: [Front] 10 -> 20 -> 30 [Rear]
 */
void queue_print(const Queue* queue);

#endif /* DSA_QUEUE_H */
