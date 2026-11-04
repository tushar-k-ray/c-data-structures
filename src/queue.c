#include "queue.h"

Queue* queue_create(void) {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    if (!q) return NULL;
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    return q;
}

void queue_clear(Queue* queue) {
    if (!queue) return;

    QueueNode* curr = queue->front;
    while (curr != NULL) {
        QueueNode* temp = curr;
        curr = curr->next;
        free(temp);
    }

    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
}

void queue_destroy(Queue** queue_ptr) {
    if (!queue_ptr || !*queue_ptr) return;
    queue_clear(*queue_ptr);
    free(*queue_ptr);
    *queue_ptr = NULL;
}

StatusCode queue_enqueue(Queue* queue, int value) {
    if (!queue) return STATUS_ERR_NULL_PTR;

    QueueNode* new_node = (QueueNode*)malloc(sizeof(QueueNode));
    if (!new_node) return STATUS_ERR_ALLOC;

    new_node->data = value;
    new_node->next = NULL;

    if (queue->rear == NULL) {
        queue->front = new_node;
        queue->rear = new_node;
    } else {
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
    queue->size++;

    return STATUS_OK;
}

StatusCode queue_dequeue(Queue* queue, int* out_value) {
    if (!queue) return STATUS_ERR_NULL_PTR;
    if (queue->front == NULL) return STATUS_ERR_EMPTY;

    QueueNode* temp = queue->front;
    if (out_value) {
        *out_value = temp->data;
    }

    queue->front = temp->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(temp);
    queue->size--;

    return STATUS_OK;
}

StatusCode queue_peek(const Queue* queue, int* out_value) {
    if (!queue || !out_value) return STATUS_ERR_NULL_PTR;
    if (queue->front == NULL) return STATUS_ERR_EMPTY;

    *out_value = queue->front->data;
    return STATUS_OK;
}

bool queue_is_empty(const Queue* queue) {
    return !queue || queue->size == 0;
}

size_t queue_size(const Queue* queue) {
    return queue ? queue->size : 0;
}

void queue_print(const Queue* queue) {
    if (!queue) {
        printf("(null queue)\n");
        return;
    }
    if (queue->front == NULL) {
        printf("[Empty Queue]\n");
        return;
    }

    printf("[Front] ");
    const QueueNode* curr = queue->front;
    while (curr != NULL) {
        printf("(%d)", curr->data);
        if (curr->next != NULL) {
            printf(" -> ");
        }
        curr = curr->next;
    }
    printf(" [Rear] (size: %zu)\n", queue->size);
}
