#include <functional>
#include "bst_tree.h"

void bst_tree::insert(long long data) {

    node *n = new node(data);

    if (root == nullptr) {
        root = n;
        if (root.compare_exchange_strong(n, root))return;
    }

    node *curr = root;
    node *previous;

    while(true) {
        previous = curr;

        if (data < previous->data)
        {
            curr = curr->left;
            if (curr == nullptr) {
                previous->left = n;
                if (previous->left.compare_exchange_strong(n, previous->left))
                    return;
            }
        }
        else
        {
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
