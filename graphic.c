

#include "graphic.h"


static unsigned int dist_sq(int pt1_x, int pt1_y, int pt2_x, int pt2_y);
static void scoping(const point_t* center, unsigned int radius, point_t* p_left_top, point_t* p_right_bottom);
static unsigned int dist_sq(int pt1_x, int pt1_y, int pt2_x, int pt2_y);
static void dominant_quadrant(const point_t* center,
                              const point_t* p_left_top,
                              const point_t* p_right_bottom,
                              point_t* quad,
                              point_t* start);
/*static void seek_start(const point_t* p_center,
                        unsigned int radius,
                        const point_t* p_left_top,
                        const point_t* p_right_bottom,
                        const point_t* p_quad,
                        point_t* p_start);*/
static void copy(const point_t* p_center,
                 unsigned int radius,
                 const point_t* p_left_top,
                 const point_t* p_right_bottom,
                 int x,
                 int y,
                 int color);

static int is_in(const point_t* p_left_top,
                 const point_t* p_right_bottom,
                 int x,
                 int y);

uint8_t pix[GRAPHIC_HEIGHT_PX][GRAPHIC_WIDTH_PX];

void display(void){
    sendDataContinu(pix, GRAPHIC_HEIGHT_PX*GRAPHIC_WIDTH_PX);


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

void circle(point_t* center, unsigned int radius, int color_rim, int color_inside){
    point_t left_top, right_bottom, dominant_quad, start;
    
    //scopingで計算する範囲を絞る。
    scoping(&center, radius, &left_top, &right_bottom);

    //計算を行う象限を決定する。例えば第一象限に円の中心があれば、円の第３象限が計算対象になる。
    dominant_quadrant(&center, &left_top, &right_bottom, &dominant_quad, &start);

    //象限情報を用いて計算開始座標を求める
//    seek_start(&center, radius, &left_top, &right_bottom, &dominant_quad, &start);

    //いよいよ計算を行う。
    int x, y;
//    while (left<= y)

}

static void scoping(const point_t* center,
                    unsigned int radius,
                    point_t* p_left_top,
                    point_t* p_right_bottom) {

    p_left_top->row = center->row-radius;
    p_left_top->col = center->col-radius;
    p_right_bottom->row = center->row+radius;
    p_right_bottom->col = center->col+radius;
    
    //縦方向の処理
    if(p_left_top->row < 0){   //上辺が上にはみ出している時
        if(p_right_bottom->row < 0){ //さらに下辺も上にはみ出し
            p_left_top->row=-1;
            p_left_top->col=-1;
            p_right_bottom->row=-1;
            p_right_bottom->col=-1;
        } else if(p_right_bottom->row > GRAPHIC_HEIGHT_PX-1) {     //下辺が下にはみ出しているとき
            p_left_top->row=0;
            p_right_bottom->row=GRAPHIC_HEIGHT_PX-1;
        } else {    //下辺が範囲内。上辺のみ処理が必要
            p_left_top->row=0;   
        }
    } else if (0 <= p_left_top->row && p_left_top->row <= GRAPHIC_HEIGHT_PX -1){  //上辺が範囲内
        if (GRAPHIC_HEIGHT_PX - 1 < p_right_bottom->row) {     //下辺が下にはみ出し
            p_right_bottom->row = GRAPHIC_HEIGHT_PX - 1;
        } else {}  //上辺、下辺がともに範囲内であれば上下の処理は必要なし。わかりやすさのためにからのelse分
    } else {    //上辺が下にはみ出している時
        p_left_top->row=-1;
        p_left_top->col=-1;
        p_right_bottom->row=-1;
        p_right_bottom->col=-1;
    }
    
    //横方向の処理
    if(p_left_top->col < 0){   //左辺が左にはみ出しているとき
        if(p_right_bottom->col < 0){   //右辺も左にはみ出しているとき
            p_left_top->row=-1;
            p_left_top->col=-1;
            p_right_bottom->row=-1;
            p_right_bottom->col=-1;
        } else if (p_right_bottom->col > GRAPHIC_WIDTH_PX-1){   //同時に右辺が右側にはみ出している時
            p_left_top->col = 0;
            p_right_bottom->col = GRAPHIC_WIDTH_PX-1;
        } else{     //残りのケース=左がはみ出していて右辺が範囲内の時。左のみ処理が必要
            p_left_top->col = 0;
        }
    } else if(0 <= p_left_top->col && p_left_top->col <= GRAPHIC_WIDTH_PX-1){    //左辺が範囲内
        if(GRAPHIC_WIDTH_PX-1 < p_right_bottom->col){       //左辺が範囲内で右辺が範囲右側にはみ出し。
            p_right_bottom->col = GRAPHIC_WIDTH_PX-1;
        } else {}   //左辺が範囲内で右辺が残りのケース=右辺は範囲内の時は処理は必要なし。
    } else {    //残りのケース=左辺が右側にはみ出している時。
        p_left_top->row=-1;
        p_left_top->col=-1;
        p_right_bottom->row=-1;
        p_right_bottom->col=-1;
    }
}

static unsigned int dist_sq(int pt1_x, int pt1_y, int pt2_x, int pt2_y){
    int dx = pt2_x - pt1_x;
    int dy = pt2_y - pt1_y;
    return (unsigned int)(dx*dx+dy*dy);
}

static void dominant_quadrant(const point_t* center,
                              const point_t* p_left_top,
                              const point_t* p_right_bottom,
                              point_t* quad,
                              point_t* start){

    int boarder_x = GRAPHIC_WIDTH_PX/2-1;
    int boarder_y = GRAPHIC_HEIGHT_PX/2-1;
    if(center->row <= boarder_y){   //the center placed in the upper
        if(center->col <= boarder_x){   //the center place in the left
            quad->row = 1;
            quad->col = 1;
            start->row = p_right_bottom->row;
            start->col = p_right_bottom->col;
        }else{  //the center placed in the right
            quad->row = 1;
            quad->col = -1;
            start->row = p_right_bottom->row;
            start->col = p_left_top->col;
        }
    }else{  //the center place in the lower
        if(center->col <= boarder_x){   //the center place in the left
            quad->row = -1;
            quad->col = 1;
            start->row = p_left_top->row;
            start->col = p_right_bottom->col;
        }else{  //the center place in the right
            quad->row = -1;
            quad->col = -1;
            start->row = p_left_top->row;
            start->col = p_left_top->col;            
        }
    }
}

/*static void seek_start(const point_t* p_center,
                        unsigned int radius,
                        const point_t* p_left_top,
                        const point_t* p_right_bottom,
                        const point_t* p_quad,
                        point_t* p_start){
    int seek_x, seek_y;
    
    seek_x = p_center->col + (p_quad->col)*radius;
    seek_y = p_center->row + (p_quad->row)*radius;

    while(seek_x < p_left_top->col || p_right_bottom->col <seek_x){
        seek_x -= p_quad->col;
    }
    while(seek_y < p_left_top->row || p_right_bottom->row <seek_y){
        seek_y -= p_quad->row;
    }
   if(p_quad->row)
}*/

static void copy(const point_t* p_center,
                 unsigned int radius,
                 const point_t* p_left_top,
                 const point_t* p_right_bottom,
                 int x,
                 int y,
                 int color){
    int vec_x = x - p_center->col;
    int vec_y = y - p_center->row;
    int temp_x, temp_y;

    //horizontal
    temp_x = p_center->col - vec_x;
    temp_y = y;
    if(is_in(p_left_top, p_right_bottom, temp_x, temp_y)){
        pix[temp_y][temp_x] = color;
    }

    //vertical
    temp_x = x;
    temp_y = p_center->row - vec_y;
    if(is_in(p_left_top, p_right_bottom, temp_x, temp_y)){
        pix[temp_y][temp_x] = color;
    }

    temp_x = p_center->col - vec_x;
    temp_y = p_center->row - vec_y;
    if(is_in(p_left_top, p_right_bottom, temp_x, temp_y)){
        pix[temp_y][temp_x] = color;
    }
}

static int is_in(const point_t* p_left_top,
                 const point_t* p_right_bottom,
                 int x,
                 int y){
    if(p_left_top->col <= x &&
       x <= p_right_bottom->col &&
       p_left_top->row <= y &&
       y <= p_right_bottom->row){
        return true;
       } else {
        return false;
       }
}