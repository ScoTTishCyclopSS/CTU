#include<iostream>
#include<vector>

using namespace std;

class Node {
public:
    int val, h;
    Node *l, *r;

    Node(int v) {
        val = v;
        l = nullptr;
        r = nullptr;
        h = 1;
    }

    bool isLeaf() {
        return (!l && !r);
    }

    void updateHeight() {
        int l_h = l ? l->h : 0;
        int r_h = r ? r->h : 0;

        h = max(l_h, r_h) + 1;
    }

    int getBalance() {
        int l_h = l ? l->h : 0;
        int r_h = r ? r->h : 0;

        return l_h - r_h;
    }

    bool hasTwoChildren() {
        return (l && r);
    }
};

int insert_val(int s, int A, int B, int mod) {
    return (A * s + B) % mod;
}

Node *insert(Node *curr, int val, int diff, int *rotations);

Node *deletion(Node *curr, int val, int diff, int *rotations);

Node *findMax(Node *curr);

Node *leftRotation(Node *node);

Node *rightRotation(Node *node);

void printTree(Node *n, int pos);

int main() {
    int diff, A, B, mod, n;
    scanf("%d %d %d %d %d", &diff, &A, &B, &mod, &n);
    vector<int> values;

    Node *root = nullptr;
    Node *relaxRoot = nullptr;

    int avlRotations = 0;
    int relaxAvlRotations = 0;

    // create values
    for (int i = 0; i < n; i++) {
        int val = i > 0 ? insert_val(values[i - 1], A, B, mod) : B;
        values.push_back(val);
        root = i > 0 ? insert(root, val, 1, &avlRotations) : new Node(val);
        relaxRoot = i > 0 ? insert(relaxRoot, val, diff, &relaxAvlRotations) : new Node(val);
    }

    for (int i = 2; i < values.size(); i += 3)
    {
        int val = values[i];
        printf("deleting %d\n", val);
        root = deletion(root, val, 1, &avlRotations);
        relaxRoot = deletion(relaxRoot, val, diff, &relaxAvlRotations);
    }

    printf("%d %d\n", root->h - 1, avlRotations);
    printf("%d %d", relaxRoot->h - 1, relaxAvlRotations);

    return 0;
}

Node *deletion(Node *curr, int val, int diff, int *rotations) {

    if (curr)
    {
        // traverse
        if (val > curr->val)
            curr->r = deletion(curr->r, val, diff, rotations);
        else if (val < curr->val)
            curr->l = deletion(curr->l, val, diff, rotations);
        else {
            // two children
            if (curr->hasTwoChildren()) {
                Node *max = findMax(curr->l);
                curr->val = max->val;
                curr->l = deletion(curr->l, max->val, diff, rotations);
            } else if (curr->isLeaf()) {   // no children
                return nullptr;
            } else {
                // one child -> replace curr with it
                Node *child = curr->l ? curr->l : curr->r;
                return child;
            }
        }

        curr->updateHeight();
        int balance = curr->getBalance();

        int l_b = curr->l ? curr->l->getBalance() : 0;
        int r_b = curr->r ? curr->r->getBalance() : 0;

        // Single Left Rotation
        if (balance < -diff && r_b <= 0) {
            *rotations += 1;
            printf("balance:%d val:%d r_b:%d\n", balance, val, r_b);
            return leftRotation(curr);
        }

        // Single Right Rotation
        if (balance > diff && l_b >= 0) {
            *rotations += 1;
            return rightRotation(curr);
        }

        // Right Left Rotation
        if (balance < -diff && r_b > 0) {
            *rotations += 1;
            curr->r = rightRotation(curr->r);
            return leftRotation(curr);
        }

        // Left Right Rotation
        if (balance > diff && l_b < 0) {
            *rotations += 1;
            curr->l = leftRotation(curr->l);
            return rightRotation(curr);
        }

        return curr;
    }

    return nullptr;
}

Node *findMax(Node *curr) {
    if (curr->r) {
        curr = findMax(curr->r);
        return curr;
    }
    return curr;
}

Node *insert(Node *curr, int val, int diff, int *rotations) {
    if (curr)
    {
        // traverse
        if (val > curr->val)
            curr->r = insert(curr->r, val, diff, rotations); // right
        else if (val < curr->val)
            curr->l = insert(curr->l, val, diff, rotations); // left
        else
            return curr; // no need to insert

        // update height
        curr->updateHeight();

        // check balance
        int balance = curr->getBalance();

        // Single Left Rotation
        if (balance < -diff && curr->r->val < val) {
            *rotations += 1;
            return leftRotation(curr);
        }

        // Single Right Rotation
        if (balance > diff && curr->l->val > val) {
            *rotations += 1;
            return rightRotation(curr);
        }

        // Right Left Rotation
        if (balance < -diff && curr->r->val > val) {
            *rotations += 1;
            curr->r = rightRotation(curr->r);
            return leftRotation(curr);
        }

        // Left Right Rotation
        if (balance > diff && curr->l->val < val) {
            *rotations += 1;
            curr->l = leftRotation(curr->l);
            return rightRotation(curr);
        }

        return curr; // no need to rotate
    }

    return new Node(val); // new leaf
}

Node *leftRotation(Node *node) {
    Node *right = node->r;
    Node *r_l = right->l;

    right->l = node;
    node->r = r_l;

    node->updateHeight();
    right->updateHeight();

    return right;
}

Node *rightRotation(Node *node) {
    Node *left = node->l;
    Node *l_r = left->r;

    left->r = node;
    node->l = l_r;

    node->updateHeight();
    left->updateHeight();

    return left;
}

void printTree(Node *n, int pos = 0) {
    if (n == nullptr) {
        for (int i = 0; i < pos; ++i)
            cout << "\t";
        cout << endl;
        return;
    }
    printTree(n->r, pos + 1);
    for (int i = 0; i < pos; i++)
        cout << "\t   ";
    printf("[%d](%d)", n->val, n->h);
    printTree(n->l, pos + 1);
}

