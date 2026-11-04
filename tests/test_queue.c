#include "test_harness.h"
#include "queue.h"

void test_queue_core_operations(void) {
    Queue* q = queue_create();
    TEST_ASSERT(q != NULL, "queue_create allocates queue");
    TEST_ASSERT(queue_is_empty(q) == true, "new queue is empty");
    TEST_ASSERT(queue_size(q) == 0, "new queue size is 0");

    int val = 0;
    TEST_ASSERT(queue_dequeue(q, &val) == STATUS_ERR_EMPTY, "dequeue on empty queue returns STATUS_ERR_EMPTY");
    TEST_ASSERT(queue_peek(q, &val) == STATUS_ERR_EMPTY, "peek on empty queue returns STATUS_ERR_EMPTY");

    // Enqueue items
    TEST_ASSERT(queue_enqueue(q, 10) == STATUS_OK, "enqueue 10");
    TEST_ASSERT(queue_enqueue(q, 20) == STATUS_OK, "enqueue 20");
    TEST_ASSERT(queue_enqueue(q, 30) == STATUS_OK, "enqueue 30");

    TEST_ASSERT(queue_size(q) == 3, "queue size is 3");
    TEST_ASSERT(queue_peek(q, &val) == STATUS_OK && val == 10, "peek yields front element 10");

    // Dequeue items (FIFO order)
    TEST_ASSERT(queue_dequeue(q, &val) == STATUS_OK && val == 10, "dequeue 1 yields 10");
    TEST_ASSERT(queue_dequeue(q, &val) == STATUS_OK && val == 20, "dequeue 2 yields 20");
    TEST_ASSERT(queue_dequeue(q, &val) == STATUS_OK && val == 30, "dequeue 3 yields 30");
    TEST_ASSERT(queue_is_empty(q) == true, "queue is empty after dequeuing all");

    // Interleaved operations
    queue_enqueue(q, 100);
    queue_enqueue(q, 200);
    queue_dequeue(q, &val);
    TEST_ASSERT(val == 100, "interleaved dequeue yields 100");
    queue_enqueue(q, 300);
    TEST_ASSERT(queue_size(q) == 2, "interleaved queue size is 2");
    queue_dequeue(q, &val);
    TEST_ASSERT(val == 200, "interleaved dequeue yields 200");
    queue_dequeue(q, &val);
    TEST_ASSERT(val == 300, "interleaved dequeue yields 300");

    // Clear
    queue_enqueue(q, 999);
    queue_clear(q);
    TEST_ASSERT(queue_is_empty(q) == true, "queue is empty after clear");

    queue_destroy(&q);
    TEST_ASSERT(q == NULL, "queue pointer is NULL after destroy");
}

void run_queue_tests(void) {
    test_queue_core_operations();
}
