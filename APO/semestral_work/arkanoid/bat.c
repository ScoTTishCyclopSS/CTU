#include "bat.h"
#include "level.h"

void initBat()
{
  bat.w = 40;
  bat.y = HEIGTH / 2 - THICKNESS;
  bat.x = (WIDTH - bat.w) / 2;
  bat.hp = 3;
  bat.score = 0;
}

void putBat(unsigned char *mem_base)
{
  int offset = bat.y * WIDTH * 2;
  for (int t = 0; t < THICKNESS; ++t)
    for (int i = offset + bat.x; i < offset + bat.x + bat.w; i += 4)
      *(volatile uint32_t *)(mem_base + (WIDTH * 2 * t) + i) = BAT_COLOR;
}

void moveBat(unsigned char *mem_base, uint32_t rgb_knobs_value, uint32_t rgb_knobs_value_old)
{
  int32_t new = (int32_t)rgb_knobs_value;
  int32_t old = (int32_t)rgb_knobs_value_old;

  // free pixels
  int free_pixels = WIDTH - bat.w;

  int pointer = free_pixels / 256;

  int dx = (new - old) * pointer;

  // setting new position
  if (bat.x + dx > BORDER_THICKNESS && bat.x + dx < WIDTH - bat.w - BORDER_THICKNESS)
  {

    // standart offset
    int offset = bat.y * WIDTH * 2;

    // drawing the previous position of the 'part of bat' as black (depends on direction, tho)
    if (dx > 0)
      for (int t = 0; t < THICKNESS; ++t)
        for (int i = offset + bat.x; i < offset + bat.x + dx; i += 4) // rigth
          *(volatile uint32_t *)(mem_base + (WIDTH * 2 * t) + i) = 0x0;

    if (dx < 0)
      for (int t = 0; t < THICKNESS; ++t)
        for (int i = offset + bat.x + dx; i < offset + bat.x + bat.w; i += 4) // left
          *(volatile uint32_t *)(mem_base + (WIDTH * 2 * t) + i) = 0x0;

    // finally setting the new coord of the bat
    bat.x += dx;
  }
}
