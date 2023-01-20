#include <iostream>
#include <vector>
#include <tuple>
#include <queue>

using namespace std;

class Node
{
public:
    int value;
    Node* left;
    Node* right;
    int best_case;
    int depth;

    Node(int v)
    {
        value = v;
        left = nullptr;
        right = nullptr;
        best_case = 0;
        depth = 0;
    }

    void update_case()
    {
        int r_bc = right ? right->best_case : 0;
        int l_bc = left ? left->best_case : 0;

        int r_d = right ? right->depth : 0;
        int l_d = left ? left->depth : 0;

        best_case = max(r_bc + l_d + 1, l_bc + r_d + 1);
    }

    void update_depth()
    {
        int r_d = right ? right->depth : 0;
        int l_d = left ? left->depth : 0;
        depth = max(r_d, l_d) + 1;
    }

    bool is_leaf()
    {
        return (right == nullptr && left == nullptr);
    }
};

void printTree(Node *n, int pos = 0) {
    if (n == nullptr) {
        for (int i = 0; i < pos; ++i)
            cout << "\t";
        cout << endl;
        return;
    }
    printTree(n->right, pos + 1);
    for (int i = 0; i < pos; i++)
        cout << "\t   ";
    printf("%d[d:%d, c:%d]", n->value, n->depth, n->best_case);
    printTree(n->left, pos + 1);
}

void insert(int v, Node* n);
void traverse(Node* n);

int main() {

    int n = 0;
    scanf("%d", &n);

    Node* root = nullptr;

    for (int i = 0; i < n; ++i) {
        int v = 0;
        scanf("%d", &v);

        if (root == nullptr)
        {
            root = new Node(v);
        }
        else
        {
            insert(v, root);
        }
    }

    traverse(root);

    //printTree(root, 0);

    // |||||||||||||||||||||||||||||||||||||||||||

    int l_best = root->left->best_case;
    int l_d = root->left->depth;

    //printf("l: best: %d, d: %d\n", l_best, l_d);

    int r_best = root->right->best_case;
    int r_d = root->right->depth;

    //printf("r: best: %d, d: %d\n", r_best, r_d);

    int result = max(l_best + r_d, r_best + l_d) + 1;
    printf("%d", result);

    return 0;
}


void traverse(Node* n)
{
    if (n->is_leaf())
    {
        n->update_depth();
        n->update_case();
        return;
    }
    else
    {
        if (n->right != nullptr)
        {
            traverse(n->right);
            n->update_depth();
            n->update_case();
        }

        if (n->left != nullptr)
        {
            traverse(n->left);
            n->update_depth();
            n->update_case();
        }
    }

}


void insert(int v, Node* n)
{
    if (v > n->value)
    {
        if (!n->right)
        {
            n->right = new Node(v);
        }
        else
        {
            insert(v, n->right);
        }
    }

    if (v < n->value)
    {
        if (!n->left)
        {
            n->left = new Node(v);
        }
        else
        {
            insert(v, n->left);
        }
    }
}
