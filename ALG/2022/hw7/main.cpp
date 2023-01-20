#include <iostream>
#include <vector>
#include <tuple>
#include <queue>

using namespace std;

#define NODE 0
#define COST 1
#define NOT_FREE 2

class Node {
public:

    int v, payed, satisfaction;
    vector<tuple<Node *, int, bool>> inDegree;

    Node(int value) {
        v = value;
        payed = INT32_MAX;
        satisfaction = INT_MIN;
    }

    bool Update(int p, int s, int max_pay)
    {
        bool res = false;

        if (payed != INT32_MAX && payed + p > max_pay)
            return res;

        if (p < payed) {
            payed = p;
            res = true;
        }

        if (s > satisfaction) {
            satisfaction = s;
            res = true;
        }
        return res;
    }
};

class Struct
{
public:
    Node *node;
    int min_pay, max_sat;

    Struct(Node *n, int m_p, int m_s)
    {
        node = n;
        min_pay = m_p;
        max_sat = m_s;
    }
};


void printQueue(queue<Struct *> q)
{
    printf("\n");
    //printing content of queue
    while (!q.empty()){
        Struct *s = q.front();
        printf("[%d %d %d], ", s->node->v, s->min_pay, s->max_sat);
        q.pop();
    }
    printf("\n\n");
}

void rateAll(Node *root, int max_pay);

int main() {
    int nodes, edges, max_pay;
    scanf("%d %d %d", &nodes, &edges, &max_pay);

    vector<Node *> graph;

    // create nodes
    for (int i = 1; i <= nodes; i++) {
        Node *new_node = new Node(i);
        graph.push_back(new_node);
    }

    // filling graph
    for (int i = 0; i < edges; ++i) {
        int n1, n2, satisfaction, payable;
        scanf("%d %d %d %d", &n1, &n2, &satisfaction, &payable);

        tuple<Node *, int, bool> inDegreeNode = make_tuple(graph[n1 - 1], satisfaction, payable);
        graph[n2 - 1]->inDegree.push_back(inDegreeNode);

        //printf("%d <- %d\n", n2, n1);
    }

    rateAll(graph[0], max_pay);

    // show nodes
    for (Node *n : graph) {
        printf("%d -> %d, %d\n", n->v, n->payed, n->satisfaction);
    }

    return 0;
}

void rateAll(Node *root, int max_pay) {
    queue<Struct *> q;
    q.push(new Struct(root, 0, 0));

    while (!q.empty()) {
        Struct* aStruct = q.front();
        q.pop();

        Node* n = aStruct->node;
        bool updated = n->Update(aStruct->min_pay, aStruct->max_sat, max_pay);
        printf("%d [%d %d] putting: ", n->v, n->payed, n->satisfaction);

        if (!n->inDegree.empty() && updated){
            for (auto &inNodeObj : n->inDegree) {
                Node *inNode = get<NODE>(inNodeObj);
                bool not_free = get<NOT_FREE>(inNodeObj);
                int cost = get<COST>(inNodeObj);

                int payed_now = aStruct->min_pay + not_free;
                int satisfied_now = aStruct->max_sat + cost;

                q.push(new Struct(inNode, payed_now, satisfied_now));
                printf("%d (%d, %d) ", inNode->v, payed_now, satisfied_now);
            }
        }
        printQueue(queue<Struct *>(q));
        free(aStruct);
    }
}


