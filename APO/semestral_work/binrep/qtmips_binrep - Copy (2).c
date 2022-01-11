/*******************************************************************
  Simple program to demostrate binary reprezentation
  on QtMips emulator developed by Karel Koci and Pavel Pisa.

  qrmips_binrep.c       - main and only file

  (C) Copyright 2004 - 2019 by Pavel Pisa
      e-mail:   pisa@cmp.felk.cvut.cz
      homepage: http://cmp.felk.cvut.cz/~pisa
      work:     http://www.pikron.com/
      license:  any combination GPL, LGPL, MPL or BSD licenses

 *******************************************************************/

#define _POSIX_C_SOURCE 200112L

#include <stdint.h>
#include <memory.h>
#include <stdlib.h>

#define SPILED_REG_KNOBS_8BIT_o         0x024
#define SPILED_REG_BASE      0xffffc100

#define LCD_FB_START       0xffe00000 //экран старт
#define LCD_FB_END         0xffe4afff // экран конец

#define WIDTH 480 * 2
#define HEIGTH 240 * 2
#define BORDER_THICKNESS 10
#define THICKNESS 5

#define FIELD_COLOR 0xfae0fae0
#define LOSE_BORDER_COLOR 0xF800F800
#define BAT_COLOR 0x07e007e0
#define BALL_COLOR 0x00FF00FF
#define BLOCK_COLOR 0xFFFFFFFF

#define MALLOC_ERR 100

typedef struct {
    int x, y;
    int w;
} Bat;

typedef struct {
    int x, y;
    int isMoving;
} Ball;

typedef struct {
    int x, y;
    int isDestroyed;
} Block;

void create_field(unsigned char *mem_base);
void iniBat();
void putBat(unsigned char *mem_base);
void moveBat(unsigned char *mem_base, uint32_t rgb_knobs_value, uint32_t rgb_knobs_value_old);
void initBall();
void putBall(unsigned char *mem_base);
void moveBall (int dx, int dy, unsigned char *mem_base);
void motionBall (unsigned char *mem_base);
void genLevel (int* blocks);
void putBlocks (unsigned char *mem_base, int* blocks) ;

Bat bat;
Ball ball;

//starting vector is at 45 angle
int Vx = 1;
int Vy = 1;

int main(int argc, char *argv[])
{
  int blocks[100];

  unsigned char *mem_base = (unsigned char *) LCD_FB_START; //указатель на начало экрана (левый верх)
  unsigned char *mem_base_rgb = (unsigned char *) SPILED_REG_BASE; //указатель на значение rgb
  uint32_t rgb_knobs_value_old = *(volatile uint32_t*)(mem_base_rgb + SPILED_REG_KNOBS_8BIT_o);
  uint32_t rgb_knobs_value = rgb_knobs_value_old;

  create_field(mem_base);
  iniBat();
  initBall();
  genLevel(blocks);
  putBlocks(mem_base, blocks);

  while(1) {

    putBat(mem_base);
    if (ball.isMoving == 1)
      motionBall(mem_base);
    putBall(mem_base);

    // check the knob position
    rgb_knobs_value = *(volatile uint32_t*)(mem_base_rgb + SPILED_REG_KNOBS_8BIT_o);

    // if we dicedied to move -> then move, u lazy fuck
    if (rgb_knobs_value_old > 0 && rgb_knobs_value_old < 256 && // using only blue knob to push the bat
        rgb_knobs_value > 0 && rgb_knobs_value < 256 && 
        rgb_knobs_value_old != rgb_knobs_value) {
      moveBat(mem_base, rgb_knobs_value, rgb_knobs_value_old);
      if (ball.isMoving == 0)
        moveBall(bat.x + bat.w / 2, 0, mem_base);
    }

    // saving the knob value for the next iteration
    rgb_knobs_value_old = rgb_knobs_value;
  }
  return 0;
}



void putBlocks (unsigned char *mem_base, int* blocks) {
  for (int k = 0; k < 48; ++k) // 10 блоков надо нарисовать (массив содержит нужные иксы)
  {
    for (int i = 0; i < THICKNESS; ++i) // heigth of one block
      for (int t = 0; t < THICKNESS * 5; t++) // width of one block
        *(volatile uint32_t*)(mem_base + blocks[k] + WIDTH * i + t) = BLOCK_COLOR;
  }
}

void genLevel (int* blocks) {
  int k = 0;
  for (int i = BORDER_THICKNESS * 4; i < HEIGTH / 3; i+= THICKNESS * 4) 
  {
    int offset = (i - 1) * WIDTH;
    for (int j = BORDER_THICKNESS * 10; j < WIDTH - BORDER_THICKNESS * 10; j+=THICKNESS * 10, k++)
    {
      //if (k < 10)
      blocks[k] = offset + j;
    }
  }   
}

void motionBall (unsigned char *mem_base) {
  int old_x = ball.x;
  int offset = (ball.y - 1) * WIDTH;
  //БЕЗ БЛОКОВ ПОКА
  ball.x += Vx;
  ball.y -= Vy;

  /* BLOCKS COLLISION */
  for (int h = ball.y - THICKNESS; h < ball.y; ++h) { // block is on right
    int offset = (h - 1) * WIDTH;
    if(*(volatile uint32_t*)(mem_base + offset + ball.x + THICKNESS + 1) == BLOCK_COLOR) {
      Vx = -Vx;
      break;
    }
  }
  for (int h = ball.y - THICKNESS; h < ball.y; ++h) { // block is on left
    int offset = (h - 1) * WIDTH;
    if(*(volatile uint32_t*)(mem_base + offset + ball.x - 1) == BLOCK_COLOR) {
      Vx = -Vx;
      break;
    }
  }
  for (int w = ball.x; w < ball.x + THICKNESS; ++w) { // block is on top
    int offset = (ball.y - THICKNESS - 1) * WIDTH;
    if(*(volatile uint32_t*)(mem_base + offset + w) == BLOCK_COLOR) {
      Vy = -Vy;
      break;
    }
  }
  for (int w = ball.x; w < ball.x + THICKNESS; ++w) { // block is on bottom
    int offset = (ball.y + 1) * WIDTH;
    if(*(volatile uint32_t*)(mem_base + offset + w) == BLOCK_COLOR) {
      Vy = -Vy;
      break;
    }
  }


  /* FIELD COLLISION */
  if (ball.x + THICKNESS >= WIDTH - BORDER_THICKNESS) {// right (+THICKNESS because the coord X is on THICKNESS pixels left)
    Vx = -Vx;
  }
  if (ball.x <= BORDER_THICKNESS) {// left
    Vx = -Vx;
  }
  if (ball.y >= HEIGTH / 2) {// down ok
    initBall();
  }
  if (ball.y - THICKNESS <= BORDER_THICKNESS) {// up (-THICKNESS because the coord Y is on THICKNESS pixels down) ok
    Vy = -Vy;
  }


  // if (*(volatile uint32_t*)(mem_base + offset + ball.x + THICKNESS + WIDTH + 1) == BLOCK_COLOR) // right down
  //   {
  //       Vx = -Vx;
  //       Vy = -Vy;
  //   }

  //   if (*(volatile uint32_t*)(mem_base + offset + ball.x + THICKNESS + WIDTH + 1) == BLOCK_COLOR) // right up
  //   {
  //       Vx = -Vx;
  //       Vy = -Vy;
  //   }

  //   if (field[offset + WIDTH - 1] == '%') // left down
  //   {
  //       Vx = -Vx;
  //       Vy = -Vy;
  //   }

  //   if (field[offset - WIDTH - 1] == '%') // left up
  //   {
  //       Vx = -Vx;
  //       Vy = -Vy;
  //   }
    
  //bat collision
  for (int i = bat.x; i < bat.x + bat.w; i++)// through the bat width
    if (ball.x == i && ball.y >= bat.y) //TODO: collision NOT on direct hit bat
      Vy = -Vy;

  for (int i = 0; i < THICKNESS; ++i)
    for (int t = 0; t < THICKNESS; ++t)
      *(volatile uint32_t*)(mem_base + offset + WIDTH * (-i) + old_x + t) = 0x0;
}

void moveBall (int dx, int dy, unsigned char *mem_base) {
  int offset = (ball.y - 1) * WIDTH;
  int old_x = ball.x;

  if (dx != 0)
      ball.x = dx;
  if (dy != 0)
      ball.y = dy;

  for (int i = 0; i < THICKNESS; ++i)
    for (int t = 0; t < THICKNESS; ++t)
      *(volatile uint32_t*)(mem_base + offset + WIDTH * (-i) + old_x + t) = 0x0;
}

void putBall(unsigned char *mem_base) {
  int offset = (ball.y - 1) * WIDTH;
  for (int i = 0; i < THICKNESS; ++i)
    for (int t = 0; t < THICKNESS; ++t)
      *(volatile uint32_t*)(mem_base + offset + WIDTH * (-i) + ball.x + t) = BALL_COLOR;
}

void initBall() {
    ball.y = bat.y - 1;
    ball.x = bat.x + bat.w / 2;
    ball.isMoving = 1;
}

//TODO переделать расчет свбодного пространства для регуляторов
void moveBat(unsigned char *mem_base, uint32_t rgb_knobs_value, uint32_t rgb_knobs_value_old) {

  int32_t new = (int32_t)rgb_knobs_value;
  int32_t old = (int32_t)rgb_knobs_value_old;

   // free pixels
  int free_pixels = WIDTH - bat.w;

  int pointer = free_pixels / 256;

  int dx = (new - old) * pointer;
  
  // setting new position
  if (bat.x + dx > BORDER_THICKNESS && bat.x + dx < WIDTH - bat.w) {

    // standart offset
    int offset = (bat.y - 1) * WIDTH;

    // drawing the previous position of the 'part of bat' as black (depends on direction, tho)
    if (dx > 0)
      for (int t = 0; t < THICKNESS; ++t)
        for (int i = offset + bat.x; i < offset + bat.x + dx; i++) //rigth
          *(volatile uint32_t*)(mem_base + (WIDTH * t) + i) = 0x0;

    if (dx < 0)
      for (int t = 0; t < THICKNESS; ++t)
        for (int i = offset + bat.x + dx; i < offset + bat.x + bat.w; i++) //left
          *(volatile uint32_t*)(mem_base + (WIDTH * t) + i) = 0x0;

    // finally setting the new coord of the bat
    bat.x += dx;
  }
}

void putBat(unsigned char *mem_base) {
    int offset = (bat.y - 1) * WIDTH;
    for (int t = 0; t < THICKNESS; ++t)
      for (int i = offset + bat.x; i < offset + bat.x + bat.w; i++)
        *(volatile uint32_t*)(mem_base + (WIDTH * t) + i) = BAT_COLOR;
}

void iniBat() {
    bat.w = 100;
    bat.y = HEIGTH / 2 - THICKNESS;
    bat.x = (WIDTH - bat.w) / 2;
}

void create_field(unsigned char *mem_base) {

  for (int t = 0; t < BORDER_THICKNESS; ++t)
  {
    for (int w = 0; w < WIDTH; ++w)
    {
      *(volatile uint32_t*)(mem_base + WIDTH * t + w) = FIELD_COLOR;
      *(volatile uint32_t*)(mem_base + WIDTH * (HEIGTH / 2 + t) + w) = LOSE_BORDER_COLOR;
    }
  }

    //to the pre-last one 
    for (int h = BORDER_THICKNESS; h < HEIGTH / 2; h++) {
      for (int t = 0; t < BORDER_THICKNESS; t++)
      {
        *(volatile uint32_t*)(mem_base + (WIDTH * h) + t) = FIELD_COLOR;
        *(volatile uint32_t*)(mem_base + (WIDTH * h) + (WIDTH - BORDER_THICKNESS) + t) = FIELD_COLOR;
      }
    }
}
