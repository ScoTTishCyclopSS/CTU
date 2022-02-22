/*
 * author: @timusfed
 */

#include <stdio.h>
#include <stdlib.h>

void chain(int N, int L, int P, int **v, int **p, int opt);

int main()
{
    int N, P, L; // n - villages, p - price for totem, l - price for warrior

    int ret = scanf("%d %d %d", &N, &P, &L);
    if (ret != 3)
        return 1;

    int **v = (int **)malloc(N * sizeof(int *));
    int **p = (int **)malloc(N * sizeof(int *));

    for (int i = 0; i < N; i++)
    {
        v[i] = (int *)malloc(N * sizeof(int)); // village info
        p[i] = (int *)malloc(N * sizeof(int)); // profits
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (i == j)
            {
                p[i][j] = 0;
                ret = scanf("%d", &v[i][j]); // save warriors
                if (ret != 1)
                    return 1;
            }
            else
            {
                p[i][j] = -__INT_MAX__;
                v[i][j] = 0;
            }
        }
    }

    chain(N, L, P, v, p, 0);
    printf("%d\n", p[0][N - 1]);
}

void chain(int N, int L, int P, int **v, int **p, int opt)
{
    for (int l = 1; l < N; l++)
    {
        for (int i = 0; i < N - l; i++)
        {
            int j = i + l;

            p[i][j] = opt ? p[i][j] : -__INT_MAX__;

            for (int k = i; k < j; k++)
            {
                if (v[i][k] > 0 && v[k + 1][j] > 0) // if there are even warriors at least in one village
                {
                    int profit = opt ? (p[i][k] + p[k + 1][j]) : (P + p[i][k] + p[k + 1][j] - L * abs(v[i][k] - v[k + 1][j]));
                    if (profit > p[i][j])
                    {
                        p[i][j] = profit;
                        v[i][j] = v[i][k] + v[k + 1][j]; // save new warriors
                    }
                }
            }
        }
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d ", p[i][j]);
        }
        printf("\n");
    }

    if (!opt)
        chain(N, L, P, v, p, 1);
}