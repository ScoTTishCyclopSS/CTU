#include <functional>
#include "bst_tree.h"
#include <iostream>

void bst_tree::insert(long long data) {

    node *new_node = new node(data);
    node *curr, *null_node = nullptr;

    if(root == nullptr)
        if (root.compare_exchange_strong(null_node, new_node))
            return;

    curr = root;

    while (true) {

        // left traverse
        if (data < curr->data) {
            if (curr->left == nullptr) {
                node *null_node1 = nullptr;
                if (curr->left.compare_exchange_strong(null_node1, new_node))
                    return;
            }
            curr = curr->left;
        } else if (data > curr->data){
            if (curr->right == nullptr) {
                node *null_node1 = nullptr;
                if (curr->right.compare_exchange_strong(null_node1, new_node))
                    return;
            }
            curr = curr->right;
        }
    }
}

bst_tree::~bst_tree() {
    // Rekurzivni funkce pro pruchod stromu a dealokaci pameti prirazene jednotlivym
    // uzlum
    std::function<void(node*)> cleanup = [&](node * n) {
        if(n != nullptr) {
            cleanup(n->left);
            cleanup(n->right);

            delete n;
        }
    };
    cleanup(root);
}
