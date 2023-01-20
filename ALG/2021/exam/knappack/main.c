#include <stdio.h>
#include <stdlib.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

int main()
{
    int space, n; // место в рюкзаке, кол-во предметов

    scanf("%d %d", &space, &n);

    int **matrix = calloc(n + 1, sizeof(int *));
    for (int i = 0; i < space; i++)
    {
        matrix[i] = calloc(space + 1, sizeof(int));
    }

    for (int i = 1; i <= n; i++) // для каждого предмета
    {
        int price, weight;
        scanf("%d %d", &price, &weight); // считывание предмета

        for (int j = 1; j <= space; j++) // от минимально возможного размера рюкзака до максимально возможного
        {
            int combined_price = (j - weight) < 0 ? 0 : matrix[i - 1][j - weight] + price; // брать вещь (=> учесть предыдущую с вычетом веса нынешней)
            int prev_price = matrix[i - 1][j]; // не брать вещь
            matrix[i][j] = MAX(combined_price, prev_price);
        }
    }

    for (int i = 0; i < n + 1; i++)
    {
        for (int j = 0; j < space + 1; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}