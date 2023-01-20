#include <stdio.h>
#include <stdlib.h>

#ifndef MAX
#define MAX 70000
#endif

typedef struct __attribute__((packed)) node {
    struct node *left;
    struct node *right;
    int key;
} node_t;

node_t *node_create(int key) {
    node_t *node = (node_t*)malloc(sizeof(node_t));
    node->left = node->right = NULL;
    node->key = key;
    return node;
}

void node_insert(node_t *node, int* from, int to, int last_left) {
    if (*from <= to) {
        if (*from < node->key) {
            if (node->left == NULL) {
                node->left = node_create(*from);
                (*from)++;
            }
            node_insert(node->left, from, to, node->key);
        }
        if (*from == node->key) {
            (*from)++;
        }
        if ((*from > node->key) && (*from < last_left) && (*from <= to)) {
            if (node->right == NULL) {
                node->right = node_create(*from);
                (*from)++;
            }
            node_insert(node->right, from, to, last_left);
        }
    }
}

int delete_minimum(node_t *node, node_t *parent) {
    int minimum_key;
    if (node->left) {
        minimum_key = delete_minimum(node->left, node);
    } else {
        minimum_key = node->key;
        if (node == parent->left) {
            parent->left = node->right;
        } else {
            parent->right = node->right;
        }
        free(node);
    }
    return minimum_key;
}

node_t *node_delete(node_t *node, int *from , int to) {
    int delete = 0;
    if (*from <= to) {


        if (*from < node->key) {
            if (node->left) {
                node->left = node_delete(node->left, from, to);
            }
            *from = node->key;
        }

        if ((*from == node->key) && (*from <= to)) {
            delete = 1;
            (*from)++;
        }

        if ((*from > node->key) && (*from <= to)) {
            if (node->right) {
                node->right = node_delete(node->right, from, to);
            }
        }
        
        if (delete) {
            if (!(node->left) && !(node->right)) {
                free(node);
                node = NULL;
            } else if (!(node->left) || !(node->right)) {
                node_t *new;
                new = (node->right) ? node->right : node->left;
                free(node);
                node = new;
            } else {
                node->key = delete_minimum(node->right, node);
            }
        }


    }
    return node;
}

void node_get_stat(node_t *node, int* depth, int *count) {
    int depth_left, depth_right;
    depth_left = depth_right = 0;
    if (node->right) {
        node_get_stat(node->right, &depth_right, count);
    }
    if (node->left) {
        node_get_stat(node->left, &depth_left, count);
    }
    (*count)++;
    *depth = depth_left < depth_right ? depth_right : depth_left;
    (*depth)++;
    free(node);
    node = NULL;
}

typedef struct tree {
   node_t *root;
} tree_t;

void tree_init(tree_t *tree) {
    tree->root = NULL;
}

void tree_insert(tree_t *tree, int *from, int to) {
    if (tree->root == NULL) {
        tree->root = node_create(*from);
        (*from)++;
    }
    node_insert(tree->root, from, to, MAX);
}


void tree_delete(tree_t *tree, int *from, int to) {
    if (tree->root) {
        tree->root = node_delete(tree->root, from, to);
    }
}

void tree_print_result(tree_t *tree) {
    int nodes, depth;
    nodes = depth = 0;
    node_get_stat(tree->root, &depth, &nodes);
    fprintf(stdout, "%d %d\r\n", nodes, depth - 1);
}

int main(int argc, char *argv[]) {

    int count, from, to;
    unsigned char code;
    tree_t tree;
    tree_init(&tree);
    fscanf(stdin, "%d\n", &count);
    for (int i = 0; i < count; i++) {
        fscanf(stdin, "%c %d %d\n", &code, &from, &to);
        if (code == 'i') {
            tree_insert(&tree, &from, to);
        } else {
            tree_delete(&tree, &from, to);
        }
    }
    tree_print_result(&tree);
    return EXIT_SUCCESS;
}
