#include "level.h"
#include "peretherals.h"

void create_field(unsigned char *mem_base)
{

  for (int t = 0; t < BORDER_THICKNESS; ++t)
    for (int w = 0; w < WIDTH; w += 4)
    {
      *(volatile uint32_t *)(mem_base + WIDTH * 2 * t + w) = FIELD_COLOR;
      *(volatile uint32_t *)(mem_base + WIDTH * 2 * (HEIGTH / 2 + t) + w) = LOSE_BORDER_COLOR;
    }

  // to the pre-last one
  for (int h = BORDER_THICKNESS; h < HEIGTH / 2; h++)
    for (int t = 0; t < BORDER_THICKNESS; t += 4)
    {
      *(volatile uint32_t *)(mem_base + (WIDTH * 2 * h) + t) = FIELD_COLOR;
      *(volatile uint32_t *)(mem_base + (WIDTH * 2 * h) + (WIDTH - BORDER_THICKNESS) + t) = FIELD_COLOR;
    }
}

void genLevel(int *blocks)
{
  int k = 0;
  for (int i = BORDER_THICKNESS * 4; i < HEIGTH / 3; i += THICKNESS * 4)
  {
    int offset = i * WIDTH * 2;
    for (int j = BORDER_THICKNESS * 10; j < WIDTH - BORDER_THICKNESS * 10 - BLOCK_W; j += BLOCK_W + THICKNESS * 4, k++)
      blocks[k] = offset + j;
  }
}

void putBlocks(unsigned char *mem_base, int *blocks)
{
  for (int k = 0; k < BLOCK_COUNT; ++k)
    for (int i = 0; i < THICKNESS; ++i)    // heigth of one block
      for (int t = 0; t < BLOCK_W; t += 4) // width of one block
        *(volatile uint32_t *)(mem_base + blocks[k] + WIDTH * 2 * i + t) = BLOCK_COLOR;
}

void deleteBlock(unsigned char *mem_base, int start_pixel)
{
  uint32_t color;
  int h1 = 0;

  while (1)
  {
    int offset = WIDTH * 2 * h1;
    color = *(volatile uint32_t *)(mem_base + offset + start_pixel);
    if (color == 0x0)
      break;

    // clear to the right
    int w = 0;
    while (1)
    {
      color = *(volatile uint32_t *)(mem_base + offset + start_pixel + w);
      if (color == 0x0)
        break;
      *(volatile uint32_t *)(mem_base + offset + start_pixel + w) = 0x0;
      w += 4;
    }

    // clear to the left
    w = -4;
    while (1)
    {
      color = *(volatile uint32_t *)(mem_base + start_pixel + offset + w);
      if (color == 0x0)
        break;
      *(volatile uint32_t *)(mem_base + start_pixel + offset + w) = 0x0;
      w -= 4;
    }

    h1++;
  }

  int h2 = -1;
  while (1)
  {
    int offset = WIDTH * 2 * h2;
    color = *(volatile uint32_t *)(mem_base + offset + start_pixel);
    if (color == 0x0)
      break;

    // clear to the right
    int w = 0;
    while (1)
    {
      color = *(volatile uint32_t *)(mem_base + offset + start_pixel + w);
      if (color == 0x0)
        break;
      *(volatile uint32_t *)(mem_base + offset + start_pixel + w) = 0x0;
      w += 4;
    }

    // clear to the left
    w = -4;
    while (1)
    {
      color = *(volatile uint32_t *)(mem_base + start_pixel + offset + w);
      if (color == 0x0)
        break;
      *(volatile uint32_t *)(mem_base + start_pixel + offset + w) = 0x0;
      w -= 4;
    }
    h2--;
  }
}
