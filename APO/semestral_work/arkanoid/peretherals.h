#ifndef PERETHERALS_H
#define PERETHERALS_H

#include <stdio.h>
#include <stdint.h>

#define SERIAL_PORT_BASE 0xffffc000
#define SERP_TX_ST_REG_o 0x08
#define SERP_TX_ST_REG_READY_m 0x1
#define SERP_TX_DATA_REG_o 0x0c
#define SPILED_REG_LED_LINE_o 0x004

#define SPILED_REG_LED_RGB1_o 0x010
#define SPILED_REG_LED_RGB2_o 0x014
#define PROGRESS_COLOR_R 0x00fd0000
#define PROGRESS_COLOR_Y 0x00ebf418
#define PROGRESS_COLOR_G 0x0000fa00

/* function that writes symbols to the terminal */
void writeReg(uint32_t base, uint32_t reg, uint32_t val);

/* function that reads symbols to be printed to the terminal */
uint32_t readReg(uint32_t base, uint32_t reg);

/* helper function for outputting symbols to the terminal */
void sendTxByte(int data);

/* function that prints the final score and game's message to the terminal */
void finalScore();

/* function that asks to return the ball launch knob (green knob) to the zero position */
void warning();

/*
 * function that sets the color of the first LED lamp:
 * red color:    100% blocks left;
 * yellow color: 50% blocks left;
 * green color:  0% blocks left;
 */
void scoreLED(unsigned char *mem_base_rgb);

/*
 * function that sets the color of the second LED lamp:
 * green color:    3 lives left;
 * yellow color: 2 lives left;
 * red color:  1 live left;
 */
void hpLED(unsigned char *mem_base_rgb);

#endif
