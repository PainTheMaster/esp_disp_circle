#include <stdio.h>
#include "graphic.h"

void app_main(void)
{
    oled_pin_config_t oled_pins ={
        .pin_gpio_cs = GPIO_NUM_23,
        .pin_gpio_dc = GPIO_NUM_22,
        .pin_gpio_res = GPIO_NUM_21,
        .pin_spi_mosi = GPIO_NUM_19,
        .pin_spi_clk = GPIO_NUM_18,
    };

    oled_init(&oled_pins);
    
}
