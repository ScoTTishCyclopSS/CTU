#ifndef __GRAPH_UTILS_H__
#define __GRAPH_UTILS_H__

typedef struct {
   int from;
   int to;
   int cost;
} edge_t;

typedef struct {
   edge_t *edges;
   int num_edges;
   int capacity;
} graph_t;


graph_t* allocate_graph(void);

void free_graph(graph_t **g);

graph_t* enlarge_graph(graph_t *g);

void print_graph(graph_t *g);

void load_txt(const char *fname, graph_t *graph);

void load_bin(const char *fname, graph_t *graph);

void save_txt(const graph_t *const graph, const char *fname);

void save_bin(const graph_t *const graph, const char *fname);

int load_graph_simple(const char *fname, graph_t *g);

#endif
