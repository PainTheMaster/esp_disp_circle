
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "oled.h"
#include "graphic.h"
#include "_graphic.h"


uint8_t pix[GRAPHIC_HEIGHT_PX][GRAPHIC_WIDTH_PX];

void display(void){
    sendDataContinu(pix, GRAPHIC_HEIGHT_PX*GRAPHIC_WIDTH_PX);
}

extern void clear(void){
    int x, y;
    for(y=0; y <= GRAPHIC_HEIGHT_PX-1; y++){
        for(x=0; x <= GRAPHIC_WIDTH_PX-1; x++){
            pix[y][x] = 0;
        }
    }

}

void set_background(uint8_t color){
    int x, y;
    for(y=0; y <= GRAPHIC_HEIGHT_PX-1; y++){
        for(x=0; x<=GRAPHIC_WIDTH_PX-1; x++){
            pix[y][x] = color;
        }
    }
}

void set_background_sect(int bnd, uint8_t col_left, uint8_t col_right){
    int x, y;

    for(y=0; y <= GRAPHIC_HEIGHT_PX-1; y++){
        for(x=0; x<=bnd; x++){
            pix[y][x] = col_left;
        }
        for(; x<=GRAPHIC_WIDTH_PX-1; x++){
            pix[y][x] = col_right;
        }
    }
}

void circle(point_t* p_center, unsigned int radius, int color_rim, int color_inside){
    point_t left_top, right_bottom, dominant_quad, start;

    //scopingで計算する範囲を絞る。
    scoping(p_center, radius, &left_top, &right_bottom);

    //計算を行う象限を決定する。例えば第一象限に円の中心があれば、円の第３象限が計算対象になる。
    dominant_quadrant(p_center, &left_top, &right_bottom, &dominant_quad, &start);


    //いよいよ計算を行う。
    int y, temp_term;
//    y = start.row;
    const unsigned int rad_sq = radius * radius; 

    /*TODO: yの適切な終点をhorizontal_scan()に倣って実装する*/
    const int sign_y = dominant_quad.row;

    if(start.row==left_top.row){
        temp_term = right_bottom.row;
    } else {
        temp_term = left_top.row;
    }
    if(temp_term*sign_y < (p_center->row)*sign_y){
        temp_term = p_center->row;
    }
    const int terminal = temp_term;

    for(y=start.row;
        terminal*sign_y <= y*sign_y;
        y -= dominant_quad.row){
        
        hrizontal_scan(p_center, radius, &left_top, &right_bottom, &dominant_quad, &start, y, color_rim, color_inside);
    }
}