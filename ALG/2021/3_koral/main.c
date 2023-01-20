/*
 * autor: @timusfed
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node
{
    int neighbors[3];
    int neighbors_red_count[3];
    int neighbors_count;
    bool red;
} Node_t;

typedef struct Graph
{
    int vertices;
    int red_count;
    Node_t *nodes;
    int v1, v2;
} Graph_t;

void initGraph(Graph_t *graph, int N, int red);
void showGraphInfo(Graph_t *graph);

void readNode(Graph_t *graph, int id, bool isRed, int next);

int nodeCalculateRedNext(Graph_t *graph, int id, int id_prev);
void graphGetSolution(Graph_t *graph);

int main(int argc, char *argv[])
{
    int N = 0, red = 0, ret;

    // read graph info
    // todo ok
    if ((ret = scanf("%d %d", &N, &red)) != 2)
    {
        return EXIT_FAILURE;
    }

    // create graph
    Graph_t graph;
    // create nodes

    // init graph
    initGraph(&graph, N, red);

    // write the graph
    // todo ok
    int val, next;
    while ((ret = scanf("%d %d", &val, &next)) == 2)
    {
        // save the node
        bool isRed = val <= red ? true : false;
        readNode(&graph, val - 1, isRed, next - 1);

        // todo and vice versa
        isRed = next <= red ? true : false;
        readNode(&graph, next - 1, isRed, val - 1);
    }

    nodeCalculateRedNext(&graph, 0, -1);
    graphGetSolution(&graph);

    return EXIT_SUCCESS;
}

int nodeCalculateRedNext(Graph_t *graph, int id, int id_prev) {
    int red_count = 0;
    for (int i = 0; i < graph->nodes[id].neighbors_count; i++) {
        if (graph->nodes[id].neighbors[i] != id_prev) {
            graph->nodes[id].neighbors_red_count[i] = nodeCalculateRedNext(graph, graph->nodes[id].neighbors[i], id);
            red_count += graph->nodes[id].neighbors_red_count[i];
        }
    }
    return (red_count + graph->nodes[id].red);
}

void graphGetSolution(Graph_t *graph) {
    int diff = graph->red_count;
    for (int i = 0; i < graph->vertices; i++) {
        for (int j =0 ; j < graph->nodes[i].neighbors_count; j++) {
            int after = graph->nodes[i].neighbors_red_count[j];
            int before = graph->red_count - after;
            int new_diff;
            if (before > after) {
                new_diff = before - after;
            } else {
                new_diff = after - before;
            }
            int v1, v2;
            if (i < graph->nodes[i].neighbors[j]) {
                v1 = i + 1;
                v2 = graph->nodes[i].neighbors[j] + 1;
            } else {
                v2 = i + 1;
                v1 = graph->nodes[i].neighbors[j] + 1;
            }
            if (new_diff < diff) {
                diff = new_diff;
                graph->v1 = v1;
                graph->v2 = v2;
                
            } else if (new_diff == diff) {
                if ((v1 < graph->v1) || ((v1 == graph->v1) && (v2 < graph->v2))) {
                    graph->v1 = v1;
                    graph->v2 = v2;
                } 
            }
        }
    }
    printf("%d %d\n", graph->v1, graph->v2);
}

void readNode(Graph_t *graph, int id, bool isRed, int next)
{
    int count = graph->nodes[id].neighbors_count;
    graph->nodes[id].red = isRed;
    graph->nodes[id].neighbors[count] = next;
    graph->nodes[id].neighbors_count++;
}

void initGraph(Graph_t *graph, int N, int red)
{
    graph->vertices = N;
    graph->red_count = red;
    graph->nodes = calloc(N, sizeof(Node_t));
    graph->v1 = graph->v2 = N;
}
