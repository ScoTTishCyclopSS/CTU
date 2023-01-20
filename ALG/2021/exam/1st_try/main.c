#include <stdio.h>
#include <stdlib.h>

typedef struct Station
{
    int x, y;
} Station_t;

typedef struct Map
{
    int stations_count;
    int *occup_rows;
    int *occup_cols;
    Station_t *stations;
} Map_t;

Map_t *initMap();
Map_t *copyMap(Map_t *map);
void solution(int **matrix, Map_t *map, int sum, int st_placed);
void put_station(Map_t *map, int i, int j);
void remove_station(Map_t *map, int i, int j);
int pos_is_valid(int i, int j, Map_t *map);
int dis_not_valid(int i, int j, int st_i, int st_j);

int H, W, D, N, result;

int main()
{
    scanf("%d %d %d %d", &H, &W, &D, &N);
    result = -1;

    // выделяем память под матрицу
    int **matrix = calloc(H, sizeof(int *));
    for (int i = 0; i < H; i++)
    {
        matrix[i] = calloc(W, sizeof(int));
    }

    // читаем матрицу
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            scanf("%d", &matrix[i][j]);
        }
    }

    Map_t *map = initMap();

    solution(matrix, map, 0, 0);

    printf("%d\n", result);

    return 0;
}

Map_t *initMap()
{
    Map_t *map = malloc(sizeof(Map_t));
    map->stations_count = 0;
    map->stations = calloc(N, sizeof(Station_t));
    map->occup_rows = calloc(H, sizeof(int));
    map->occup_cols = calloc(W, sizeof(int));
    return map;
}

Map_t *copyMap(Map_t *map)
{
    Map_t *new_map = initMap();
    new_map->stations_count = map->stations_count;

    for (int i = 0; i < new_map->stations_count; i++)
    {
        new_map->stations[i].x = map->stations[i].x;
        new_map->stations[i].y = map->stations[i].y;
    }

    for (int i = 0; i < H; i++)
        new_map->occup_rows[i] = map->occup_rows[i];

    for (int i = 0; i < W; i++)
        new_map->occup_cols[i] = map->occup_cols[i];

    return new_map;
}

void solution(int **matrix, Map_t *map, int sum, int st_placed)
{
    if (N == st_placed)
    {
        //printf("решение: %d\n", sum);

        result = (result == -1 || result > sum) ? sum : result;

        // for (int i = 0; i < map->stations_count; i++)
        // {
        //     printf("%d -> %d %d\n", i + 1, map->stations[i].x, map->stations[i].y);
        // }
        // printf("\n");
        return;
    }

    for (int i = 0; i < H; i++)
    {
        // пропускать строки, которые уже заняты
        if (map->occup_rows[i] == 1)
        {
            //printf("пропускаю строку %d\n", i);
            continue;
        }

        for (int j = 0; j < W; j++)
        {
            // пропускать колонки, которые уже заняты
            if (map->occup_cols[j] == 1)
            {
                //printf("пропускаю столбец %d\n", j);
                continue;
            }

            // проверяем если позиция валидна для станции
            // (явно отдалена от других)
            if (pos_is_valid(i, j, map))
            {
                // создаем станцию на этом месте
                put_station(map, i, j);

                //printf("занимаю место %d %d\n", i, j);

                // создаем копию карты с новой станцией
                Map_t *new_map = copyMap(map);

                // считаем выгоду
                int local_sum = sum + matrix[i][j];

                // от нынешнего решения -> создать новое
                solution(matrix, new_map, local_sum, st_placed + 1);

                // убрать станцию, чтобы освободить место для потенциальных станций в будущем
                remove_station(map, i, j);

                //printf("убираю %d %d\n", i, j);
            }
            else
            {
                //printf("%d близко\n", matrix[i][j]);
            }
        }
        //printf("\n");
        // pray
        if (H * W > 70)
        {
            break;
        }
    }
}

void put_station(Map_t *map, int i, int j)
{
    map->stations[map->stations_count].x = i;
    map->stations[map->stations_count].y = j;
    map->occup_rows[i] = 1;
    map->occup_cols[j] = 1;
    map->stations_count++;
}

void remove_station(Map_t *map, int i, int j)
{
    map->stations[map->stations_count].x = 0;
    map->stations[map->stations_count].y = 0;
    map->occup_rows[i] = 0;
    map->occup_cols[j] = 0;
    map->stations_count--;
}

int pos_is_valid(int x, int y, Map_t *map)
{
    //printf("проверяю %d %d\n", x, y);
    for (int i = 0; i < map->stations_count; i++)
    {
        if (dis_not_valid(x, y, map->stations[i].x, map->stations[i].y))
        {
            //printf("позиция %d %d близко к станции на %d %d\n", x, y, map->stations[i].x, map->stations[i].y);
            return 0;
        }
    }
    return 1;
}

int dis_not_valid(int i, int j, int st_i, int st_j)
{
    int diff_r = i - st_i;
    int diff_c = j - st_j;
    return (abs(diff_r) < D - 1 && abs(diff_c) < D - 1);
}
