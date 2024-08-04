
#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

#include <stdio.h>
#include <stdint.h>
#include "oled.h"

#define GRAPHIC_HEIGHT_PX OLED_HEIGHT_PX
#define GRAPHIC_WIDTH_PX OLED_WIDTH_PX

#define GRAPHIC_RED_256 (uint8_t)(0b11100000)
#define GRAPHIC_GREEN_256 (uint8_t)(0b11100)
#define GRAPHIC_BLUE_256 (uint8_t)(0b11)
#define GRAPHIC_WHITE_256 (uint8_t)(0b11111111)
#define GRAPHIC_BLACK_256 (uint8_t)(0)

typedef struct {
    int row;
    int col;
} point_t;

extern void display(void);
extern void set_background(uint8_t color);
extern void set_background_sect(int bnd, uint8_t col_left, uint8_t col_right);

#endif //_GRAPHIC_H_