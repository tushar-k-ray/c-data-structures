#include "linked_list.h"

/* ========================================================================= */
/*                      SINGLY LINKED LIST                                  */
/* ========================================================================= */

SList* slist_create(void) {
    SList* list = (SList*)malloc(sizeof(SList));
    if (!list) {
        return NULL;
    }
    list->head = NULL;
    list->size = 0;
    return list;
}

static void slist_free_nodes(SNode* head) {
    SNode* curr = head;
    while (curr != NULL) {
        SNode* temp = curr;
        curr = curr->next;
        free(temp);
    }
}

void slist_clear(SList* list) {
    if (!list) return;
    slist_free_nodes(list->head);
    list->head = NULL;
    list->size = 0;
}

void slist_destroy(SList** list_ptr) {
    if (!list_ptr || !*list_ptr) return;
    slist_clear(*list_ptr);
    free(*list_ptr);
    *list_ptr = NULL;
}

StatusCode slist_push_front(SList* list, int value) {
    if (!list) return STATUS_ERR_NULL_PTR;

    SNode* new_node = (SNode*)malloc(sizeof(SNode));
    if (!new_node) return STATUS_ERR_ALLOC;

    new_node->data = value;
    new_node->next = list->head;
    list->head = new_node;
    list->size++;

    return STATUS_OK;
}

StatusCode slist_push_back(SList* list, int value) {
    if (!list) return STATUS_ERR_NULL_PTR;

    SNode* new_node = (SNode*)malloc(sizeof(SNode));
    if (!new_node) return STATUS_ERR_ALLOC;

    new_node->data = value;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
    } else {
        SNode* curr = list->head;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = new_node;
    }
    list->size++;

    return STATUS_OK;
}

StatusCode slist_insert_at(SList* list, size_t index, int value) {
    if (!list) return STATUS_ERR_NULL_PTR;
    if (index > list->size) return STATUS_ERR_OUT_OF_BOUNDS;

    if (index == 0) {
        return slist_push_front(list, value);
    }
    if (index == list->size) {
        return slist_push_back(list, value);
    }

    SNode* new_node = (SNode*)malloc(sizeof(SNode));
    if (!new_node) return STATUS_ERR_ALLOC;

    new_node->data = value;

    SNode* curr = list->head;
    for (size_t i = 0; i < index - 1; i++) {
        curr = curr->next;
    }

    new_node->next = curr->next;
    curr->next = new_node;
    list->size++;

    return STATUS_OK;
}

StatusCode slist_insert_sorted(SList* list, int value) {
    if (!list) return STATUS_ERR_NULL_PTR;

    if (list->head == NULL || list->head->data >= value) {
        return slist_push_front(list, value);
    }

    SNode* new_node = (SNode*)malloc(sizeof(SNode));
    if (!new_node) return STATUS_ERR_ALLOC;
    new_node->data = value;

    SNode* curr = list->head;
    while (curr->next != NULL && curr->next->data < value) {
        curr = curr->next;
    }

    new_node->next = curr->next;
    curr->next = new_node;
    list->size++;

    return STATUS_OK;
}

StatusCode slist_pop_front(SList* list, int* out_value) {
    if (!list) return STATUS_ERR_NULL_PTR;
    if (list->head == NULL) return STATUS_ERR_EMPTY;

    SNode* temp = list->head;
    if (out_value) {
        *out_value = temp->data;
    }
    list->head = temp->next;
    free(temp);
    list->size--;

    return STATUS_OK;
}

StatusCode slist_pop_back(SList* list, int* out_value) {
    if (!list) return STATUS_ERR_NULL_PTR;
    if (list->head == NULL) return STATUS_ERR_EMPTY;

    if (list->head->next == NULL) {
        if (out_value) {
            *out_value = list->head->data;
        }
        free(list->head);
        list->head = NULL;
        list->size = 0;
        return STATUS_OK;
    }

    SNode* curr = list->head;
    while (curr->next->next != NULL) {
        curr = curr->next;
    }

    if (out_value) {
        *out_value = curr->next->data;
    }
    free(curr->next);
    curr->next = NULL;
    list->size--;

    return STATUS_OK;
}

StatusCode slist_delete_at(SList* list, size_t index, int* out_value) {
    if (!list) return STATUS_ERR_NULL_PTR;
    if (index >= list->size) return STATUS_ERR_OUT_OF_BOUNDS;

    if (index == 0) {
        return slist_pop_front(list, out_value);
    }

    SNode* curr = list->head;
    for (size_t i = 0; i < index - 1; i++) {
        curr = curr->next;
    }

    SNode* to_delete = curr->next;
    if (out_value) {
        *out_value = to_delete->data;
    }
    curr->next = to_delete->next;
    free(to_delete);
    list->size--;

    return STATUS_OK;
}

StatusCode slist_delete_value(SList* list, int value) {
    if (!list) return STATUS_ERR_NULL_PTR;
    if (list->head == NULL) return STATUS_ERR_NOT_FOUND;

    if (list->head->data == value) {
        return slist_pop_front(list, NULL);
    }

    SNode* curr = list->head;
    while (curr->next != NULL && curr->next->data != value) {
        curr = curr->next;
    }

    if (curr->next == NULL) {
        return STATUS_ERR_NOT_FOUND;
    }

    SNode* to_delete = curr->next;
    curr->next = to_delete->next;
    free(to_delete);
    list->size--;

    return STATUS_OK;
}

StatusCode slist_find(const SList* list, int value, size_t* out_index) {
    if (!list) return STATUS_ERR_NULL_PTR;

    SNode* curr = list->head;
    size_t idx = 0;
    while (curr != NULL) {
        if (curr->data == value) {
            if (out_index) *out_index = idx;
            return STATUS_OK;
        }
        curr = curr->next;
        idx++;
    }

    return STATUS_ERR_NOT_FOUND;
}

StatusCode slist_get(const SList* list, size_t index, int* out_value) {
    if (!list || !out_value) return STATUS_ERR_NULL_PTR;
    if (index >= list->size) return STATUS_ERR_OUT_OF_BOUNDS;

    SNode* curr = list->head;
    for (size_t i = 0; i < index; i++) {
        curr = curr->next;
    }

    *out_value = curr->data;
    return STATUS_OK;
}

size_t slist_size(const SList* list) {
    return list ? list->size : 0;
}

bool slist_is_empty(const SList* list) {
    return !list || list->size == 0;
}

StatusCode slist_reverse(SList* list) {
    if (!list) return STATUS_ERR_NULL_PTR;
    if (list->size <= 1) return STATUS_OK;

    SNode* prev = NULL;
    SNode* curr = list->head;
    SNode* next = NULL;

    while (curr != NULL) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }

    list->head = prev;
    return STATUS_OK;
}

bool slist_has_cycle(const SList* list) {
    if (!list || list->head == NULL || list->head->next == NULL) {
        return false;
    }

    SNode* slow = list->head;
    SNode* fast = list->head;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            return true;
        }
    }

    return false;
}

StatusCode slist_merge_sorted(const SList* list1, const SList* list2, SList** out_merged) {
    if (!list1 || !list2 || !out_merged) return STATUS_ERR_NULL_PTR;

    SList* merged = slist_create();
    if (!merged) return STATUS_ERR_ALLOC;

    const SNode* p1 = list1->head;
    const SNode* p2 = list2->head;

    while (p1 != NULL && p2 != NULL) {
        if (p1->data <= p2->data) {
            slist_push_back(merged, p1->data);
            p1 = p1->next;
        } else {
            slist_push_back(merged, p2->data);
            p2 = p2->next;
        }
    }

    while (p1 != NULL) {
        slist_push_back(merged, p1->data);
        p1 = p1->next;
    }

    while (p2 != NULL) {
        slist_push_back(merged, p2->data);
        p2 = p2->next;
    }

    *out_merged = merged;
    return STATUS_OK;
}

void slist_print(const SList* list) {
    if (!list) {
        printf("(null list)\n");
        return;
    }
    if (list->head == NULL) {
        printf("[Empty List]\n");
        return;
    }

    const SNode* curr = list->head;
    printf("[Head] ");
    while (curr != NULL) {
        printf("(%d) -> ", curr->data);
        curr = curr->next;
    }
    printf("NULL (size: %zu)\n", list->size);
}

/* ========================================================================= */
/*                      DOUBLY LINKED LIST                                  */
/* ========================================================================= */

DList* dlist_create(void) {
    DList* list = (DList*)malloc(sizeof(DList));
    if (!list) return NULL;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void dlist_destroy(DList** list_ptr) {
    if (!list_ptr || !*list_ptr) return;
    DList* list = *list_ptr;

    DNode* curr = list->head;
    while (curr != NULL) {
        DNode* temp = curr;
        curr = curr->next;
        free(temp);
    }

    free(list);
    *list_ptr = NULL;
}

StatusCode dlist_push_front(DList* list, int value) {
    if (!list) return STATUS_ERR_NULL_PTR;

    DNode* new_node = (DNode*)malloc(sizeof(DNode));
    if (!new_node) return STATUS_ERR_ALLOC;

    new_node->data = value;
    new_node->prev = NULL;
    new_node->next = list->head;

    if (list->head != NULL) {
        list->head->prev = new_node;
    } else {
        list->tail = new_node;
    }
    list->head = new_node;
    list->size++;

    return STATUS_OK;
}

StatusCode dlist_push_back(DList* list, int value) {
    if (!list) return STATUS_ERR_NULL_PTR;

    DNode* new_node = (DNode*)malloc(sizeof(DNode));
    if (!new_node) return STATUS_ERR_ALLOC;

    new_node->data = value;
    new_node->next = NULL;
    new_node->prev = list->tail;

    if (list->tail != NULL) {
        list->tail->next = new_node;
    } else {
        list->head = new_node;
    }
    list->tail = new_node;
    list->size++;

    return STATUS_OK;
}

StatusCode dlist_pop_front(DList* list, int* out_value) {
    if (!list) return STATUS_ERR_NULL_PTR;
    if (list->head == NULL) return STATUS_ERR_EMPTY;

    DNode* temp = list->head;
    if (out_value) *out_value = temp->data;

    list->head = temp->next;
    if (list->head != NULL) {
        list->head->prev = NULL;
    } else {
        list->tail = NULL;
    }

    free(temp);
    list->size--;
    return STATUS_OK;
}

StatusCode dlist_pop_back(DList* list, int* out_value) {
    if (!list) return STATUS_ERR_NULL_PTR;
    if (list->tail == NULL) return STATUS_ERR_EMPTY;

    DNode* temp = list->tail;
    if (out_value) *out_value = temp->data;

    list->tail = temp->prev;
    if (list->tail != NULL) {
        list->tail->next = NULL;
    } else {
        list->head = NULL;
    }

    free(temp);
    list->size--;
    return STATUS_OK;
}

StatusCode dlist_delete_value(DList* list, int value) {
    if (!list) return STATUS_ERR_NULL_PTR;
    if (list->head == NULL) return STATUS_ERR_NOT_FOUND;

    DNode* curr = list->head;
    while (curr != NULL && curr->data != value) {
        curr = curr->next;
    }

    if (curr == NULL) {
        return STATUS_ERR_NOT_FOUND;
    }

    if (curr == list->head) {
        return dlist_pop_front(list, NULL);
    }
    if (curr == list->tail) {
        return dlist_pop_back(list, NULL);
    }

    curr->prev->next = curr->next;
    curr->next->prev = curr->prev;
    free(curr);
    list->size--;

    return STATUS_OK;
}

size_t dlist_size(const DList* list) {
    return list ? list->size : 0;
}

bool dlist_is_empty(const DList* list) {
    return !list || list->size == 0;
}

void dlist_print_forward(const DList* list) {
    if (!list) {
        printf("(null dlist)\n");
        return;
    }
    if (list->head == NULL) {
        printf("[Empty DList]\n");
        return;
    }

    const DNode* curr = list->head;
    printf("[Head] NULL <-> ");
    while (curr != NULL) {
        printf("(%d) <-> ", curr->data);
        curr = curr->next;
    }
    printf("NULL [Tail] (size: %zu)\n", list->size);
}

void dlist_print_backward(const DList* list) {
    if (!list) {
        printf("(null dlist)\n");
        return;
    }
    if (list->tail == NULL) {
        printf("[Empty DList]\n");
        return;
    }

    const DNode* curr = list->tail;
    printf("[Tail] NULL <-> ");
    while (curr != NULL) {
        printf("(%d) <-> ", curr->data);
        curr = curr->prev;
    }
    printf("NULL [Head] (size: %zu)\n", list->size);
}
