#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>

#include "graph.h"
#include "my_malloc.h"

#ifndef INIT_SIZE 
#define INIT_SIZE 10
#endif

graph_t* allocate_graph(void) 
{
   graph_t *g = myMalloc(sizeof(graph_t));
   g->edges = NULL;
   g->num_edges = 0;
   g->capacity = 0;
   return g;
}

void free_graph(graph_t **g) 
{
   assert(g != NULL && *g != NULL);
   if ((*g)->capacity > 0) {
      free((*g)->edges);
   }
   free(*g);
   *g = NULL;
}

graph_t* enlarge_graph(graph_t *g) 
{
   assert(g != NULL);
   int n = g->capacity == 0 ? INIT_SIZE : g->capacity * 2;
   edge_t *e = myMalloc(n * sizeof(edge_t));
   memcpy(e, g->edges, g->num_edges * sizeof(edge_t));
   free(g->edges);
   g->edges = e;
   g->capacity = n;
   return g;
}

void print_graph(graph_t *g) 
{
   assert(g != NULL);
   fprintf(stderr, "Graph has %d edges and %d edges are allocated\n", g->num_edges, g->capacity);
   edge_t *e = g->edges;
   for (int i = 0; i < g->num_edges; ++i, ++e) {
      printf("%d %d %d\n", e->from, e->to, e->cost);
   }
}

void load_txt(const char *fname, graph_t *graph)
{
   int i = 0;
   FILE *f = fopen(fname, "r");
   if (f != NULL) {
      while (!feof(f)) {
         if (graph->num_edges == graph->capacity) {
            enlarge_graph(graph);
         }
         if(fscanf(f, "%d %d %d\n", &graph->edges[i].from, &graph->edges[i].to, &graph->edges[i].cost) == 3) {
            graph->num_edges++;
            i++;
         }
      }
   }
   i = 0;
   fclose(f);
}

void load_bin(const char *fname, graph_t *graph)
{
   FILE *f = fopen(fname, "rb");
   int info[3];
   if (f != NULL) {
      while (!feof(f)) {
         if (graph->num_edges == graph->capacity) {
            enlarge_graph(graph);
         }
         if (fread(info, sizeof(info), 1, f)) {
            graph->edges[graph->num_edges].from = ntohl(info[0]);
            graph->edges[graph->num_edges].to = ntohl(info[1]);
            graph->edges[graph->num_edges].cost = ntohl(info[2]);
            graph->num_edges++;
         }
      }
      print_graph(graph);
   }
   fclose(f);
}

void save_txt(const graph_t *const graph, const char *fname)
{
   FILE *f = fopen(fname, "w");
   if (f != NULL) {
      for (int i = 0; i < graph->num_edges; i++) {
         fprintf(f, "%d %d %d\n", graph->edges[i].from, graph->edges[i].to, graph->edges[i].cost);
      }
   }
   fclose(f);
}

void save_bin(const graph_t *const graph, const char *fname)
{
   FILE *f = fopen(fname, "wb");
   int info[3];
   if (f != NULL) {
      for (int i = 0; i < graph->num_edges; i++) {
         info[0] = htonl(graph->edges[i].from);
         info[1] = htonl(graph->edges[i].to);
         info[2] = htonl(graph->edges[i].cost);
         fwrite(info, sizeof(info), 1, f);
      }
   }
   fclose(f);
}

int load_graph_simple(const char *fname, graph_t *g) 
{
   int c = 0;
   int exit = 0;
   FILE *f = fopen(fname, "r");
   while (f && !exit) {
      if (g->num_edges == g->capacity) {
         enlarge_graph(g);
      }
      edge_t *e = g->edges + g->num_edges;
      while (g->num_edges < g->capacity) {
         int r = fscanf(f, "%d %d %d\n", &(e->from), &(e->to), &(e->cost));
         if (r == 3) {
            g->num_edges += 1;
            c += 1;
            e += 1;
         } else {
            exit = 1;
            break;
         }
      }
   }
   if (f) {
      fclose(f);
   }
   return c;
}
