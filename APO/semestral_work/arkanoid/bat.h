#ifndef BAT_H
#define BAT_H

#include <stdio.h>
#include <stdint.h>

#define BAT_COLOR 0x07e007e0

/* bat structure */
typedef struct
{
    int x, y;
    int w;
    int hp;
    int score;
} Bat;

extern Bat bat;

/* bat initialization */
void initBat();

/* function that puts the bat in the starting position */
void putBat(unsigned char *mem_base);

/* function that controls the movement of the bat */
void moveBat(unsigned char *mem_base, uint32_t rgb_knobs_value, uint32_t rgb_knobs_value_old);

#endif
