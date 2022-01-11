#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "graph.h"

/* This is a testing file just to be able to compile the
 * examples of binary files to convert graph from txt->bin
 * and bin -> txt
 */

// - function -----------------------------------------------------------------
graph_t *allocate_graph()
{
  graph_t *g = malloc(sizeof(graph_t));
  g->top = NULL;
  g->bottom = NULL;
  g->value = NULL;
  g->capacity = 0;
  return g;
}

// - function -----------------------------------------------------------------
void free_graph(graph_t **graph)
{
  free((*graph)->top);
  free((*graph)->value);
  free((*graph)->bottom);
  free(*graph);
  *graph = NULL;
}

void expand_arr(graph_t *graph)
{
  graph->bottom = realloc(graph->bottom, sizeof(int) * (graph->capacity + 1));
  graph->top = realloc(graph->top, sizeof(int) * (graph->capacity + 1));
  graph->value = realloc(graph->value, sizeof(int) * (graph->capacity + 1));
}

// - function -----------------------------------------------------------------
void load_txt(const char *fname, graph_t *graph)
{
  char c;
  FILE *f = fopen(fname, "r");
  if (f != NULL)
  {
    int info[3], i = 0;
    int digit, number = 0;

    while ((c = fgetc(f)) == 1 || c != EOF)
    {
      if (isdigit(c))
      {
        digit = c - '0';
        number = number * 10 + digit;
      }
      if (c == ' ' || c == '\n')
      {
        info[i] = number;
        number = 0;
        if (c == '\n')
        {
          i = 0;
          expand_arr(graph);
          graph->bottom[graph->capacity] = info[0];
          graph->top[graph->capacity] = info[1];
          graph->value[graph->capacity] = info[2];
          graph->capacity++;
          continue;
        }
        i++;
      }
    }
  }
  fclose(f);
}

// - function -----------------------------------------------------------------
void load_bin(const char *fname, graph_t *graph)
{
  FILE *f = fopen(fname, "rb");
  int info[3];
  if (f != NULL)
  {
    while (!feof(f))
    {
      expand_arr(graph);
      if (fread(&info, sizeof(int), 3, f))
      {
        graph->bottom[graph->capacity] = info[0];
        graph->value[graph->capacity] = info[2];
        graph->top[graph->capacity] = info[1];
        graph->capacity++;
      }
    }
  }
  fclose(f);
}

// - function -----------------------------------------------------------------
void save_txt(const graph_t *const graph, const char *fname)
{
  FILE *f = fopen(fname, "w");
  if (f != NULL)
  {
    for (int i = 0; i < graph->capacity; i++)
    {
      fprintf(f, "%d %d %d\n", graph->bottom[i], graph->top[i], graph->value[i]);
    }
  }
  fclose(f);
}

// - function -----------------------------------------------------------------
void save_bin(const graph_t *const graph, const char *fname)
{
  FILE *f = fopen(fname, "wb");
  int info[3];
  if (f != NULL)
  {
    for (int i = 0; i < graph->capacity; i++)
    {
      info[0] = graph->bottom[i];
      info[2] = graph->value[i];
      info[1] = graph->top[i];
      fwrite(&info, sizeof(int), 3, f);
    }
  }
  fclose(f);
}
