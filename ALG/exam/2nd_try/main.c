#include <stdio.h>
#include <stdlib.h>

typedef struct Map
{
    int stations_count;
    int *occup_rows;
    int *occup_rows_w;
    int curr_w;
} Map_t;

Map_t *initMap();
Map_t *copyMap(Map_t *map);
void solution(int **matrix_sum, Map_t *map, int sum);
int **create_matrix_sum(int **matrix);
void put_station(Map_t *map, int i, int **matrix_sum);
void remove_station(Map_t *map, int i, int **matrix_sum);
int row_is_occup(Map_t *map, int i);

int H, W, result;

int main()
{
    scanf("%d %d", &H, &W);
    result = -1;

    // выделяем память под матрицу
    int **matrix_base = calloc(H, sizeof(int *));
    for (int i = 0; i < H; i++)
    {
        matrix_base[i] = calloc(W, sizeof(int));
    }

    // читаем матрицу
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            scanf("%d", &matrix_base[i][j]);
        }
    }

    int **matrix_sum = create_matrix_sum(matrix_base);

    Map_t *map = initMap();

    solution(matrix_sum, map, 0);

    printf("%d\n", result);

    return 0;
}

Map_t *initMap()
{
    Map_t *map = malloc(sizeof(Map_t));
    map->stations_count = 0;
    map->occup_rows = calloc(H, sizeof(int));
    map->occup_rows_w = calloc(H, sizeof(int));

    for (int i = 0; i < H; i++)
    {
        map->occup_rows[i] = 0;
        map->occup_rows_w[i] = 0;
    }

    map->curr_w = W;

    return map;
}

Map_t *copyMap(Map_t *map)
{
    Map_t *new_map = initMap();
    new_map->stations_count = map->stations_count;
    new_map->curr_w = map->curr_w;

    for (int i = 0; i < H; i++)
    {
        new_map->occup_rows[i] = map->occup_rows[i];
        new_map->occup_rows_w[i] = map->occup_rows_w[i];
    }

    return new_map;
}

void solution(int **matrix_sum, Map_t *map, int sum)
{
    if (map->stations_count == (H / 2) || map->curr_w == 0)
    {
        if (result == -1 || result < sum)
        {
            result = sum;
        }

        // printf("result: %d: ", sum);
        // for (int i = 0; i < H; i++)
        // {
        //     int a = map->occup_rows[i];
        //     int b = map->occup_rows_w[i];
        //     if (a != 0 && b != 0)
        //     {
        //         printf("[r:%d w:%d] ", i + 1, map->occup_rows_w[i]);
        //     }
        // }
        // printf("\n\n");
        return;
    }

    for (int i = 0; i < H; i++)
    {
        // пропускать строки, которые уже заняты
        if (row_is_occup(map, i))
        {
            //printf("пропускаю строку: %d\n", i + 1);
            continue;
        }

        // забираем область согласно нынешней длине
        // создаем станцию на этом месте
        put_station(map, i, matrix_sum);

        // скопировать карту
        Map_t *new_map = copyMap(map);

        //создать локальную сумму
        int local_sum = sum + matrix_sum[i][map->curr_w];
        //printf("посылаю решение с суммой: %d [r:%d w:%d]\n", local_sum, i + 1, map->curr_w + 1);

        // запустить решение с новой картой
        solution(matrix_sum, new_map, local_sum);

        // удалить станцию на исходной карте для дальнейших решений
        remove_station(map, i, matrix_sum);
    }
}

int row_is_occup(Map_t *map, int i)
{
    if (map->occup_rows[i] == 1) // строка сама уже занята
        return 1;

    if (i > 0) // рядом есть занятые строки
        if (map->occup_rows[i - 1] == 1)
            return 1;

    if (i < H - 1)
        if (map->occup_rows[i + 1] == 1)
            return 1;

    return 0;
}

void put_station(Map_t *map, int i, int **matrix_sum)
{
    map->occup_rows_w[i] = map->curr_w;
    map->occup_rows[i] = 1;

    //printf("занимаю строку: %d, шир.: %d, сумм.: %d\n", i + 1, map->curr_w, map->sum);

    map->curr_w--;
    map->stations_count++;

    // printf("занятые строки: ");
    // for (int i = 0; i < H; i++)
    // {
    //     printf("%d ", map->occup_rows[i]);
    // }
    // printf("\n");
}

void remove_station(Map_t *map, int i, int **matrix_sum)
{
    map->occup_rows_w[i] = 0;
    map->occup_rows[i] = 0;
    map->stations_count--;
    map->curr_w++;
}

int **create_matrix_sum(int **matrix)
{
    int **matrix_sum = calloc(H, sizeof(int *));
    for (int i = 0; i < H; i++)
    {
        matrix_sum[i] = calloc(W, sizeof(int));
    }

    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            if (j == 0)
            {
                matrix_sum[i][j] = matrix[i][j];
            }
            else
            {
                matrix_sum[i][j] = matrix[i][j] + matrix_sum[i][j - 1];
            }
        }
    }

    return matrix_sum;
}
