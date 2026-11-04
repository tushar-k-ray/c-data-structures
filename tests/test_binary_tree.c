#include "test_harness.h"
#include "binary_tree.h"

static int g_traversal_buf[64];
static size_t g_traversal_idx = 0;

static void collect_traversal(int val) {
    if (g_traversal_idx < 64) {
        g_traversal_buf[g_traversal_idx++] = val;
    }
}

void test_bst_insertion_and_search(void) {
    BSTree* tree = bst_create();
    TEST_ASSERT(tree != NULL, "bst_create allocates BST");
    TEST_ASSERT(bst_is_empty(tree) == true, "new BST is empty");
    TEST_ASSERT(bst_size(tree) == 0, "new BST size is 0");

    // Insert 50, 30, 70, 20, 40, 60, 80
    TEST_ASSERT(bst_insert(tree, 50) == STATUS_OK, "insert 50");
    TEST_ASSERT(bst_insert(tree, 30) == STATUS_OK, "insert 30");
    TEST_ASSERT(bst_insert(tree, 70) == STATUS_OK, "insert 70");
    TEST_ASSERT(bst_insert(tree, 20) == STATUS_OK, "insert 20");
    TEST_ASSERT(bst_insert(tree, 40) == STATUS_OK, "insert 40");
    TEST_ASSERT(bst_insert(tree, 60) == STATUS_OK, "insert 60");
    TEST_ASSERT(bst_insert(tree, 80) == STATUS_OK, "insert 80");

    TEST_ASSERT(bst_size(tree) == 7, "tree size is 7");
    TEST_ASSERT(bst_insert(tree, 50) == STATUS_ERR_DUPLICATE, "duplicate insert returns duplicate error");

    // Search
    TEST_ASSERT(bst_search(tree, 50) == true, "search root 50");
    TEST_ASSERT(bst_search(tree, 20) == true, "search leaf 20");
    TEST_ASSERT(bst_search(tree, 80) == true, "search leaf 80");
    TEST_ASSERT(bst_search(tree, 999) == false, "search non-existent 999 returns false");

    // Valid BST check
    TEST_ASSERT(bst_is_valid(tree) == true, "tree satisfies BST invariants");

    // Min and Max
    int min_val = 0, max_val = 0;
    TEST_ASSERT(bst_min(tree, &min_val) == STATUS_OK && min_val == 20, "min is 20");
    TEST_ASSERT(bst_max(tree, &max_val) == STATUS_OK && max_val == 80, "max is 80");

    // Height: root (50) -> (30) -> (20) has 2 edges, height 2
    TEST_ASSERT(bst_height(tree) == 2, "tree height is 2");

    bst_destroy(&tree);
    TEST_ASSERT(tree == NULL, "tree pointer is NULL after destroy");
}

void test_bst_traversals(void) {
    BSTree* tree = bst_create();
    // Insert: 50, 30, 70, 20, 40, 60, 80
    bst_insert(tree, 50);
    bst_insert(tree, 30);
    bst_insert(tree, 70);
    bst_insert(tree, 20);
    bst_insert(tree, 40);
    bst_insert(tree, 60);
    bst_insert(tree, 80);

    // In-order (should be sorted: 20, 30, 40, 50, 60, 70, 80)
    g_traversal_idx = 0;
    bst_inorder(tree, collect_traversal);
    int exp_inorder[] = {20, 30, 40, 50, 60, 70, 80};
    bool inorder_ok = (g_traversal_idx == 7);
    for (size_t i = 0; i < 7 && inorder_ok; i++) {
        if (g_traversal_buf[i] != exp_inorder[i]) inorder_ok = false;
    }
    TEST_ASSERT(inorder_ok, "in-order traversal visits in ascending order");

    // Pre-order (Root, Left, Right): 50, 30, 20, 40, 70, 60, 80
    g_traversal_idx = 0;
    bst_preorder(tree, collect_traversal);
    int exp_preorder[] = {50, 30, 20, 40, 70, 60, 80};
    bool preorder_ok = (g_traversal_idx == 7);
    for (size_t i = 0; i < 7 && preorder_ok; i++) {
        if (g_traversal_buf[i] != exp_preorder[i]) preorder_ok = false;
    }
    TEST_ASSERT(preorder_ok, "pre-order traversal matches expected sequence");

    // Level-order (BFS): 50, 30, 70, 20, 40, 60, 80
    g_traversal_idx = 0;
    bst_levelorder(tree, collect_traversal);
    int exp_levelorder[] = {50, 30, 70, 20, 40, 60, 80};
    bool levelorder_ok = (g_traversal_idx == 7);
    for (size_t i = 0; i < 7 && levelorder_ok; i++) {
        if (g_traversal_buf[i] != exp_levelorder[i]) levelorder_ok = false;
    }
    TEST_ASSERT(levelorder_ok, "level-order traversal matches BFS sequence");

    bst_destroy(&tree);
}

void test_bst_deletions(void) {
    BSTree* tree = bst_create();
    /* Build tree:
              50
            /    \
           30     70
          /  \   /  \
         20  40 60  80
    */
    bst_insert(tree, 50);
    bst_insert(tree, 30);
    bst_insert(tree, 70);
    bst_insert(tree, 20);
    bst_insert(tree, 40);
    bst_insert(tree, 60);
    bst_insert(tree, 80);

    // Case 1: Delete leaf node (20)
    TEST_ASSERT(bst_delete(tree, 20) == STATUS_OK, "delete leaf 20");
    TEST_ASSERT(bst_search(tree, 20) == false, "20 no longer found");
    TEST_ASSERT(bst_size(tree) == 6, "size is 6 after leaf deletion");
    TEST_ASSERT(bst_is_valid(tree) == true, "tree remains valid BST after leaf deletion");

    // Case 2: Delete node with 1 child
    // Right now 30 only has right child 40 (since 20 was deleted)
    TEST_ASSERT(bst_delete(tree, 30) == STATUS_OK, "delete node with 1 child (30)");
    TEST_ASSERT(bst_search(tree, 30) == false, "30 no longer found");
    TEST_ASSERT(bst_search(tree, 40) == true, "40 still found after parent deleted");
    TEST_ASSERT(bst_size(tree) == 5, "size is 5");
    TEST_ASSERT(bst_is_valid(tree) == true, "tree remains valid BST after 1-child deletion");

    // Case 3: Delete node with 2 children (Root node 50)
    // 50 has left child 40, right child 70 (which has children 60, 80)
    // In-order successor of 50 is 60.
    TEST_ASSERT(bst_delete(tree, 50) == STATUS_OK, "delete root node with 2 children (50)");
    TEST_ASSERT(bst_search(tree, 50) == false, "root 50 no longer found");
    TEST_ASSERT(bst_size(tree) == 4, "size is 4");
    TEST_ASSERT(bst_is_valid(tree) == true, "tree remains valid BST after 2-child deletion");

    // Verify remaining nodes via in-order: 40, 60, 70, 80
    g_traversal_idx = 0;
    bst_inorder(tree, collect_traversal);
    int exp_after_del[] = {40, 60, 70, 80};
    bool rem_ok = (g_traversal_idx == 4);
    for (size_t i = 0; i < 4 && rem_ok; i++) {
        if (g_traversal_buf[i] != exp_after_del[i]) rem_ok = false;
    }
    TEST_ASSERT(rem_ok, "in-order after deletions is sorted [40, 60, 70, 80]");

    // Try deleting non-existent node
    TEST_ASSERT(bst_delete(tree, 999) == STATUS_ERR_NOT_FOUND, "delete non-existent returns NOT_FOUND");

    bst_destroy(&tree);
}

void test_bst_lowest_common_ancestor(void) {
    BSTree* tree = bst_create();
    bst_insert(tree, 20);
    bst_insert(tree, 8);
    bst_insert(tree, 22);
    bst_insert(tree, 4);
    bst_insert(tree, 12);
    bst_insert(tree, 10);
    bst_insert(tree, 14);

    int lca = 0;
    TEST_ASSERT(bst_lowest_common_ancestor(tree, 4, 14, &lca) == STATUS_OK && lca == 8, "LCA of 4 and 14 is 8");
    TEST_ASSERT(bst_lowest_common_ancestor(tree, 14, 8, &lca) == STATUS_OK && lca == 8, "LCA of 14 and 8 is 8");
    TEST_ASSERT(bst_lowest_common_ancestor(tree, 10, 22, &lca) == STATUS_OK && lca == 20, "LCA of 10 and 22 is 20");
    TEST_ASSERT(bst_lowest_common_ancestor(tree, 4, 999, &lca) == STATUS_ERR_NOT_FOUND, "LCA with non-existent node returns error");

    bst_destroy(&tree);
}

void run_binary_tree_tests(void) {
    test_bst_insertion_and_search();
    test_bst_traversals();
    test_bst_deletions();
    test_bst_lowest_common_ancestor();
}
