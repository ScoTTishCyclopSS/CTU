#ifndef LEVEL_H
#define LEVEL_H

#include <stdio.h>
#include <stdint.h>

#define FIELD_COLOR 0xfae0fae0
#define LOSE_BORDER_COLOR 0xF800F800

#define BORDER_THICKNESS 3
#define THICKNESS 2

#define WIDTH 240 * 2
#define HEIGTH 160 * 2

#define BLOCK_COUNT 40
#define BLOCK_W 12 * 2

#define BLOCK_COLOR 0xFFFFFFFF

#define KNOB_SIZE 256

/* function that generates a field and its borders */
void create_field(unsigned char *mem_base);

/* function that generates blocks for new level */
void genLevel(int *blocks);

/* a function that puts blocks on the field */
void putBlocks(unsigned char *mem_base, int *blocks);

/* function that removes blocks when the ball hits them */
void deleteBlock(unsigned char *mem_base, int start_pixel);

#endif
