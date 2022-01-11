#include <stdint.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

#include "level.h"
#include "bat.h"
#include "ball.h"
#include "peretherals.h"

#define SPILED_REG_KNOBS_8BIT_o 0x024
#define SPILED_REG_BASE    0xffffc100

#define LCD_FB_START       0xffe00000
#define LCD_FB_END         0xffe4afff

Bat bat;
Ball ball;

int main(int argc, char *argv[]) {
  int blocks[100];

  unsigned char *mem_base = (unsigned char *)LCD_FB_START;           // pointer to the beginning of the screen (top left)
  unsigned char *mem_base_rgb = (unsigned char *)SPILED_REG_BASE;    // pointer to rgb value

  uint32_t rgb_knobs_value_old = *(volatile uint32_t *)(mem_base_rgb + SPILED_REG_KNOBS_8BIT_o);
  uint32_t rgb_knobs_value = rgb_knobs_value_old;

  create_field(mem_base);
  initBat();
  initBall();
  genLevel(blocks);
  putBlocks(mem_base, blocks);

  while (1) {
    scoreLED(mem_base_rgb);
    hpLED(mem_base_rgb);
    putBat(mem_base);

    // trow the ball
    if (ball.isMoving == 1)
      motionBall(mem_base, mem_base_rgb);

    // finish the game
    if (bat.hp <= 0 || bat.score == BLOCK_COUNT * 10) {
      finalScore();
      bat.hp = 3;
      bat.score = 0;
      initBall();
      putBlocks(mem_base, blocks);
    }

    putBall(mem_base);

    // check the knob position
    rgb_knobs_value = *(volatile uint32_t *)(mem_base_rgb + SPILED_REG_KNOBS_8BIT_o);

    // check if player wants to throw the ball
    if(ball.isMoving == 0 && rgb_knobs_value >= KNOB_SIZE && rgb_knobs_value < KNOB_SIZE * KNOB_SIZE ){
      ball.isMoving = 1;
      warning();
    }

    // if player dicedied to move -> then move
    if ( (rgb_knobs_value_old % KNOB_SIZE > 0) && (rgb_knobs_value_old % KNOB_SIZE < KNOB_SIZE) &&
        rgb_knobs_value % KNOB_SIZE > 0 && rgb_knobs_value % KNOB_SIZE < KNOB_SIZE && 
        rgb_knobs_value_old % KNOB_SIZE != rgb_knobs_value % KNOB_SIZE){
      moveBat(mem_base, rgb_knobs_value % 256, rgb_knobs_value_old % 256);
      if (ball.isMoving == 0)
        moveBall(bat.x + bat.w / 2, 0, mem_base);
    }

    // saving the knob value for the next iteration
    rgb_knobs_value_old = rgb_knobs_value;
  }

  return 0;
}
