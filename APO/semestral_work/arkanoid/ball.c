#include "ball.h"
#include "bat.h"
#include "level.h"

// starting vector is at 45 angle
int Vx = 1;
int Vy = 1;

void initBall()
{
  ball.y = bat.y - 1;
  ball.x = bat.x + bat.w / 2;
  ball.isMoving = 0;
}

void putBall(unsigned char *mem_base)
{
  int offset = ball.y * WIDTH * 2;
  for (int i = 0; i < THICKNESS; ++i)
    for (int t = 0; t < THICKNESS; t += 4)
      *(volatile uint32_t *)(mem_base + offset + WIDTH * 2 * (-i) + ball.x + t) = BALL_COLOR;
}

void moveBall(int dx, int dy, unsigned char *mem_base)
{
  int offset = ball.y * WIDTH * 2;
  int old_x = ball.x;

  if (dx != 0)
    ball.x = dx;
  if (dy != 0)
    ball.y = dy;

  for (int i = 0; i < THICKNESS; ++i)
    for (int t = 0; t < THICKNESS; t += 4)
      *(volatile uint32_t *)(mem_base + offset + WIDTH * 2 * (-i) + old_x + t) = 0x0;
}

void motionBall(unsigned char *mem_base, unsigned char *mem_base_rgb)
{
  int old_x = ball.x;
  int old_y = ball.y;

  ball.x += Vx;
  ball.y -= Vy;

  // right
  for (int h = ball.y - THICKNESS; h < ball.y; h++)
  {
    int offset = h * WIDTH * 2;
    uint32_t color = *(volatile uint32_t *)(mem_base + offset + ball.x + THICKNESS + 1);
    if (color == FIELD_COLOR)
    {
      Vx = -Vx;
      break;
    }
    if (color == BLOCK_COLOR)
    {
      deleteBlock(mem_base, offset + ball.x + THICKNESS + 1);
      bat.score = bat.score + 10;
      Vx = -Vx;
      break;
    }
  }

  // left
  for (int h = ball.y - THICKNESS; h < ball.y; h++)
  {
    int offset = h * WIDTH * 2;
    uint32_t color = *(volatile uint32_t *)(mem_base + offset + ball.x - 1);
    if (color == FIELD_COLOR)
    {
      Vx = -Vx;
      break;
    }
    if (color == BLOCK_COLOR)
    {
      deleteBlock(mem_base, offset + ball.x - 1);
      bat.score = bat.score + 10;
      Vx = -Vx;
      break;
    }
  }

  // top
  for (int w = ball.x; w < ball.x + THICKNESS; w++)
  {
    int offset = (ball.y - THICKNESS - 1) * WIDTH * 2;
    uint32_t color = *(volatile uint32_t *)(mem_base + offset + w);
    if (color == FIELD_COLOR)
    {
      Vy = -Vy;
      break;
    }
    if (color == BLOCK_COLOR)
    {
      deleteBlock(mem_base, offset + ball.x);
      bat.score = bat.score + 10;
      Vy = -Vy;
      break;
    }
  }

  // bottom
  for (int w = ball.x; w < ball.x + THICKNESS; w++)
  {
    int offset = (ball.y + 1) * WIDTH * 2;
    uint32_t color = *(volatile uint32_t *)(mem_base + offset + w);
    if (color == LOSE_BORDER_COLOR)
    {
      bat.hp = bat.hp - 1;
      bat.score -= 30;
      if (bat.score <= 0)
        bat.score = 0;
      Vy = -Vy;
      initBall(bat);
      break;
    }
    if (color == BLOCK_COLOR)
    {
      deleteBlock(mem_base, offset + ball.x);
      bat.score = bat.score + 10;
      Vy = -Vy;
      break;
    }
    if (color == BAT_COLOR)
    {
      Vy = -Vy;
      break;
    }
  }

  for (int i = 0; i < THICKNESS; ++i)
    for (int t = 0; t < THICKNESS; t += 4)
      *(volatile uint32_t *)(mem_base + (old_y * WIDTH * 2) + WIDTH * 2 * (-i) + old_x + t) = 0x0;
}
