/*
 * @Author: 小土坡 xiaotupo@163.com
 * @Date: 2025-05-09 21:13:44
 * @LastEditors: 小土坡 xiaotupo@163.com
 * @LastEditTime: 2025-06-06 01:30:27
 * @FilePath: /ws2812_demo/main/main.c
 * @Description: 
 * 
 * Copyright (c) 2025 by 小土坡, All Rights Reserved. 
 */
/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "ws2812_control.h"




static const char *TAG = "example";

#if STRIP_DEMO
void strip_demo()
{
ws2812_strip_t *strip = ws2812_create();
    if (!strip) {
        ESP_LOGE(TAG, "Failed to initialize WS2812 strip");
        return;
    }

    vTaskDelay(pdMS_TO_TICKS(1500));
    
    led_set_on(strip, COLOR_RED);
    vTaskDelay(pdMS_TO_TICKS(1000));
    led_set_on(strip, COLOR_GREEN);
    vTaskDelay(pdMS_TO_TICKS(1000));
    led_set_on(strip, COLOR_YELLOW);
    vTaskDelay(pdMS_TO_TICKS(1000));
    led_set_on(strip, COLOR_PINK);
    vTaskDelay(pdMS_TO_TICKS(1000));
    led_set_on(strip, COLOR_PURPLE);
    vTaskDelay(pdMS_TO_TICKS(1000));
    led_set_on(strip, COLOR_ORANGE);
    vTaskDelay(pdMS_TO_TICKS(1000));
    led_set_on(strip, COLOR_WHITE);
    vTaskDelay(pdMS_TO_TICKS(1000));

    
    
    led_set_pixel(strip, 0, COLOR_GREEN);
    led_set_pixel(strip, 1, COLOR_RED);
    led_set_pixel(strip, 2, COLOR_BLUE);

    vTaskDelay(pdMS_TO_TICKS(1000));
    ws2812_set(strip, COLOR_GREEN, LED_EFFECT_BLINK_FAST); // LED_EFFECT_RAINBOW 
    led_set_rainbow(strip, 24); // 20ms间隔的彩虹效果
}

#endif

void matrx_demo()
{
    ws2812_matrix_t* WS2812 = ws2812_matrix_create();

    //设置LED为红色
    led_matrix_set_pixel(WS2812,0,0,COLOR_RGB(255,0,0));
    led_matrix_set_pixel(WS2812,1,1,COLOR_RGB(255,0,0));
    led_matrix_set_pixel(WS2812,2,2,COLOR_RGB(255,0,0));
    led_matrix_set_pixel(WS2812,3,3,COLOR_RGB(255,0,0));

    //更新矩阵显示
    led_matrix_show(WS2812);
}

void app_main(void)
{

    /* Configure the peripheral according to the LED type */
    // strip_demo();
    matrx_demo();

    while (1) {
        /* Toggle the LED state */
        ESP_LOGE(TAG, "app_main...");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
