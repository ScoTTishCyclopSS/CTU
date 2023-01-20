#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;

#define Path 0
#define Cut 1

class Node {
public:
    int idx, value, path, part;
    Node *right, *left;

    Node(int i) {
        right = nullptr;
        left = nullptr;
        idx = i;
        value = 0;
        path = 0;
        part = 0;
    }
};

tuple<int, int> traverse_me(Node *curr_n);

void show_graph();

bool is_leaf(Node *n);

Node *root = nullptr;

vector<Node> graph; // nodes data

int main() {
    int p, s, start = -1; // params
    scanf("%d %d", &p, &s);


    for (int i = 0; i < p + s; i++) {
        graph.emplace_back(i);
    }

    for (int i = 0; i < s; i++) {
        int id, l, r, l_v, r_v;
        scanf("%d %d %d %d %d\n", &id, &l, &l_v, &r, &r_v);
        //printf("%d %d %d %d %d\n", id, l, l_v, r, r_v);

        if (start == -1) {
            start = id;
        }

        graph[id].left = &graph[l]; // set lest child ptr
        graph[id].right = &graph[r]; // set right child ptr

        graph[l].value = l_v; // cost to get right
        graph[r].value = r_v; // cost to get left
    }

    //show_graph();
    root = &graph[start];
    tuple<int, int> result = traverse_me(&graph[start]);
    printf("%d", max(get<Path>(result), get<Cut>(result)));
    return 0;
}

tuple<int, int> traverse_me(Node *curr_n) {

    if (is_leaf(curr_n)) {
        //printf("%d -> leaf --> (%d 0)\n", curr_n->idx, curr_n->value);
        return make_tuple(curr_n->value, 0);
    }

    tuple<int, int> left_pair = traverse_me(curr_n->left);
    tuple<int, int> right_pair = traverse_me(curr_n->right);

    // if you go further
    int best_path = max(
            curr_n->value + get<Path>(left_pair) + get<Cut>(right_pair),
            curr_n->value + get<Path>(right_pair) + get<Cut>(left_pair)
    );

    // if you let child create independent solution (cut)
    int best_cut = max({
                               get<Cut>(left_pair) + get<Cut>(right_pair),
                               get<Path>(right_pair) + get<Path>(left_pair)
                       });

    // pre-leaf has best cut as combination of children paths
    if (get<Cut>(right_pair) == 0 && get<Cut>(left_pair) == 0) {
        best_cut = get<Path>(left_pair) + get<Path>(right_pair);
    }

//    printf("%d -> l:[%d %d] | r:[%d %d] --> (%d %d)\n",
//           curr_n->idx,
//           get<Path>(left_pair), get<Cut>(left_pair),
//           get<Path>(right_pair), get<Cut>(right_pair),
//           best_path, best_cut
//    );

    if (curr_n == root)
        return make_tuple(best_cut, -1);
    return make_tuple(best_path, best_cut);
}

bool is_leaf(Node *n) {
    return (n->left == nullptr && n->right == nullptr);
}

void show_graph() {
    for (Node n: graph) {
        int me = n.idx;
        int left = n.left == nullptr ? -1 : n.left->idx;
        int right = n.right == nullptr ? -1 : n.right->idx;
        int left_v = n.left == nullptr ? -1 : n.left->value;
        int right_v = n.right == nullptr ? -1 : n.right->value;
        printf("%d -> %d(%d), %d(%d)\n", me, left, left_v, right, right_v);
    }
}
