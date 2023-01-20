#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int neighbors[3];
    int red_after_neighbor[3];
    int neighbors_count;
    int red;
} Node_t;

void readNode(Node_t *nodes, int id, int isRed, int next);
int count_red_after_node(Node_t *nodes, int id, int prev_id);
void solution(Node_t *nodes, int N, int R);

int main(int argc, char *argv[])
{
    int N = 0, R = 0;
    scanf("%d %d", &N, &R);

    Node_t *nodes = calloc(N, sizeof(Node_t));

    // прочитать весь граф, записать ноды в двухстороннем порядке

    int val, next;
    while (scanf("%d %d", &val, &next) == 2)
    {
        int is_red = val <= R ? 1 : 0;
        readNode(nodes, val - 1, is_red, next - 1);

        is_red = next <= R ? 1 : 0;
        readNode(nodes, next - 1, is_red, val - 1);
    }

    // логика - рекурсивно посчитать у каждого нода кол-во красных нодов после КАЖДОГО его соседа
    count_red_after_node(nodes, 0, -1);

    // логика - стартовать от каждого нода в графе, для каждого соседа проверить разницу в красных нодах после разрезания
    solution(nodes, N, R);

    return 0;
}

void readNode(Node_t *nodes, int id, int isRed, int next)
{
    int count = nodes[id].neighbors_count;
    nodes[id].red = isRed;
    nodes[id].neighbors[count] = next;
    nodes[id].neighbors_count++;
}

int count_red_after_node(Node_t *nodes, int id, int prev_id)
{
    int red_count = 0;
    // для каждого соседа нода
    for (int i = 0; i < nodes[id].neighbors_count; i++)
    {
        int neighbor_id = nodes[id].neighbors[i];

        // ациклично!!
        if (neighbor_id != prev_id)
        {
            // рекурсия на поиск красных нодов после соседа
            nodes[id].red_after_neighbor[i] = count_red_after_node(nodes, neighbor_id, id);
            red_count += nodes[id].red_after_neighbor[i];
            //printf("%d: сосед %d [%d] красных %d\n", id + 1, neighbor_id + 1, nodes[neighbor_id].red, red_count);
        }
    }
    return (red_count + nodes[id].red); // нод, из которого мы вышли тоже может быть красным
}

void solution(Node_t *nodes, int N, int R)
{
    int v1_fin, v2_fin, diff_fin = R;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < nodes[i].neighbors_count; j++)
        {
            int red_count_after = nodes[i].red_after_neighbor[j];
            int red_count_before = R - red_count_after;
            int diff = abs(red_count_after - red_count_before); // находим разницу при "переризании" нода с соседом

            //printf("перерезаю %d %d, diff %d\n", i + 1, nodes[i].neighbors[j] + 1, diff);

            // вершины необходимо вывести в топологическом порядке
            int v1, v2, flag;
            flag = i < nodes[i].neighbors[j]; // проверка, если вершина раньше соседа

            v1 = flag ? (i + 1) : nodes[i].neighbors[j] + 1;
            v2 = flag ? nodes[i].neighbors[j] + 1 : (i + 1);

            // проверка если разница лучше (приближ. к "поровну")
            if (diff < diff_fin)
            {
                // обновляем результат
                diff_fin = diff;
                v1_fin = v1;
                v2_fin = v2;
            }
            // по условию - эквивалентная разница является проверкой топологии
            else if (diff == diff_fin)
            {
                if (v1 < v1_fin || (v1 == v1_fin && v2 < v2_fin))
                {
                    v1_fin = v1;
                    v2_fin = v2;
                }
            }
        }
        //printf("\n");
    }
    printf("%d %d %d\n", v1_fin, v2_fin, diff_fin);
}
