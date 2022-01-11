#ifndef BALL_H
#define BALL_H

#include <stdio.h>
#include <stdint.h>

#define BALL_COLOR 0xEA1FEA1F

/* ball structure */
typedef struct
{
    int x, y;
    int isMoving;
} Ball;

extern Ball ball;

/* ball initialization */
void initBall();

/* function that puts the ball in the starting position */
void putBall(unsigned char *mem_base);

/* function that controls the movement of the ball with the bat */
void moveBall(int dx, int dy, unsigned char *mem_base);

void moveBall(int dx, int dy, unsigned char *mem_base);

/* function that controls the direction of movement of the ball */
void motionBall(unsigned char *mem_base, unsigned char *mem_base_rgb);

#endif
