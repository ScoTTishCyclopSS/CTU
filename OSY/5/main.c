/*
 * @author: timusfed
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

#define CRASH_ON_THREAD_CHECK(thread)          \
  {                                            \
    if (thread != 0)                           \
    {                                          \
      fprintf(stderr, "Crashed on thread!\n"); \
      exit(EXIT_FAILURE);                      \
    }                                          \
  }

pthread_mutex_t mutex;
pthread_mutex_t printMutex;
pthread_mutex_t runMutex;
sem_t sem;

typedef struct KonzStruct
{
  int x;
  char *word;
  struct KonzStruct *next;
} KonzStruct_t;

typedef struct List
{
  KonzStruct_t *last;
  KonzStruct_t *root;
  int size;
} List_t;

void *producent_func(void *var);
void *konzument_func(void *var);
int poppop(int id);
void pushpush(int x, char *text);

List_t *list;
bool readErr;

int main(int argc, char *argv[])
{
  pthread_mutex_init(&mutex, NULL);
  pthread_mutex_init(&printMutex, NULL);
  pthread_mutex_init(&runMutex, NULL);
  sem_init(&sem, 0, 0);
  readErr = false;
  int N;

  // ========== Read the number of threads ========== //
  N = argc == 2 ? atoi(argv[1]) : 1;
  if (N > sysconf(_SC_NPROCESSORS_ONLN))
    return EXIT_FAILURE;

  // ========== Init the list ========== //
  list = malloc(sizeof(List_t));
  list->root = NULL;
  list->last = list->root;
  list->size = 0;

  // ========== Init producent ========== //
  pthread_t prod;
  int producent = pthread_create(&prod, NULL, producent_func, NULL);
  CRASH_ON_THREAD_CHECK(producent);

  // ========== Creating the list of additional threads ========== //
  pthread_t *threadsList = malloc(sizeof(pthread_t) * N);
  int *id = malloc(sizeof(int) * N);
  for (int i = 0; i < N; i++)
  {
    id[i] = i + 1;
    int konzument = pthread_create(&threadsList[i], NULL, konzument_func, (void *)&id[i]);
    CRASH_ON_THREAD_CHECK(konzument);
  }

  // ========== Signal the threads to end ========== //
  int prod_join = pthread_join(prod, NULL);
  CRASH_ON_THREAD_CHECK(prod_join);

  for (int i = 0; i < N; i++)
  {
    pushpush(-1, NULL);
    sem_post(&sem);
  }

  // ========== Waiting the threads to end ========== //
  for (int i = 0; i < N; i++)
  {
    int konz_join = pthread_join(threadsList[i], NULL);
    CRASH_ON_THREAD_CHECK(konz_join);
  }

  // ========== Free the memory ========== //
  free(id);
  free(threadsList);
  free(list);
  pthread_mutex_destroy(&mutex);
  pthread_mutex_destroy(&printMutex);
  pthread_mutex_destroy(&runMutex);
  sem_destroy(&sem);

  if (readErr)
  {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void *producent_func(void *var)
{
  int ret, x;
  char *text;
  while ((ret = scanf("%d %ms", &x, &text)) == 2)
  {
    if (x < 0 || text == NULL)
    {
      free(text);
      readErr = true;
      break;
    }
    pushpush(x, text);
    sem_post(&sem);
  }
  if (ret != EOF)
  {
    readErr = true;
  }

  return NULL;
}

void pushpush(int x, char *text)
{
  // save new data to node
  KonzStruct_t *node = malloc(sizeof(KonzStruct_t));
  node->x = x;
  node->word = text;
  node->next = NULL;

  pthread_mutex_lock(&mutex);
  // if the last node exists -> we have where to push data (after the last)
  if (list->last)
  {
    // shouldn't work for the first element
    list->last->next = node;
  }
  else
  {
    list->root = node;
  }
  // so now the last is updated as well
  list->last = node;
  list->size++;
  pthread_mutex_unlock(&mutex);
}

void *konzument_func(void *id_in)
{
  int id = *((int *)id_in);
  while (true)
  {
    sem_wait(&sem);
    if (poppop(id))
      break;
  }
  return NULL;
}

int poppop(int id)
{
  // lock the reading process
  pthread_mutex_lock(&mutex);
  KonzStruct_t *node = list->root;

  // trasfer data to root
  list->size -= 1;

  if (list->size == 0)
  {
    list->root = NULL;
    list->last = NULL;
  } else {
    list->root = node->next;
  }

  pthread_mutex_unlock(&mutex);
  if (node->word == NULL || node->x == -1)
  {
    free(node);
    node = NULL;
    return EXIT_FAILURE;
  }

  pthread_mutex_lock(&printMutex);
  printf("Thread %d:", id);
  for (int i = 0; i < node->x; i++)
  {
    printf(" %s", node->word);
  }
  printf("\n");
  pthread_mutex_unlock(&printMutex);


  // free the memory
  free(node->word);
  node->word = NULL;
  free(node);
  node = NULL;

  return EXIT_SUCCESS;
}
