/*
 * autor: @timusfed
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// define errors
#define MAT_READ_ERR -1
#define MAT_READ_ERR_MSG "Error while reading input matrix data!\n"

// define usefull vars
#define PERFECT_SQ 0
#define OK_SQ 1
#define WORST_SQ 2

#define MAX_COLS 2000
#define MIN_ROWS 2 //???

#define MAX_OK_SUM 1
#define MIN_EAST_WEST 6

typedef struct Grid
{
    int rows;
    int cols;
    int *input_mat;
    int area;
    int sum;
    int max_area;
} BuildGrid;

void defineGrid(BuildGrid *buildGrid);
void findMid(BuildGrid *buildGrid);
bool buildEastWest(BuildGrid *buildGrid, int eastId, int westId);

int main(int argc, char *argv[])
{

    BuildGrid buildGrid;
    defineGrid(&buildGrid);

    // read matrix info
    if (scanf("%d %d", &buildGrid.rows, &buildGrid.cols) != 2)
    {
        fprintf(stderr, MAT_READ_ERR_MSG);
        return MAT_READ_ERR;
    }

    //2 ≤ M, N ≤ 2000.
    if (buildGrid.rows < MIN_ROWS || buildGrid.cols > MAX_COLS)
    {
        fprintf(stderr, MAT_READ_ERR_MSG);
        return MAT_READ_ERR;
    }

    // read mat itself
    buildGrid.input_mat = (int *)malloc(sizeof(int) * buildGrid.rows * buildGrid.cols);

    // fill the mat
    int k = 0;
    char symbol;
    while (scanf("%c", &symbol) == 1)
    {
        if (symbol == '.')
        {
            buildGrid.input_mat[k++] = PERFECT_SQ;
        }
        if (symbol == 'o')
        {
            buildGrid.input_mat[k++] = OK_SQ;
        }
        if (symbol == 'X')
        {
            buildGrid.input_mat[k++] = WORST_SQ;
        }
    }
    findMid(&buildGrid);
    printf("%d\n", buildGrid.max_area);
    free(buildGrid.input_mat);
    return 0;
}

void findMid(BuildGrid *buildGrid)
{
    int start = buildGrid->cols + 1;
    int end = (buildGrid->rows * (buildGrid->cols - 1)) - 1;

    for (int i = start; i < end; i++)
    {
        // skip if it's 1st or last element in a row
        if (i % buildGrid->cols == 0 || (i + 1) % buildGrid->cols == 0)
        {
            continue;
        }
        // if the closest area > MAX_OK_SUM then no reason to build what so ever
        if (buildGrid->input_mat[i - 1] + buildGrid->input_mat[i] > MAX_OK_SUM)
        {
            continue;
        }
        // add cost to sum & count area
        buildGrid->sum += buildGrid->input_mat[i - 1] + buildGrid->input_mat[i];
        buildGrid->area += 2;
        // save values
        int old_area = buildGrid->area;

        //printf("\n%d index\n", i);
        for (int shift = i + 1; shift % buildGrid->cols != 0; shift++)
        {
            // if sum > after adding new square then leave
            if (buildGrid->sum + buildGrid->input_mat[shift] > MAX_OK_SUM)
            {
                break;
            }
            // add cost to sum & count area
            buildGrid->sum += buildGrid->input_mat[shift];
            buildGrid->area++;
            old_area = buildGrid->area;
            // building west & east
            //printf("With shift to %d index\n", shift);
            if (buildEastWest(buildGrid, i - 1, shift)) //
            {
                //printf("SuckASS: sum %d, area %d\n\n", buildGrid->sum, buildGrid->area);
                if (buildGrid->area > buildGrid->max_area)
                {
                    buildGrid->max_area = buildGrid->area;
                }
            }
            else
            {
                //printf("Unluck: sum %d, area %d\n\n", buildGrid->sum, buildGrid->area);
            }
            buildGrid->area = old_area;
        }
        // reset values for the nex cycle
        buildGrid->sum = 0;
        buildGrid->area = 0;
    }
}

bool buildEastWest(BuildGrid *buildGrid, int eastId, int westId)
{
    // ======================== Straight to the top ==========================

    int count_top = 0;
    int ok_top = 0;
    int local_sum = buildGrid->sum;
    bool top_has_ok = false;
    for (int i = eastId - buildGrid->cols, j = westId - buildGrid->cols; // left & right blocks
         i >= 0;                                                         // until the whole 1st row
         i -= buildGrid->cols, j -= buildGrid->cols, count_top++)        // row by row
    {
        if (local_sum + buildGrid->input_mat[i] + buildGrid->input_mat[j] > MAX_OK_SUM)
        {
            break;
        }
        if (buildGrid->input_mat[i] + buildGrid->input_mat[j] == MAX_OK_SUM)
        {
            ok_top = count_top;
            top_has_ok = true;
        }
        local_sum += (buildGrid->input_mat[i] + buildGrid->input_mat[j]);
    }

    // printf("->%d - local_sum\n", local_sum);
    //printf("count_top = %d, ok_top = %d\n", count_top, ok_top);

    if (count_top == 0)
    {
        return false;
    }

    // ======================== Straight to the bottom ==========================

    int count_bottom = 0;
    int ok_bottom = 0;
    local_sum = buildGrid->sum;
    bool bottom_has_ok = false;
    for (int i = eastId + buildGrid->cols, j = westId + buildGrid->cols; // left & right blocks
         i < buildGrid->rows * buildGrid->cols;                          // until the last row
         i += buildGrid->cols, j += buildGrid->cols, count_bottom++)     // row by row
    {
        if (local_sum + buildGrid->input_mat[i] + buildGrid->input_mat[j] > MAX_OK_SUM)
        {
            break;
        }
        if (buildGrid->input_mat[i] + buildGrid->input_mat[j] == MAX_OK_SUM)
        {
            ok_bottom = count_bottom;
            bottom_has_ok = true;
        }
        local_sum += (buildGrid->input_mat[i] + buildGrid->input_mat[j]);
    }

    // printf("->%d - local_sum\n", local_sum);
    //printf("count_bottom = %d, ok_bottom = %d\n", count_bottom, ok_bottom);

    if (count_bottom == 0)
    {
        return false;
    }

    // ======================== Resolving issue: both have the the OK square ========================

    if (top_has_ok && bottom_has_ok)
    {
        // both first levels have ok_sq? no need to build then
        if (ok_top == 0 && ok_bottom == 0)
        {
            return false;
        }
        //printf("both have ok\n");
        if (ok_top == 0)
        {
            buildGrid->area += count_top * 2 + ok_bottom * 2;
        }
        else if (ok_bottom == 0)
        {
            buildGrid->area += count_bottom * 2 + ok_top * 2;
        }
        else if (count_top >= count_bottom)
        {
            ok_bottom = ok_bottom == 0 ? 1 : ok_bottom;
            buildGrid->area += count_top * 2 + ok_bottom * 2;
        }
        else if (count_bottom > count_top)
        {
            ok_top = ok_top == 0 ? 1 : ok_top;
            buildGrid->area += count_bottom * 2 + ok_top * 2;
        }
    }
    else
    {
        buildGrid->area += count_top * 2 + count_bottom * 2;
    }

    return true;
}

void defineGrid(BuildGrid *buildGrid)
{
    buildGrid->rows = 0;
    buildGrid->cols = 0;
    buildGrid->sum = 0;
    buildGrid->area = 0;
    buildGrid->max_area = 0;
}
