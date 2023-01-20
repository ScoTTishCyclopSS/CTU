/*
* author: @timusfed
*/

#include <cstdio>
#include <vector>
#include <cstdlib>
//#include "t.h"

using namespace std;

struct Node {
    int value;
    struct Node *left, *right;
};



Node * initNode(int value);
Node *freeNode(Node *node);
void insertNode(int n1, int n2, Node *node);
Node *deleteNode(int n1, int n2, Node *node);
Node *minValue(Node *node, int &min);
int height(Node *root);

int finalNodes = 0;

int main(int argc, char* argv[])
{
    int operationCount = 0, n1, n2;
    struct Node* root = nullptr;
    char operation;

    if (scanf("%d", &operationCount)!= 1) return 1;

    for (int i = 0; i < operationCount; ++i)
    {
        scanf(" %c %d %d", &operation, &n1, &n2);
        if (operation == 'i') {
            if (!root) {
                root = initNode(n1);
                n1 += 1;
            }
            insertNode(n1, n2, root);

        }
        else {
            root = deleteNode(n1, n2, root);
        }
    }
    //printTree(root, nullptr, false);

    finalNodes = 0;
    printf("%d %d", finalNodes, height(root) - 1);
    return 0;
}

void insertNode(int n1, int n2, Node *node) {
    if (n1 <= n2){
        if (n1 == node->value) n1 += 1; // duplicates

        int new_n2 = n2 >= node->value ? node->value - 1: n2;
        int new_n1 = n1 < node->value ? node->value + 1: n1;

        if (n1 <= new_n2) {
            if (!node->left) {
                node->left = initNode(n1);
                n1 += 1;
            }
            insertNode(n1, new_n2, node->left);
        }

        if (new_n1 <= n2) {
            if (!node->right) {
                node->right = initNode(new_n1);
                new_n1 += 1;
            }
            insertNode(new_n1, n2, node->right);
        }
    }
}

Node *deleteNode(int n1, int n2, Node *node) {
    if (n1 <= n2) {

        int new_n2 = n2 >= node->value ? node->value - 1: n2;
        int new_n1 = n1 < node->value ? node->value + 1: n1;

        if (n1 <= new_n2) {
            if (node->left) {
                node->left = deleteNode(n1, new_n2, node->left);
            }
        }

        if (new_n1 <= n2) {
            if (node->right) {
                node->right = deleteNode(new_n1, n2, node->right);
            }
        }

        if (node->value >= n1 && node->value <= n2) {
            node = freeNode(node);
        }
    }
    return node;
}

Node *initNode(int value) {
    auto* n = (struct Node*)malloc(sizeof (struct Node));
    n->value = value;
    n->left = n->right = nullptr;
    return n;
}

Node *freeNode(Node *node) {

    // leaf
    if (!node->left && !node->right) {
        free(node);
        node = nullptr;
        return  node;
    }


    // one child
    if (!node->left || !node->right) {
        struct Node* child = node->left ? node->left : node->right;
        node = child;
        return node;
    }

        // both children
    else {
        node->right = minValue(node->right, node->value);
        return node;
    }
}

Node *minValue(Node *node, int &min) {
    if (node->left) {
        node->left = minValue(node->left, min);
        return node;
    }

    min = node->value;
    node = freeNode(node);
    return node;
}

int height(Node *root) {
    if (!root)
        return 0;

    finalNodes += 1;
    int leftHeight = height(root->left);
    int rightHeight = height(root->right);
    return max(leftHeight, rightHeight) + 1;
}