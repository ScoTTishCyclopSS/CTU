#include <algorithm>
#include <vector>
#include <iostream>

struct Node {
    int left;
    int right;
    int lcost;
    int rcost;
    int inside;
    int outside;
};

void cost(const int id, const int parent, std::vector<Node> &arr) {
    // structure fields are default initialized to zeros
    // allowed values are in [1, 200]
    if (arr[id].lcost == 0) {
        arr[id].inside = 0;
        int upcost = (arr[parent].left == id) ? arr[parent].lcost : arr[parent].rcost;
        arr[id].outside = upcost;
        return;
    }

    cost(arr[id].left, id, arr); // traverse to left
    cost(arr[id].right, id, arr); // traverse to right

    arr[id].inside = std::max(arr[arr[id].left].outside + arr[arr[id].right].outside,
                              arr[arr[id].left].inside + arr[arr[id].right].inside);

    int upcost = (parent != -1) ? ((arr[parent].left == id) ? arr[parent].lcost : arr[parent].rcost) : 0;
    arr[id].outside = std::max(upcost + arr[arr[id].left].inside + arr[arr[id].right].outside,
                               upcost + arr[arr[id].right].inside + arr[arr[id].left].outside);

    //printf("%d -> %d %d\n", id, arr[id].inside, arr[id].outside);
}

int sumcost(const int id, std::vector<Node> &arr) {
    printf("id: %d\n", id);

    // structure fields are default initialized to zeros
    // allowed values are in [1, 200]
    if (arr[id].lcost == 0) {
        // printf("end\n");
        return 0;
    }

    if (arr[id].inside > arr[id].outside) {
        // printf("inside\n");
        return arr[id].lcost + arr[id].rcost + sumcost(arr[id].left, arr) + sumcost(arr[id].right, arr);
    }

    if (arr[id].outside > arr[id].inside) {
        // printf("outside\n");
        return std::max(arr[id].lcost, arr[id].rcost) + sumcost(arr[id].left, arr) + sumcost(arr[id].right, arr);
    }
}

int main() {
    int P, S;
    std::cin >> P >> S;

    int nodeslen = S * 2 + 1;

    std::vector<Node> confluence(nodeslen);

    int I, left, lcost, right, rcost;
    std::cin >> I >> left >> lcost >> right >> rcost;
    int root = I;

    for (int i = 0; i < S; i++) {
        confluence[I] = {.left = left, .right = right, .lcost = lcost, .rcost = rcost};
        std::cin >> I >> left >> lcost >> right >> rcost;
    }

    // this is correct, because we always have at least three nodes
    // so there will be always at least one recursive call to set up `parent`
    cost(root, -1, confluence);

    printf("%d\n", confluence[root].inside);
    //std::cout << sumcost(root, confluence) << "\n";

    return 0;
}