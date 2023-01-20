/*
 * autor: @timusfed
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct Grid
{
    int rows;
    int cols;
    int *input_mat;
} BuildGrid;

void defineGrid(BuildGrid *buildGrid);
void printMat(BuildGrid *buildGrid);

int main(int argc, char *argv[])
{
    BuildGrid buildGrid;
    defineGrid(&buildGrid);

    // read matrix info
    scanf("%d %d", &buildGrid.rows, &buildGrid.cols);
    buildGrid.input_mat = (int *)malloc(sizeof(int) * buildGrid.rows * buildGrid.cols);
    int* new_mat = (int *)malloc(sizeof(int) * buildGrid.rows * buildGrid.cols);

    int k = 0;
    int num;
    while (scanf("%d", &num) == 1 && k != buildGrid.rows * buildGrid.cols)
    {
        buildGrid.input_mat[k++] = num;
    }
    printMat(&buildGrid);
    printf("\n");

    for (int i = 0; i < buildGrid.rows; i++)
    {
        for (int j = i * buildGrid.cols; j < i * buildGrid.cols + buildGrid.cols; j++)
        {
            if (j > i * buildGrid.cols && i > 0)
            {
                new_mat[j] = buildGrid.input_mat[j] + new_mat[j - 1] + new_mat[j - buildGrid.cols] - new_mat[j - buildGrid.cols - 1];
            }
            else if (i > 0)
            {
                new_mat[j] = buildGrid.input_mat[j] + buildGrid.input_mat[j - buildGrid.rows];
            }
            else if (j > i * buildGrid.cols)
            {
                new_mat[j] = buildGrid.input_mat[j] + new_mat[j - 1];
            }
            else
            {
                new_mat[j] = buildGrid.input_mat[j];
            }
            printf("%d ", new_mat[j]);
        }
        printf("\n");
    }
    free(buildGrid.input_mat);
    return 0;
}

void defineGrid(BuildGrid *buildGrid)
{
    buildGrid->rows = 0;
    buildGrid->cols = 0;
}

void printMat(BuildGrid *buildGrid)
{
    for (int i = 0; i < buildGrid->rows; i++)
    {
        for (int j = i * buildGrid->cols; j < i * buildGrid->cols + buildGrid->cols; j++)
        {
            printf("%d ", buildGrid->input_mat[j]);
        }
        printf("\n");
    }
}
