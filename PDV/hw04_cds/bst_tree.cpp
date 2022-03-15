#include <functional>
#include "bst_tree.h"
#include <iostream>

void bst_tree::insert(long long data) {

    node *n = new node(data);
    node *curr = root;
    node *previous;

    if (curr == nullptr)
        if (root.compare_exchange_strong(curr, n))
            return;

    return;

    while (true) {
        previous = curr;

        if (data < previous->data) {
            curr = curr->left;
            if (curr == nullptr) {
                previous->left = n;
                if (previous->left.compare_exchange_strong(n, previous->left))
                    return;
            }
        } else {
            curr = curr->right;
            if (curr == nullptr) {
                previous->right = n;
                if (previous->right.compare_exchange_strong(n, previous->right))
                    return;
            }
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
