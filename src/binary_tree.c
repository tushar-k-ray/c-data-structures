#include "binary_tree.h"
#include <limits.h>

BSTree* bst_create(void) {
    BSTree* tree = (BSTree*)malloc(sizeof(BSTree));
    if (!tree) return NULL;
    tree->root = NULL;
    tree->size = 0;
    return tree;
}

static void free_nodes_postorder(TreeNode* node) {
    if (node == NULL) return;
    free_nodes_postorder(node->left);
    free_nodes_postorder(node->right);
    free(node);
}

void bst_clear(BSTree* tree) {
    if (!tree) return;
    free_nodes_postorder(tree->root);
    tree->root = NULL;
    tree->size = 0;
}

void bst_destroy(BSTree** tree_ptr) {
    if (!tree_ptr || !*tree_ptr) return;
    bst_clear(*tree_ptr);
    free(*tree_ptr);
    *tree_ptr = NULL;
}

static TreeNode* create_tree_node(int value) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (!node) return NULL;
    node->data = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static TreeNode* insert_node(TreeNode* root, int value, StatusCode* status) {
    if (root == NULL) {
        TreeNode* new_node = create_tree_node(value);
        if (!new_node) {
            *status = STATUS_ERR_ALLOC;
            return NULL;
        }
        *status = STATUS_OK;
        return new_node;
    }

    if (value < root->data) {
        root->left = insert_node(root->left, value, status);
    } else if (value > root->data) {
        root->right = insert_node(root->right, value, status);
    } else {
        *status = STATUS_ERR_DUPLICATE;
    }

    return root;
}

StatusCode bst_insert(BSTree* tree, int value) {
    if (!tree) return STATUS_ERR_NULL_PTR;

    StatusCode status = STATUS_OK;
    tree->root = insert_node(tree->root, value, &status);
    if (status == STATUS_OK) {
        tree->size++;
    }
    return status;
}

static bool search_node(const TreeNode* root, int value) {
    if (root == NULL) return false;
    if (root->data == value) return true;
    if (value < root->data) {
        return search_node(root->left, value);
    } else {
        return search_node(root->right, value);
    }
}

bool bst_search(const BSTree* tree, int value) {
    if (!tree) return false;
    return search_node(tree->root, value);
}

static TreeNode* find_min_node(TreeNode* node) {
    TreeNode* curr = node;
    while (curr && curr->left != NULL) {
        curr = curr->left;
    }
    return curr;
}

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
        // Node found
        *status = STATUS_OK;

        // Case 1: Leaf node (no children)
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }
        // Case 2: One child (only right child)
        else if (root->left == NULL) {
            TreeNode* temp = root->right;
            free(root);
            return temp;
        }
        // Case 2: One child (only left child)
        else if (root->right == NULL) {
            TreeNode* temp = root->left;
            free(root);
            return temp;
        }
        // Case 3: Two children
        // Find in-order successor (smallest in right subtree)
        TreeNode* successor = find_min_node(root->right);
        root->data = successor->data;
        // Delete successor from right subtree
        root->right = delete_node(root->right, successor->data, status);
    }

    return root;
}

StatusCode bst_delete(BSTree* tree, int value) {
    if (!tree) return STATUS_ERR_NULL_PTR;
    if (tree->root == NULL) return STATUS_ERR_EMPTY;

    StatusCode status = STATUS_OK;
    tree->root = delete_node(tree->root, value, &status);
    if (status == STATUS_OK) {
        tree->size--;
    }
    return status;
}

static void inorder_helper(const TreeNode* node, TreeVisitFunc visit) {
    if (!node) return;
    inorder_helper(node->left, visit);
    visit(node->data);
    inorder_helper(node->right, visit);
}

void bst_inorder(const BSTree* tree, TreeVisitFunc visit) {
    if (!tree || !visit) return;
    inorder_helper(tree->root, visit);
}

static void preorder_helper(const TreeNode* node, TreeVisitFunc visit) {
    if (!node) return;
    visit(node->data);
    preorder_helper(node->left, visit);
    preorder_helper(node->right, visit);
}

void bst_preorder(const BSTree* tree, TreeVisitFunc visit) {
    if (!tree || !visit) return;
    preorder_helper(tree->root, visit);
}

static void postorder_helper(const TreeNode* node, TreeVisitFunc visit) {
    if (!node) return;
    postorder_helper(node->left, visit);
    postorder_helper(node->right, visit);
    visit(node->data);
}

void bst_postorder(const BSTree* tree, TreeVisitFunc visit) {
    if (!tree || !visit) return;
    postorder_helper(tree->root, visit);
}

/* Helper queue for Level-Order BFS traversal */
typedef struct BFSNode {
    const TreeNode* tree_node;
    struct BFSNode* next;
} BFSNode;

void bst_levelorder(const BSTree* tree, TreeVisitFunc visit) {
    if (!tree || !tree->root || !visit) return;

    BFSNode* head = (BFSNode*)malloc(sizeof(BFSNode));
    if (!head) return;
    head->tree_node = tree->root;
    head->next = NULL;
    BFSNode* tail = head;

    while (head != NULL) {
        BFSNode* curr = head;
        head = head->next;
        if (head == NULL) {
            tail = NULL;
        }

        const TreeNode* tn = curr->tree_node;
        visit(tn->data);

        if (tn->left != NULL) {
            BFSNode* new_node = (BFSNode*)malloc(sizeof(BFSNode));
            if (new_node) {
                new_node->tree_node = tn->left;
                new_node->next = NULL;
                if (tail == NULL) {
                    head = new_node;
                    tail = new_node;
                } else {
                    tail->next = new_node;
                    tail = new_node;
                }
            }
        }

        if (tn->right != NULL) {
            BFSNode* new_node = (BFSNode*)malloc(sizeof(BFSNode));
            if (new_node) {
                new_node->tree_node = tn->right;
                new_node->next = NULL;
                if (tail == NULL) {
                    head = new_node;
                    tail = new_node;
                } else {
                    tail->next = new_node;
                    tail = new_node;
                }
            }
        }

        free(curr);
    }
}

static int height_helper(const TreeNode* node) {
    if (node == NULL) return -1;
    int lh = height_helper(node->left);
    int rh = height_helper(node->right);
    return (lh > rh ? lh : rh) + 1;
}

int bst_height(const BSTree* tree) {
    if (!tree) return -1;
    return height_helper(tree->root);
}

StatusCode bst_min(const BSTree* tree, int* out_value) {
    if (!tree || !out_value) return STATUS_ERR_NULL_PTR;
    if (tree->root == NULL) return STATUS_ERR_EMPTY;

    TreeNode* curr = tree->root;
    while (curr->left != NULL) {
        curr = curr->left;
    }
    *out_value = curr->data;
    return STATUS_OK;
}

StatusCode bst_max(const BSTree* tree, int* out_value) {
    if (!tree || !out_value) return STATUS_ERR_NULL_PTR;
    if (tree->root == NULL) return STATUS_ERR_EMPTY;

    TreeNode* curr = tree->root;
    while (curr->right != NULL) {
        curr = curr->right;
    }
    *out_value = curr->data;
    return STATUS_OK;
}

size_t bst_size(const BSTree* tree) {
    return tree ? tree->size : 0;
}

bool bst_is_empty(const BSTree* tree) {
    return !tree || tree->size == 0;
}

static bool validate_bst(const TreeNode* node, long min_val, long max_val) {
    if (node == NULL) return true;
    if ((long)node->data <= min_val || (long)node->data >= max_val) {
        return false;
    }
    return validate_bst(node->left, min_val, node->data) &&
           validate_bst(node->right, node->data, max_val);
}

bool bst_is_valid(const BSTree* tree) {
    if (!tree) return false;
    return validate_bst(tree->root, LONG_MIN, LONG_MAX);
}

StatusCode bst_lowest_common_ancestor(const BSTree* tree, int val1, int val2, int* out_lca) {
    if (!tree || !out_lca) return STATUS_ERR_NULL_PTR;
    if (tree->root == NULL) return STATUS_ERR_EMPTY;

    // Both values must exist in the tree
    if (!bst_search(tree, val1) || !bst_search(tree, val2)) {
        return STATUS_ERR_NOT_FOUND;
    }

    const TreeNode* curr = tree->root;
    while (curr != NULL) {
        if (val1 < curr->data && val2 < curr->data) {
            curr = curr->left;
        } else if (val1 > curr->data && val2 > curr->data) {
            curr = curr->right;
        } else {
            *out_lca = curr->data;
            return STATUS_OK;
        }
    }

    return STATUS_ERR_NOT_FOUND;
}

/* ASCII tree visualizer helper */
typedef struct Trunk {
    struct Trunk* prev;
    const char* str;
} Trunk;

static void show_trunks(Trunk* p) {
    if (p == NULL) return;
    show_trunks(p->prev);
    printf("%s", p->str);
}

static void print_ascii_helper(const TreeNode* root, Trunk* prev, bool is_left) {
    if (root == NULL) return;

    const char* prev_str = "    ";
    Trunk trunk = { prev, prev_str };

    print_ascii_helper(root->right, &trunk, true);

    if (!prev) {
        trunk.str = "---";
    } else if (is_left) {
        trunk.str = ".---";
        prev_str = "   |";
    } else {
        trunk.str = "`---";
        prev->str = prev_str;
    }

    show_trunks(&trunk);
    printf(" [%d]\n", root->data);

    if (prev) {
        prev->str = prev_str;
    }
    trunk.str = "   |";

    print_ascii_helper(root->left, &trunk, false);
}

void bst_print_ascii(const BSTree* tree) {
    if (!tree || !tree->root) {
        printf("(empty tree)\n");
        return;
    }
    printf("\n--- Binary Search Tree (ASCII View) ---\n");
    print_ascii_helper(tree->root, NULL, false);
    printf("---------------------------------------\n");
}
