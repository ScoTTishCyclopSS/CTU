#include <stdio.h>
#include <stdbool.h>

#define MIN_VALUE 3
#define MAX_VALUE 69

void roof(int);
void house(int, int, int);
void fence(int, int);
void simple_house(int, int);
int bad_check(int, int);

int main(int argc, char const *argv[])
{
    int r, width, height, fence_size;
    if ((r = scanf("%d %d", &width, &height) == 2))
    {
        if ((r = scanf("%d", &fence_size) == 1))
        {
            if (bad_check(width, 0) == false && bad_check(height, 0) == false)
            {
                if (bad_check(width, 1) == false)
                {
                    if (width == height)
                    {
                        if (fence_size < height)
                        {
                            if (bad_check(fence_size, 2) == false)
                            {
                                roof(width);
                                house(width, height, fence_size);
                                return 0;
                            }
                        }
                        else
                        {
                            fprintf(stderr, "Error: Neplatna velikost plotu!\n");
                            return 103;
                        }
                    }
                    else
                    {
                        roof(width);
                        simple_house(width, height);
                        return 0;
                    }
                }
            }
        }
        else
        {
            if ((r = scanf("%d", &fence_size) == 0))
            {
                fprintf(stderr, "Error: Chybny vstup!\n");
                return 100;
            }
            else
            {
                if (bad_check(width, 0) == false && bad_check(height, 0) == false)
                {
                    if (bad_check(width, 1) == false)
                    {
                        printf("simple\n");
                        roof(width);
                        simple_house(width, height);
                        return 0;
                    }
                }
            }
        }
    }
    else
    {
        fprintf(stderr, "Error: Chybny vstup!\n");
        return 100;
    }
}

int bad_check(int element, int case_switch)
{
    switch (case_switch)
    {
    case 0: //for width and height
        if (element < MIN_VALUE || element > MAX_VALUE)
        {
            fprintf(stderr, "Error: Vstup mimo interval!\n");
            return 101;
        }
        break;

    case 1: //for width
        if (element % 2 == 0)
        {
            fprintf(stderr, "Error: Sirka neni liche cislo!\n");
            return 102;
        }
        break;

    case 2:
        if (element <= 0) //for fence size
        {
            fprintf(stderr, "Error: Neplatna velikost plotu!\n");
            return 103;
        }
        break;

    default:
        break;
    }
    return 0;
}

void roof(int width)
{
    int center = width / 2;
    int counter = center;
    int mid_sp = 1;

    for (int i = 0; i < center; i++)
    {
        if (counter == center)
        {
            for (int i = 0; i < center; i++)
            {
                printf(" ");
            }
            printf("X");
            counter--;
        }
        else
        {
            for (int i = 0; i < counter; i++)
            {
                printf(" ");
            }
            counter--;
            printf("X");
            for (int i = 0; i < mid_sp; i++)
            {
                printf(" ");
            }
            mid_sp += 2;
            printf("X");
        }
        printf("\n");
    }
}

void simple_house(int width, int height)
{
    for (int i = 0; i < width; i++)
    {
        printf("X");
    }
    printf("\n");
    for (int i = 0; i < height - 2; i++)
    {
        printf("X");
        for (int i = 0; i < width - 2; i++)
        {
            printf(" ");
        }
        printf("X\n");
    }
    for (int i = 0; i < width; i++)
    {
        printf("X");
    }
    printf("\n");
}

void house(int width, int height, int fence_size)
{
    int mid_sp = (width - 2) / 2;
    int i;
    for (int i = 0; i < width; i++)
    {
        printf("X");
    }
    printf("\n");
    for (i = 0; i < height - 2; i++)
    {
        printf("X");
        for (int j = 0; j <= mid_sp; j++)
        {
            if (i % 2 == 0)
            {
                printf("o*");
            }
            else
            {
                printf("*o");
            }
        }
        printf("\bX");
        if (i == height - fence_size - 1)
        {
            fence(fence_size, 0);
        }
        else if (i >= height - fence_size)
        {
            fence(fence_size, 1);
        }
        printf("\n");
    }
    for (int i = 0; i < width; i++)
    {
        printf("X");
    }
    fence(fence_size, 0);
    printf("\n");
}

void fence(int fence_size, int case_build)
{
    switch (case_build)
    {
    case 0:
        if (fence_size % 2 != 0)
        {
            printf("|");
        }
        for (int i = 0; i < fence_size / 2; i++)
        {
            printf("-|");
        }
        break;

    case 1:
        if (fence_size % 2 != 0)
        {
            printf("|");
        }
        for (int i = 0; i < fence_size / 2; i++)
        {
            printf(" |");
        }
        break;

    default:
        break;
    }
}