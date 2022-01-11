#include <stdio.h>
#include <stdbool.h>

#define MIN 3
#define MAX 69

/* Function declaration */
int roof(int);
int house(int, int);
int flood_house(int, int, int);
int fence_build(int, int, int);


/* The main structure. Creates the full house */
int main(int argc, char *argv[])
{
    int width, height, fence, read;

    if ((read = scanf("%d %d", &width, &height) == 2)) {
        if ((read = scanf("%d", &fence) == 1)) {
            if (width > MAX || width < MIN || height < MIN || height > MAX) {
                /* If width or height doesn't pass the check, an error message appears. */
                fprintf(stderr, "Error: Vstup mimo interval!\n");
                return 101;
            } else {
                if (width % 2 != 0) {
                    if (width == height) {
                        if (fence < height && fence > 0) {
                            /* 
                             * If values had passed all checks (and the width is odd and also equals height,
                             * and the fence value is lower than the height of the house and it's positive), 
                             * so builds the roof and then the flooded house with the fence. 
                             */
                            roof(width);
                            flood_house(width, height, fence);
                        } else {
                            /* If the fence value is higher than the height of the house, an error message appears. */
                            fprintf(stderr, "Error: Neplatna velikost plotu!\n");
                            return 103;
                        }
                    }
                } else {
                    /* If the width is even, an error message appears. */
                    fprintf(stderr, "Error: Sirka neni liche cislo!\n");
                    return 102;
                }
            }
        } else {
            if ((read = scanf("%d", &fence) == 0)) {
                /* If the entered fence value isn't correct, an error message appears. */
                fprintf(stderr, "Error: Chybny vstup!\n");
                return 100;
            } else {
                if (width > MAX || width < MIN || height < MIN || height > MAX) {
                    /* If width or height doesn't pass the check, an error message appears. */
                    fprintf(stderr, "Error: Vstup mimo interval!\n");
                    return 101;
                } else {
                    if (width % 2 != 0) {
                        /* 
                         * If values had passed all checks,
                         * (and the width is odd and the fence value wasn't entered), 
                         * so builds the roof and then the house. 
                         */
                        roof(width);
                        house(width, height);
                    } else {
                        /* If the width is even, an error message appears. */
                        fprintf(stderr, "Error: Sirka neni liche cislo!\n");
                        return 102;
                    }
                }
            }
        }
    } else {
        /* If the entered height or width value isn't correct, an error message appears. */
        fprintf(stderr, "Error: Chybny vstup!\n");
        return 100;
    }
}


/* This function builds the roof. */
int roof(int width)
{
    int i = 0;

    /* Getting the central point of the roof by taking width and divide it by 2. */
    for (i = 0; i < width / 2; i++) { 
        /* Filling up the left space with spaces based on the center info. */
        for (int j = 0; j < (width / 2) - i; j++) { 
            printf(" ");
        }
        printf("X");
        /* Filling up the right space with spaces based on the center info. */
        for (int k = 0; k <= (2 * i) - 2; k++) {
            printf(" ");
        }
        if (i > 0) {
            printf("X");
        }
        printf("\n");
    }
    return 0;
}


/* This function builds the house directly. */
int house(int width, int height)
{
    int i = 0;

    /* Creating the upper line of the house's square. */
    for (i = 0; i < width; i++) {
        printf("X");
    }
    printf("\n");
    for (i = 0; i < height - 2; i++) {
        printf("X");
        /* Adding " " for the middle space of the house. */
        for (int l = 0; l < width - 2; l++) {
            printf(" ");
        }
        printf("X");
        printf("\n");
    }
    /* Creating the bottom line of the house's square. */
    for (i = 0; i < width; i++) {
        printf("X");
    }
    printf("\n");
    return 0;
}


/* This function builds the flooded house directly (because it looks like flooded). */
int flood_house(int width, int height, int fence)
{
    int i = 0;

    /* Creating the upper line of the house's square. */
    for (i = 0; i < width; i++) {
        printf("X");
    }
    printf("\n");
    for (i = 0; i < height - 2; i++) {
        printf("X");
        if (i % 2 == 0) {
            /* Adding "o*" for the middle space of the house, if the line is even. */
            for (int l = 0; l < (width - 2) / 2; l++) {
                printf("o*");
            }
            printf("oX");
            fence_build(height, fence, i);
        } else {
            /* Adding "*o" for the middle space of the house, if the line is odd. */
            for (int l = 0; l < (width - 2) / 2; l++) {
                printf("*o");
            }
            printf("*X");
            fence_build(height, fence, i);
        }
        printf("\n");
    }
    /* Creating the bottom line of the house's square. */
    for (i = 0; i < width; i++) {
        printf("X");
    }
    /* Creating the bottom layer of the fence (depends on the "fence" value (even or odd)). */
    for (int k = 0; k < fence / 2; k++) {
        fence%2 == 0 ? printf("-|") : printf("|-");
    }
    if (fence%2 != 0)
        printf("|");
    printf("\n");
    return 0;
}


/*This function builds the fence (except the lower layer).*/
int fence_build(int height, int fence, int i)
{
    /* Creating the fence depends on the it's value (even or odd). */
    if (fence % 2 == 0) {
        if (i == height - fence - 1) {
            /* 
             * Adding the top layer of the fence, which was found by subtracting 
             * the height of the fence from the height of the house. 
             */
            for (int k = 0; k < fence / 2; k++) {
                printf("-|");
            }
        }
        if (i >= height - fence && i <= height) {
            /* Adding the fence directly from the top layer to the penultimate line. */
            for (int k = 0; k < fence / 2; k++) {
                printf(" |");
            }
        }
    } else {
        if (i == height - fence - 1) {
            /* 
             * Adding the top layer of the fence, which was found by subtracting 
             * the height of the fence from the height of the house. 
             */
            for (int k = 0; k < fence / 2; k++) {
                    printf("|-");
                }
        }
        if (i >= height - fence && i <= height) {
            /* Adding the fence directly from the top layer to the penultimate line. */
            for (int k = 0; k < fence / 2; k++) {
                printf("| ");         
            }
        }
        if (i >= height - fence - 1)
            printf("|");
    }
    return 0;
}
