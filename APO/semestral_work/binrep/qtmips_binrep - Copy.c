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

/*
 * Next macros provides location of knobs and LEDs peripherals
 * implemented on QtMips simulator.
 *
 * More information can be found on page
 *   https://github.com/ppisa/QtMips
 */


/*
 * Base address of the region where simple serial port (UART)
 * implementation is mapped in emulated MIPS address space
 */
#define SERIAL_PORT_BASE   0xffffc000
/*
 * Byte offset of the 32-bit transition status register
 * of the serial port
 */
#define SERP_TX_ST_REG_o         0x08

/*
 * Mask of the bit which inform that peripheral is ready to accept
 * next character to send. If it is zero, then peripheral is
 * busy by sending of previous character.
 */
#define SERP_TX_ST_REG_READY_m    0x1
/*
 * Byte offset of the UART transmit register.
 * When the 32-bit word is written the least-significant (LSB)
 * eight bits are send to the terminal.
 */
#define SERP_TX_DATA_REG_o        0x0c

/*
 * Base address of the region where knobs and LEDs peripherals
 * are mapped in the emulated MIPS physical memory address space.
 */
#define SPILED_REG_BASE      0xffffc100

/* Valid address range for the region */
#define SPILED_REG_SIZE      0x00000100

/*
 * Byte offset of the register which controls individual LEDs
 * in the row of 32 yellow LEDs. When the corresponding bit
 * is set (value 1) then the LED is lit.
 */
#define SPILED_REG_LED_LINE_o           0x004

/*
 * The register to control 8 bit RGB components of brightness
 * of the first RGB LED
 */
#define SPILED_REG_LED_RGB1_o           0x010

/*
 * The register to control 8 bit RGB components of brightness
 * of the second RGB LED
 */
#define SPILED_REG_LED_RGB2_o           0x014

/*
 * The register which combines direct write to RGB signals
 * of the RGB LEDs, write to the keyboard scan register
 * and control of the two additional individual LEDs.
 * The direct write to RGB signals is orred with PWM
 * signal generated according to the values in previous
 * registers.
 */
#define SPILED_REG_LED_KBDWR_DIRECT_o   0x018

/*
 * Register providing access to unfiltered encoder channels
 * and keyboard return signals.
 */
#define SPILED_REG_KBDRD_KNOBS_DIRECT_o 0x020

/*
 * The register representing knobs positions as three
 * 8-bit values where each value is incremented
 * and decremented by the knob relative turning.
 */
#define SPILED_REG_KNOBS_8BIT_o         0x024


#define LCD_FB_START       0xffe00000 //экран старт
#define LCD_FB_END         0xffe4afff // экран конец



static inline void serp_write_reg(uint32_t base, uint32_t reg, uint32_t val)
{
  *(volatile uint32_t *)(base + reg) = val;
}
static inline uint32_t serp_read_reg(uint32_t base, uint32_t reg)
{
  return *(volatile uint32_t *)(base + reg);
}


#define WIDTH 240 * 2
#define HEIGTH 160 * 2
#define THICKNESS 4
#define BLOCKS_COUNT 20

#define FIELD_COLOR 0xF800F800
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
    if (rgb_knobs_value_old > 0 && rgb_knobs_value_old < 255 && // using only blue knob to push the bat
        rgb_knobs_value > 0 && rgb_knobs_value < 255 && 
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
  for (int k = 0; k < 100; ++k) // 10 блоков надо нарисовать (массив содержит нужные иксы)
  {
    for (int i = 0; i < THICKNESS; ++i) // heigth
      for (int t = 0; t < THICKNESS * 2; ++t) // width 
        *(volatile uint32_t*)(mem_base + blocks[k] + (WIDTH * 2) * i + t) = BLOCK_COLOR;
  }
}

void genLevel (int* blocks) {
  int k = 0;
  for (int i = THICKNESS * 4; i < HEIGTH / 4; i+= THICKNESS * 4) 
  {
    int offset = (i - 1) * WIDTH * 2;
    for (int j = THICKNESS * 4; j < WIDTH - THICKNESS * 4; j+=THICKNESS * 4, k++)
    {
      //if (k < 10)
      blocks[k] = offset + j;
    }
  }   
}

void motionBall (unsigned char *mem_base) {
    int offset = (ball.y - 1) * WIDTH * 2;
    int old_x = ball.x;

    //БЕЗ БЛОКОВ ПОКА
    ball.x += Vx;
    ball.y -= Vy;

    if (ball.x >= WIDTH - THICKNESS) {// right
        Vx = -Vx;
    }
    if (ball.x <= THICKNESS) {// left ok
        Vx = -Vx;
    }
    if (ball.y >= HEIGTH / 2) {// down
        Vy = -Vy;
    }
    if (ball.y <= THICKNESS * 2) {// up
        Vy = -Vy;
    }
    
    //bat collision
    for (int i = bat.x; i < bat.x + bat.w; i++) // through the bat width
        if (ball.x == i && ball.y == bat.y - 1) //TODO: collision NOT on direct hit bat
            Vy = -Vy;

    for (int i = 0; i < THICKNESS; ++i)
      for (int t = 0; t < THICKNESS; ++t)
        *(volatile uint32_t*)(mem_base + offset + (WIDTH * 2) * (-i) + old_x + t) = 0x0;
}

void moveBall (int dx, int dy, unsigned char *mem_base) {
    int offset = (ball.y - 1) * WIDTH * 2;
    int old_x = ball.x;

    if (dx != 0)
        ball.x = dx;
    if (dy != 0)
        ball.y = dy;

    for (int i = 0; i < THICKNESS; ++i)
      for (int t = 0; t < THICKNESS; ++t)
        *(volatile uint32_t*)(mem_base + offset + (WIDTH * 2) * (-i) + old_x + t) = 0x0;
}

void putBall(unsigned char *mem_base) {
  int offset = (ball.y - 1) * WIDTH * 2;
  for (int i = 0; i < THICKNESS; ++i)
    for (int t = 0; t < THICKNESS; ++t)
      *(volatile uint32_t*)(mem_base + offset + (WIDTH * 2) * (-i) + ball.x + t) = BALL_COLOR;
}

void initBall() {
    ball.y = bat.y - 1;
    ball.x = bat.x + bat.w / 2;
    ball.isMoving = 1;
}

void moveBat(unsigned char *mem_base, uint32_t rgb_knobs_value, uint32_t rgb_knobs_value_old) {

  int32_t new = (int32_t)rgb_knobs_value;
  int32_t old = (int32_t)rgb_knobs_value_old;

  // free pixels to the right and left, each one with 127 nob divisions (255 - 1 center / 2)
  int free_pixels = (WIDTH - bat.w) / 2;

  // => 127 делений деленных на кол-во пикселей одной свободной стороны 
  //= кол-во делений на нобе, которое необходимо поменять для перемещения иты на один пиксель, округлять стоит в большую сторону!
  int pointer = (127 + free_pixels - 1) / free_pixels; //~ 1,33 -> 2.0

  // comparing the current position of the knob with the previous one 
  // and divided by "pointer" will give us the number of pixels to shift
  int dx = (new - old) / pointer;
  
  // setting new position
  if (bat.x + dx > 0 && bat.x + dx < WIDTH - bat.w) {

    // standart offset
    int offset = (bat.y - 1)* WIDTH * 2;

    // drawing the previous position of the 'part of bat' as black (depends on direction, tho)
    if (dx > 0)
      for (int i = offset + bat.x; i < offset + bat.x + dx; i++) //rigth
        for (int t = 0; t < THICKNESS; ++t)
          *(volatile uint32_t*)(mem_base + (WIDTH * 2 * t) + i) = 0x0;

    if (dx < 0)
      for (int i = offset + bat.x + dx; i < offset + bat.x + bat.w; i++) //left
        for (int t = 0; t < THICKNESS; ++t)
          *(volatile uint32_t*)(mem_base + (WIDTH * 2 * t) + i) = 0x0;

    // finally setting the new coord of the bat
    bat.x += dx;
  }

}


void putBat(unsigned char *mem_base) {
    int offset = (bat.y - 1) * WIDTH * 2;
    for (int i = offset + bat.x; i < offset + bat.x + bat.w; i++)
      for (int t = 0; t < THICKNESS; ++t)
        *(volatile uint32_t*)(mem_base + (WIDTH * 2 * t) + i) = BAT_COLOR;
}

void iniBat() {
    bat.w = 50;
    bat.y = HEIGTH / 2 - 10;
    bat.x = (WIDTH - bat.w) / 2;
}


void create_field(unsigned char *mem_base) {

    //1st row and last
    for (int i = 0; i < WIDTH; i+=4) {
      for (int t = 0; t < THICKNESS; ++t)
      {
        int offset = WIDTH * 2;
        *(volatile uint32_t*)(mem_base + i + offset* t ) = FIELD_COLOR;
        *(volatile uint32_t*)(mem_base + (WIDTH * HEIGTH) + i + (offset * t)) = FIELD_COLOR;
      }
    }

    //to the pre-last one 
    for (int i = 1; i < HEIGTH; i++) {
      for (int t = 0; t < THICKNESS; t++)
      {
        *(volatile uint32_t*)(mem_base + (WIDTH * i) + t) = FIELD_COLOR;
      }
    }
}
