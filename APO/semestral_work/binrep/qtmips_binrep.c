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
#include "qtmips_binrep_base.c"

#define SPILED_REG_KNOBS_8BIT_o         0x024
#define SPILED_REG_BASE      0xffffc100

#define SPILED_REG_LED_RGB1_o           0x010

#define LCD_FB_START       0xffe00000 //экран старт
#define LCD_FB_END         0xffe4afff // экран конец

#define WIDTH 240 * 2
#define HEIGTH 160 * 2
#define BORDER_THICKNESS 3
#define THICKNESS 2

#define BLOCK_COUNT 25
#define BLOCK_W 12 * 2

#define FIELD_COLOR 0xfae0fae0
#define LOSE_BORDER_COLOR 0xF800F800
#define BAT_COLOR 0x07e007e0
#define BALL_COLOR 0x00FF00FF
#define BLOCK_COLOR 0xFFFFFFFF

typedef struct {
    int x, y;
    int w;
} Bat;

typedef struct {
    int x, y;
    int isMoving;
} Ball;

void create_field(unsigned char *mem_base);
void iniBat();
void putBat(unsigned char *mem_base);
void moveBat(unsigned char *mem_base, uint32_t rgb_knobs_value, uint32_t rgb_knobs_value_old);
void initBall();
void putBall(unsigned char *mem_base);
void moveBall (int dx, int dy, unsigned char *mem_base);
void motionBall (unsigned char *mem_base);
void genLevel (int* blocks);
void putBlocks (unsigned char *mem_base, int* blocks);
void deleteBlock(unsigned char *mem_base, int start_pixel);

Bat bat;
Ball ball;

//starting vector is at 45 angle
int Vx = 1;
int Vy = 1;

int main(int argc, char *argv[]) {
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
  for (int k = 0; k < BLOCK_COUNT; ++k) // 10 блоков надо нарисовать (массив содержит нужные иксы)
  {
    for (int i = 0; i < THICKNESS; ++i) // heigth of one block
      for (int t = 0; t < BLOCK_W; t+=4) // width of one block
        *(volatile uint32_t*)(mem_base + blocks[k] + WIDTH * 2 * i + t) = BLOCK_COLOR;
  }
}

void genLevel (int* blocks) {
  int k = 0;
  for (int i = BORDER_THICKNESS * 4; i < HEIGTH / 3; i+= THICKNESS * 4) 
  {
    int offset = i * WIDTH * 2;
    for (int j = BORDER_THICKNESS * 10; j < WIDTH - BORDER_THICKNESS * 10 - BLOCK_W; j+=BLOCK_W + THICKNESS * 4, k++)
    {
      blocks[k] = offset + j;
    }
  }   
}

void motionBall (unsigned char *mem_base) {
  int old_x = ball.x;
  int old_y = ball.y;

  ball.x += Vx;
  ball.y -= Vy;

  // right
  for(int h = ball.y - THICKNESS; h < ball.y; h++) {
    int offset = h * WIDTH * 2;
    uint32_t color = *(volatile uint32_t*)(mem_base + offset + ball.x + THICKNESS + 1);
    if(color == FIELD_COLOR) {
      Vx = -Vx;
      break;
    }
    if(color == BLOCK_COLOR) {
      deleteBlock(mem_base, offset + ball.x + THICKNESS + 1);
      Vx = -Vx;
      break;
    }
  }

  // left
  for(int h = ball.y - THICKNESS; h < ball.y; h++) {
    int offset = h * WIDTH * 2;
    uint32_t color = *(volatile uint32_t*)(mem_base + offset + ball.x - 1);
    if(color == FIELD_COLOR) {
      Vx = -Vx;
      break;
    }
    if(color == BLOCK_COLOR) {
      deleteBlock(mem_base, offset + ball.x - 1); 
      Vx = -Vx;
      break;
    }
  }

  // top
  for(int w = ball.x; w < ball.x + THICKNESS; w++) {
    int offset = (ball.y - THICKNESS - 1) * WIDTH * 2;
    uint32_t color = *(volatile uint32_t*)(mem_base + offset + w);
    if(color == FIELD_COLOR) {
      Vy = -Vy;
      break;
    }
    if(color == BLOCK_COLOR) {
      deleteBlock(mem_base, offset + ball.x); 
      Vy = -Vy;
      break;
    }
  }

  // bottom
  for(int w = ball.x; w < ball.x + THICKNESS; w++) {
    int offset = (ball.y + 1) * WIDTH * 2;
    uint32_t color = *(volatile uint32_t*)(mem_base + offset + w);
    if(color == LOSE_BORDER_COLOR) {
      Vy = -Vy;
      break;
    }
    if(color == BLOCK_COLOR) {
      deleteBlock(mem_base, offset + ball.x);
      Vy = -Vy;
      break;
    }
    if(color == BAT_COLOR) {
      Vy = -Vy;
      break;
    }
  }

  for (int i = 0; i < THICKNESS; ++i)
    for (int t = 0; t < THICKNESS; t+=4)
      *(volatile uint32_t*)(mem_base + (old_y * WIDTH * 2) + WIDTH  * 2 * (-i) + old_x + t) = 0x0;
}

void deleteBlock(unsigned char *mem_base, int start_pixel) {
  uint32_t color;

  int h1 = 0;
  while(1) {
    int offset = WIDTH * 2 * h1;
    color = *(volatile uint32_t*)(mem_base + offset + start_pixel );
    if (color == 0x0) break;

    // clear to the right
    int w = 0;
    while (1) {
      color = *(volatile uint32_t*)(mem_base + offset + start_pixel + w);
      if (color == 0x0) break;
      *(volatile uint32_t*)(mem_base + offset + start_pixel + w) = 0x0;
      w += 4;
    }

    // clear to the left
    w = -4;
    while (1) {
      color = *(volatile uint32_t*)(mem_base + start_pixel + offset + w);
      if (color == 0x0) break;
      *(volatile uint32_t*)(mem_base + start_pixel + offset + w) = 0x0;
      w-=4;
    }

    h1++;
  }

  int h2 = -1;
  while(1) {
    int offset = WIDTH * 2 * h2;
    color = *(volatile uint32_t*)(mem_base + offset + start_pixel);
    if (color == 0x0) break;

    // clear to the right
    int w = 0;
    while (1) {
      color = *(volatile uint32_t*)(mem_base + offset + start_pixel + w);
      if (color == 0x0) break;
      *(volatile uint32_t*)(mem_base + offset + start_pixel + w) = 0x0;
      w += 4;
    }

    //clear to the left
    w = -4;
    while (1) {
      color = *(volatile uint32_t*)(mem_base + start_pixel + offset + w);
      if (color == 0x0) break;
      *(volatile uint32_t*)(mem_base + start_pixel + offset + w) = 0x0;
      w-=4;
    }
    h2--;
  }
}

void moveBall (int dx, int dy, unsigned char *mem_base) {
  int offset = ball.y * WIDTH * 2;
  int old_x = ball.x;

  if (dx != 0)
      ball.x = dx;
  if (dy != 0)
      ball.y = dy;

  for (int i = 0; i < THICKNESS; ++i)
    for (int t = 0; t < THICKNESS; t+=4)
      *(volatile uint32_t*)(mem_base + offset + WIDTH * 2 * (-i) + old_x + t) = 0x0;
}

void putBall(unsigned char *mem_base) {
  int offset = ball.y * WIDTH * 2;
  for (int i = 0; i < THICKNESS; ++i)
    for (int t = 0; t < THICKNESS; t+=4)
      *(volatile uint32_t*)(mem_base + offset + WIDTH * 2 * (-i) + ball.x + t) = BALL_COLOR;
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
  if (bat.x + dx > BORDER_THICKNESS && bat.x + dx < WIDTH - bat.w - BORDER_THICKNESS) {

    // standart offset
    int offset = bat.y * WIDTH * 2;

    // drawing the previous position of the 'part of bat' as black (depends on direction, tho)
    if (dx > 0)
      for (int t = 0; t < THICKNESS; ++t)
        for (int i = offset + bat.x; i < offset + bat.x + dx; i+=4) //rigth
          *(volatile uint32_t*)(mem_base + (WIDTH  * 2 * t) + i) = 0x0;

    if (dx < 0)
      for (int t = 0; t < THICKNESS; ++t)
        for (int i = offset + bat.x + dx; i < offset + bat.x + bat.w; i+=4) //left
          *(volatile uint32_t*)(mem_base + (WIDTH  * 2 * t) + i) = 0x0;

    // finally setting the new coord of the bat
    bat.x += dx;
  }
}

void putBat(unsigned char *mem_base) {
    int offset = bat.y * WIDTH * 2;
    for (int t = 0; t < THICKNESS; ++t)
      for (int i = offset + bat.x; i < offset + bat.x + bat.w; i+=4)
        *(volatile uint32_t*)(mem_base + (WIDTH  * 2 * t) + i) = BAT_COLOR;
}

void iniBat() {
    bat.w = 40;
    bat.y = HEIGTH / 2 - THICKNESS;
    bat.x = (WIDTH - bat.w) / 2;
}

void create_field(unsigned char *mem_base) {

  for (int t = 0; t < BORDER_THICKNESS; ++t)
  {
    for (int w = 0; w < WIDTH; w+=4)
    {
      *(volatile uint32_t*)(mem_base + WIDTH  * 2 * t + w) = FIELD_COLOR;
      *(volatile uint32_t*)(mem_base + WIDTH  * 2 * (HEIGTH / 2 + t) + w) = LOSE_BORDER_COLOR;
    }
  }

    //to the pre-last one 
    for (int h = BORDER_THICKNESS; h < HEIGTH / 2; h++) {
      for (int t = 0; t < BORDER_THICKNESS; t+=4)
      {
        *(volatile uint32_t*)(mem_base + (WIDTH  * 2 * h) + t) = FIELD_COLOR;
        *(volatile uint32_t*)(mem_base + (WIDTH  * 2 * h) + (WIDTH - BORDER_THICKNESS) + t) = FIELD_COLOR;
      }
    }
}
