#include "test_harness.h"
#include "linked_list.h"

void test_slist_basic_operations(void) {
    SList* list = slist_create();
    TEST_ASSERT(list != NULL, "slist_create should allocate list");
    TEST_ASSERT(slist_is_empty(list) == true, "new list should be empty");
    TEST_ASSERT(slist_size(list) == 0, "new list size should be 0");

    // Push front
    TEST_ASSERT(slist_push_front(list, 10) == STATUS_OK, "push_front 10 succeeds");
    TEST_ASSERT(slist_push_front(list, 20) == STATUS_OK, "push_front 20 succeeds");
    TEST_ASSERT(slist_size(list) == 2, "size should be 2");

    int val = 0;
    TEST_ASSERT(slist_get(list, 0, &val) == STATUS_OK && val == 20, "index 0 should be 20");
    TEST_ASSERT(slist_get(list, 1, &val) == STATUS_OK && val == 10, "index 1 should be 10");

    // Push back
    TEST_ASSERT(slist_push_back(list, 30) == STATUS_OK, "push_back 30 succeeds");
    TEST_ASSERT(slist_size(list) == 3, "size should be 3");
    TEST_ASSERT(slist_get(list, 2, &val) == STATUS_OK && val == 30, "index 2 should be 30");

    // Insert at
    TEST_ASSERT(slist_insert_at(list, 1, 15) == STATUS_OK, "insert 15 at index 1");
    TEST_ASSERT(slist_get(list, 1, &val) == STATUS_OK && val == 15, "index 1 should now be 15");
    TEST_ASSERT(slist_size(list) == 4, "size should be 4");

    // Find
    size_t found_idx = 0;
    TEST_ASSERT(slist_find(list, 15, &found_idx) == STATUS_OK && found_idx == 1, "find 15 at index 1");
    TEST_ASSERT(slist_find(list, 999, &found_idx) == STATUS_ERR_NOT_FOUND, "find non-existent returns NOT_FOUND");

    // Pop front
    TEST_ASSERT(slist_pop_front(list, &val) == STATUS_OK && val == 20, "pop_front yields 20");
    TEST_ASSERT(slist_size(list) == 3, "size after pop_front should be 3");

    // Pop back
    TEST_ASSERT(slist_pop_back(list, &val) == STATUS_OK && val == 30, "pop_back yields 30");
    TEST_ASSERT(slist_size(list) == 2, "size after pop_back should be 2");

    // Delete value
    TEST_ASSERT(slist_delete_value(list, 15) == STATUS_OK, "delete value 15");
    TEST_ASSERT(slist_size(list) == 1, "size after deleting 15 should be 1");
    TEST_ASSERT(slist_delete_value(list, 999) == STATUS_ERR_NOT_FOUND, "delete non-existent returns NOT_FOUND");

    // Delete at
    TEST_ASSERT(slist_delete_at(list, 0, &val) == STATUS_OK && val == 10, "delete at 0 yields 10");
    TEST_ASSERT(slist_is_empty(list) == true, "list should now be empty");

    slist_destroy(&list);
    TEST_ASSERT(list == NULL, "list pointer should be NULL after destroy");
}

void test_slist_sorted_and_reverse(void) {
    SList* list = slist_create();

    // Sorted insert: 50, 10, 30, 20, 40
    slist_insert_sorted(list, 50);
    slist_insert_sorted(list, 10);
    slist_insert_sorted(list, 30);
    slist_insert_sorted(list, 20);
    slist_insert_sorted(list, 40);

    TEST_ASSERT(slist_size(list) == 5, "sorted list should have 5 items");

    int expected[] = {10, 20, 30, 40, 50};
    bool sorted_ok = true;
    for (size_t i = 0; i < 5; i++) {
        int v = 0;
        slist_get(list, i, &v);
        if (v != expected[i]) sorted_ok = false;
    }
    TEST_ASSERT(sorted_ok, "list should be sorted ascending 10,20,30,40,50");

    // Reverse in place
    TEST_ASSERT(slist_reverse(list) == STATUS_OK, "reverse list succeeds");
    int rev_expected[] = {50, 40, 30, 20, 10};
    bool rev_ok = true;
    for (size_t i = 0; i < 5; i++) {
        int v = 0;
        slist_get(list, i, &v);
        if (v != rev_expected[i]) rev_ok = false;
    }
    TEST_ASSERT(rev_ok, "reversed list should be 50,40,30,20,10");

    slist_destroy(&list);
}

void test_slist_cycle_detection(void) {
    SList* list = slist_create();
    slist_push_back(list, 1);
    slist_push_back(list, 2);
    slist_push_back(list, 3);
    slist_push_back(list, 4);

    TEST_ASSERT(slist_has_cycle(list) == false, "normal list has no cycle");

    // Manually introduce cycle for testing Floyd's algorithm: node 4 points to node 2
    SNode* node2 = list->head->next;
    SNode* node4 = node2->next->next;
    node4->next = node2;

    TEST_ASSERT(slist_has_cycle(list) == true, "slist_has_cycle detects introduced cycle");

    // Break cycle before freeing memory
    node4->next = NULL;
    TEST_ASSERT(slist_has_cycle(list) == false, "cycle successfully removed");

    slist_destroy(&list);
}

void test_slist_merge_sorted(void) {
    SList* l1 = slist_create();
    SList* l2 = slist_create();

    slist_push_back(l1, 10);
    slist_push_back(l1, 30);
    slist_push_back(l1, 50);

    slist_push_back(l2, 20);
    slist_push_back(l2, 40);
    slist_push_back(l2, 60);

    SList* merged = NULL;
    TEST_ASSERT(slist_merge_sorted(l1, l2, &merged) == STATUS_OK, "merge two sorted lists");
    TEST_ASSERT(slist_size(merged) == 6, "merged list has 6 elements");

    int expected[] = {10, 20, 30, 40, 50, 60};
    bool ok = true;
    for (size_t i = 0; i < 6; i++) {
        int v = 0;
        slist_get(merged, i, &v);
        if (v != expected[i]) ok = false;
    }
    TEST_ASSERT(ok, "merged list correctly sorted");

    slist_destroy(&l1);
    slist_destroy(&l2);
    slist_destroy(&merged);
}

void test_dlist_operations(void) {
    DList* dl = dlist_create();
    TEST_ASSERT(dl != NULL, "dlist_create allocates doubly linked list");
    TEST_ASSERT(dlist_is_empty(dl) == true, "new dlist is empty");

    dlist_push_front(dl, 20);
    dlist_push_front(dl, 10);
    dlist_push_back(dl, 30);
    dlist_push_back(dl, 40);

    TEST_ASSERT(dlist_size(dl) == 4, "dlist size is 4");

    int v = 0;
    TEST_ASSERT(dlist_pop_front(dl, &v) == STATUS_OK && v == 10, "pop_front yields 10");
    TEST_ASSERT(dlist_pop_back(dl, &v) == STATUS_OK && v == 40, "pop_back yields 40");
    TEST_ASSERT(dlist_size(dl) == 2, "dlist size is now 2");

    TEST_ASSERT(dlist_delete_value(dl, 30) == STATUS_OK, "delete value 30");
    TEST_ASSERT(dlist_size(dl) == 1, "dlist size is now 1");

    dlist_destroy(&dl);
    TEST_ASSERT(dl == NULL, "dlist pointer is NULL after destroy");
}

void run_linked_list_tests(void) {
    test_slist_basic_operations();
    test_slist_sorted_and_reverse();
    test_slist_cycle_detection();
    test_slist_merge_sorted();
    test_dlist_operations();
}
