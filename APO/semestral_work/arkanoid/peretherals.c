#include "peretherals.h"
#include "level.h"
#include "bat.h"

void writeReg(uint32_t base, uint32_t reg, uint32_t val)
{
  *(volatile uint32_t *)(base + reg) = val;
}

uint32_t readReg(uint32_t base, uint32_t reg)
{
  return *(volatile uint32_t *)(base + reg);
}

void sendTxByte(int data)
{
  while (!(readReg(SERIAL_PORT_BASE, SERP_TX_ST_REG_o) &
           SERP_TX_ST_REG_READY_m))
    ;
  writeReg(SERIAL_PORT_BASE, SERP_TX_DATA_REG_o, data);
}

void finalScore()
{
  int array[4];
  int n = bat.score;
  for (int i = 0; i < 3; i++)
  {
    array[i] = n % 10;
    n /= 10;
  }

  if (bat.hp <= 0)
  {
    const char *text = "Game over!\nYour final score is: ";
    const char *s;

    for (s = text; *s; s++)
      sendTxByte(*s);
  }
  else
  {
    const char *text = "You win!\nYour final score is: ";
    const char *s;

    for (s = text; *s; s++)
      sendTxByte(*s);
  }

  for (int j = 2; j >= 0; j--)
  {
    char c = array[j] + '0';
    sendTxByte(c);
  }

  sendTxByte('\n');
}

void warning()
{
  const char *text = "Please, turn green knob to zero!\n";
  const char *s;

  for (s = text; *s; s++)
    sendTxByte(*s);
}

void scoreLED(unsigned char *mem_base_rgb)
{
  if (bat.score < (BLOCK_COUNT * 10) / 2)
    *(volatile uint32_t *)(mem_base_rgb + SPILED_REG_LED_RGB1_o) = PROGRESS_COLOR_R;

  if (bat.score >= (BLOCK_COUNT * 10) / 2 && bat.score != BLOCK_COUNT * 10)
    *(volatile uint32_t *)(mem_base_rgb + SPILED_REG_LED_RGB1_o) = PROGRESS_COLOR_Y;

  if (bat.score >= BLOCK_COUNT * 10)
    *(volatile uint32_t *)(mem_base_rgb + SPILED_REG_LED_RGB1_o) = PROGRESS_COLOR_G;
}

void hpLED(unsigned char *mem_base_rgb)
{
  if (bat.hp == 3)
    *(volatile uint32_t *)(mem_base_rgb + SPILED_REG_LED_RGB2_o) = PROGRESS_COLOR_G;

  if (bat.hp == 2)
    *(volatile uint32_t *)(mem_base_rgb + SPILED_REG_LED_RGB2_o) = PROGRESS_COLOR_Y;

  if (bat.hp == 1)
    *(volatile uint32_t *)(mem_base_rgb + SPILED_REG_LED_RGB2_o) = PROGRESS_COLOR_R;
}
