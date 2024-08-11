#include <stdio.h>
#include "graphic.h"
#include "oled.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

void app_main(void)
{
    oled_pin_config_t oled_pins ={
        .pin_gpio_cs = GPIO_NUM_23,
        .pin_gpio_dc = GPIO_NUM_22,
        .pin_gpio_res = GPIO_NUM_21,
        .pin_spi_mosi = GPIO_NUM_19,
        .pin_spi_clk = GPIO_NUM_18,
    };

    vTaskDelay(pdMS_TO_TICKS(5000));

    oled_init(&oled_pins);
    printf("init OK\n");
    vTaskDelay(pdMS_TO_TICKS(1000));


    point_t center={
        .row = 20,
        .col = 20,
    };

    const int radius =10;
    uint8_t rgb[]={GRAPHIC_RED_256, GRAPHIC_GREEN_256, GRAPHIC_BLUE_256};
    uint8_t color_rim, color_fill;
    int color_change = 0;
    color_rim = rgb[color_change%3];
    color_fill = rgb[(color_change+1)%3];

    gpio_set_direction(2, GPIO_MODE_OUTPUT);

    circle(&center, radius, color_rim, color_fill);
    display();
    printf("1st disp ok\n");
    vTaskDelay(pdMS_TO_TICKS(1000));

    int i, vx, vy;
    const int margin = 10;
    const int leftwall = 0 - margin;
    const int rightwall = GRAPHIC_WIDTH_PX-1+margin;
    const int upwall = 0 - margin;
    const int downwall = GRAPHIC_HEIGHT_PX-1+margin;


    vx = 1;
    vy = 2;
    i=0;
    while(1){
        clear();
        circle(&center, radius, color_rim, color_fill);
        display();
        vTaskDelay(pdMS_TO_TICKS(50));
        if(center.col-radius <= leftwall || rightwall <= center.col+radius){
            vx *= -1;
            color_change++;
            color_rim = rgb[color_change%3];
            color_fill = rgb[(color_change+1)%3];
            gpio_set_level(2, color_change%2);
        }
        if(center.row-radius <= upwall || downwall <= center.row+radius){
            vy *= -1;
            color_change++;
            color_rim = rgb[color_change%3];
            color_fill = rgb[(color_change+1)%3];
            gpio_set_level(2, color_change%2);
        }
        center.col += vx;
        center.row += vy;
        i++;
    //    vTaskDelay(pdMS_TO_TICKS(100));
    }


    gpio_set_direction(2, GPIO_MODE_OUTPUT);
    while(1){
        gpio_set_level(2, 1);
        vTaskDelay(pdMS_TO_TICKS(100));
        gpio_set_level(2, 0);
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    
}
