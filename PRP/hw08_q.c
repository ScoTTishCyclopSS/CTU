#include "queue.h"

void *get_from_queue(queue_t *queue, int idx) // ok
{
  void *element = NULL;
  if (idx > queue->size - 1 || idx < 0 || queue->size <= 0)
  {
    element = NULL;
  }
  else
  {
    if (queue->frnt + idx > queue->cap)
    {
      idx -= queue->cap + 1;
    }
    element = queue->arr[queue->frnt + idx];
  }
  return element;
}

void *pop_from_queue(queue_t *queue)
{
  void *element = NULL;
  if (queue == NULL || queue->size == 0)
  {
    return element;
  }
  element = queue->arr[queue->frnt];
  queue->size--;
  if (queue->frnt == queue->cap)
  {
    queue->frnt = 0;
  }
  else
  {
    queue->frnt++;
  }
  return element;
}

bool push_to_queue(queue_t *queue, void *data) //вроде ок
{
  if (queue->size == queue->cap)
  {
    return false;
  }
  queue->arr[queue->rear] = data;
  queue->size++;
  if (queue->rear == queue->cap)
  {
    queue->rear = 0;
  }
  else
  {
    queue->rear++;
  }
  return true;
}

void delete_queue(queue_t *queue) // ok
{
  if (queue->arr != NULL)
  {
    free(queue->arr);
  }
  if (queue != NULL)
  {
    free(queue);
  }
}

int get_queue_size(queue_t *queue) // ok
{
  return queue->size;
}

queue_t *create_queue(int capacity) // ok
{
  queue_t *q = malloc(sizeof(queue_t));
  q->rear = 0;
  q->frnt = 0;
  q->cap = capacity;
  q->arr = malloc(sizeof(void *) * (capacity + 1));
  q->size = 0;
  return q;
}