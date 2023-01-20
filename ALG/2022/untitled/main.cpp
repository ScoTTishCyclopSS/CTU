#include <iostream>
#include <tuple>

using namespace std;

class Node {
public:

    int a, b;
    Node *l, *r;

    Node(int a_, int b_) {
        a = a_;
        b = b_;
        l = nullptr;
        r = nullptr;
    }

    bool isOverlap(int a_, int b_) { // for insert
        return a <= b_ && a_ <= b;
    }

    bool isOverlap(int c) { // for delete
        return a <= c && c <= b;
    }

    void ChangeMe(int a_, int b_) {
        b = b_ >= b ? b_ : b;
        a = a_ <= a ? a_ : a;
    }

    int NumbersRepresented() {
        return b - a + 1;
    }
};

void insertion(Node *node, int a, int b);

Node *Merge(Node *curr, int a, int b, Node *P, Node *start);

Node *deletion(Node *node, int c);

Node *Min(Node *curr, Node *toSwap);

Node *deleteMe(Node *node);

int CalculateResult(Node *node, int *nodeCount, int *numRepr);

void printTree(Node *nNode, int pos);

int main() {
    int n;
    char op;
    Node *root = nullptr;

    scanf("%d", &n); // n - operations

    for (int i = 0; i < n; ++i) {
        int a, b;
        scanf("%s", &op); // operation
        if (op == 'i') {
            scanf("%d %d", &a, &b);
            //printf("[%d]inserting %d %d...\n", i, a, b);
            // if no root yet
            if (!root) {
                root = new Node(a, b);
                continue;
            }
            insertion(root, a, b);
        } else {
            //printf("[%d]deleting %d...\n", i, a);
            scanf("%d", &a);
            root = deletion(root, a);
        }
        //printf("-------------------------\n");
        //printTree(root, 0);
    }

    //printTree(root, 0);

    int u = 0, m = 0;
    int h = CalculateResult(root, &u, &m) - 1;
    printf("%d %d %d", u, h, m);
}

void insertion(Node *node, int a, int b) {
    // -> right
    if (a > node->b) {
        // no child on a right? -> create
        if (!node->r) {
            node->r = new Node(a, b);
        }
        insertion(node->r, a, b);
    }

    // -> left
    if (b < node->a) {
        // no child on a left? -> create
        if (!node->l) {
            node->l = new Node(a, b);
        }
        insertion(node->l, a, b);
    }

    if (node->isOverlap(a, b)) {
        node->ChangeMe(a, b);
        node->l = Merge(node->l, a, b, node, node);
        node->r = Merge(node->r, a, b, node, node);
    }
}

// start -> node, where we started search, P -> parent for U
Node *Merge(Node *curr, int a, int b, Node *P, Node *start) {
    if (curr) {
        if (curr->isOverlap(a, b)) {
            // update START
            start->ChangeMe(curr->a, curr->b);
        }

        if (!curr->isOverlap(a, b)) {
            // update P
            P = curr;
        }

        if (!curr->isOverlap(a, b) && P != curr) {
            // U found
            return curr;
        }

        Node *left_merge = Merge(curr->l, a, b, curr, start);
        Node *right_merge = Merge(curr->r, a, b, curr, start);

        if (curr != P) {
            if (!left_merge && !right_merge)
                return nullptr;

            if (left_merge)
                return left_merge;

            return right_merge;
        }

        curr->l = left_merge;
        curr->r = right_merge;
        return curr;
    }
    return nullptr;
}

Node *deletion(Node *node, int c) {
    if (node) {
        if (c > node->b)
            node->r = deletion(node->r, c);

        if (c < node->a)
            node->l = deletion(node->l, c);

        if (node->isOverlap(c))
            node = deleteMe(node);
    }
    return node;
}

Node *deleteMe(Node *node) {
    if (!node->r && !node->l) {
        node = nullptr;
        return node;
    }

    // both children
    if (node->r && node->l) {
        node->r = Min(node->r, node);
        return node;
    }

    // one child
    Node *child = node->l ? node->l : node->r;
    return child;
}

Node *Min(Node *curr, Node *toSwap) {
    if (curr->l) {
        curr->l = Min(curr->l, toSwap);
        return curr;
    }
    toSwap->a = curr->a;
    toSwap->b = curr->b;
    curr = deleteMe(curr);
    return curr;
}

// -----------------------------------------------------

int CalculateResult(Node *node, int *nodeCount, int *numRepr) {
    if (node) {
        *nodeCount += 1;
        *numRepr += node->NumbersRepresented();
        int left_d = CalculateResult(node->l, nodeCount, numRepr);
        int right_d = CalculateResult(node->r, nodeCount, numRepr);
        return max(left_d, right_d) + 1;
    }
    return 0;
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
    printf("[%d %d]", n->a, n->b);
    printTree(n->l, pos + 1);
}