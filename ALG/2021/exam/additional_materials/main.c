#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define IMPLIES(x, y) (!(x) || (y))

#define COMPARE(x, y) (((x) > (y)) - ((x) < (y)))
#define SIGN(x) COMPARE(x, 0)

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(*a))

#define SWAP(x, y, T) \
   do                 \
   {                  \
      T tmp = (x);    \
      (x) = (y);      \
      (y) = tmp;      \
   } while (0)

#define SORT2(a, b, T)      \
   do                       \
   {                        \
      if ((a) > (b))        \
         SWAP((a), (b), T); \
   } while (0)

#define SET(d, n, v)                             \
   do                                            \
   {                                             \
      size_t i_, n_;                             \
      for (n_ = (n), i_ = 0; n_ > 0; --n_, ++i_) \
         (d)[i_] = (v);                          \
   } while (0)

#define ZERO(d, n) SET(d, n, 0)

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

int values[] = {88, 56, 100, 2, 25};

int cmpfunc(const void *a, const void *b)
{
   return (*(int *)a - *(int *)b);
}

int main()
{
   int n;

   printf("Before sorting the list is: \n");
   for (n = 0; n < 5; n++)
   {
      printf("%d ", values[n]);
   }

   qsort(values, 5, sizeof(int), cmpfunc);

   printf("\nAfter sorting the list is: \n");
   for (n = 0; n < 5; n++)
   {
      printf("%d ", values[n]);
   }
   printf("\n");

   return (0);
}
