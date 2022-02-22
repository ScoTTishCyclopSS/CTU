#include <stdio.h>
#include <stdlib.h>

#define INIT_SIZE 16

typedef struct Level
{
    int *eggs;
} Level_t;

int curr_lvl_size, width, max_lvl, final_eggs;

int eggsCollected(Level_t **levels, int x_right, int y);
void reachNextEgg(Level_t **levels, int x_right, int level, int eggs_collected);
void print_game(Level_t **levels, int max_lvl);
void freeAll(Level_t **levels, int max_lvl);

int main()
{
    curr_lvl_size += INIT_SIZE;
    max_lvl = 0;

    Level_t **levels = calloc(curr_lvl_size, sizeof(Level_t *));

    int eggs_count = 0;
    if (scanf("%d %d", &width, &eggs_count) != 2)
    {
        fprintf(stderr, "Data read err!\n");
        return -1;
    }

    for (int i = 0; i < eggs_count; i++)
    {
        int egg, level = 0;
        if (scanf("%d %d", &egg, &level) != 2)
        {
            fprintf(stderr, "Egg read err!\n");
            return -1;
        }

        max_lvl = level > max_lvl ? level : max_lvl;

        // realloc
        if (level > curr_lvl_size)
        {
            //printf("REALLOC!\n");
            while (curr_lvl_size < max_lvl)
            {
                curr_lvl_size += INIT_SIZE;
                levels = realloc(levels, curr_lvl_size * sizeof(Level_t *));
                for (int r = curr_lvl_size - INIT_SIZE; r < curr_lvl_size; r++)
                {
                    levels[r] = NULL;
                }
            }
        }

        if (!levels[level])
        {
            //printf("No such lvl!\nlvl:%d lvl_max:%d curr_lvl_size:%d\n", level, max_lvl, curr_lvl_size);
            levels[level] = malloc(sizeof(Level_t));
            levels[level]->eggs = calloc(width + 1, sizeof(int));
        }

        // add new egg
        levels[level]->eggs[egg] = egg;
    }

    //print_game(levels, max_lvl);

    reachNextEgg(levels, 2, 0, 0);
    printf("final: %d\n", final_eggs);

    freeAll(levels, max_lvl);

    return 0;
}

void freeAll(Level_t **levels, int max_lvl)
{
    for (int i = 0; i <= max_lvl; i++)
    {
        if (levels[i])
        {
            free(levels[i]->eggs);
            free(levels[i]);
        }
    }
    free(levels);
}

void print_game(Level_t **levels, int max_lvl)
{
    for (int i = 0; i <= max_lvl; i++)
    {
        if (levels[i])
        {
            printf("Level %d:", i);
            for (int j = 0; j <= width; j++)
            {
                if (levels[i]->eggs[j])
                {
                    printf(" %d", levels[i]->eggs[j]);
                }
            }
            printf("\n");
        }
    }
    printf("\n");
}

void reachNextEgg(Level_t **levels, int x_right, int level, int eggs_collected) // 2 0 0
{
    if (x_right > width || x_right <= 1)
    {
        return;
    }
    else if (level > max_lvl)
    {
        printf("task completed, eggs collected: %d\n", eggs_collected);
        if (eggs_collected > final_eggs)
        {
            final_eggs = eggs_collected;
        }
        return;
    }

    eggs_collected += eggsCollected(levels, x_right, level);      // curr
    reachNextEgg(levels, x_right, level + 1, eggs_collected);     // top
    reachNextEgg(levels, x_right - 1, level + 1, eggs_collected); // left
    reachNextEgg(levels, x_right + 1, level + 1, eggs_collected); // right
}

int eggsCollected(Level_t **levels, int x_right, int level)
{
    int eggs = 0;

    if (levels[level]) // if the level has eggs
    {
        // left kart pixel
        eggs += levels[level]->eggs[x_right - 1] > 0 ? 1 : 0;
        // right kart pixel
        eggs += levels[level]->eggs[x_right] > 0 ? 1 : 0;
    }
    return eggs;
}
