#ifndef DSA_BINARY_TREE_H
#define DSA_BINARY_TREE_H

#include "common.h"

typedef struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

typedef struct BSTree {
    TreeNode* root;
    size_t size;
} BSTree;

typedef void (*TreeVisitFunc)(int value);

/**
 * Creates and initializes a new empty Binary Search Tree.
 */
BSTree* bst_create(void);

/**
 * Frees all nodes in the tree using post-order deallocation and frees the tree container.
 * Sets *tree_ptr to NULL.
 */
void bst_destroy(BSTree** tree_ptr);

/**
 * Removes all nodes from the tree without freeing the container.
 */
void bst_clear(BSTree* tree);

/**
 * Inserts a value into the BST maintaining the binary search tree property.
 * Returns STATUS_ERR_DUPLICATE if value already exists.
 */
StatusCode bst_insert(BSTree* tree, int value);

/**
 * Searches for a value in the BST.
 * Returns true if found, false otherwise.
 */
bool bst_search(const BSTree* tree, int value);

/**
 * Deletes a node with the specified value from the BST.
 * Correctly handles 0 children (leaf), 1 child, and 2 children (in-order successor).
 */
StatusCode bst_delete(BSTree* tree, int value);

/**
 * In-Order Traversal (Left, Root, Right).
 * Visits nodes in ascending sorted order.
 */
void bst_inorder(const BSTree* tree, TreeVisitFunc visit);

/**
 * Pre-Order Traversal (Root, Left, Right).
 */
void bst_preorder(const BSTree* tree, TreeVisitFunc visit);

/**
 * Post-Order Traversal (Left, Right, Root).
 */
void bst_postorder(const BSTree* tree, TreeVisitFunc visit);

/**
 * Level-Order Traversal (Breadth-First Search).
 * Uses a FIFO queue to visit nodes level by level.
 */
void bst_levelorder(const BSTree* tree, TreeVisitFunc visit);

/**
 * Computes the height of the tree (empty tree = -1, single node = 0, etc.).
 */
int bst_height(const BSTree* tree);

/**
 * Finds the minimum value in the BST.
 */
StatusCode bst_min(const BSTree* tree, int* out_value);

/**
 * Finds the maximum value in the BST.
 */
StatusCode bst_max(const BSTree* tree, int* out_value);

/**
 * Returns the number of nodes in the BST.
 */
size_t bst_size(const BSTree* tree);

/**
 * Returns true if the BST has 0 nodes.
 */
bool bst_is_empty(const BSTree* tree);

/**
 * Validates whether the binary tree strictly satisfies the BST invariants.
 */
bool bst_is_valid(const BSTree* tree);

/**
 * Finds the Lowest Common Ancestor (LCA) of two values in the BST.
 */
StatusCode bst_lowest_common_ancestor(const BSTree* tree, int val1, int val2, int* out_lca);

/**
 * Pretty-prints the binary search tree in terminal with visual branches.
 */
void bst_print_ascii(const BSTree* tree);

#endif /* DSA_BINARY_TREE_H */
