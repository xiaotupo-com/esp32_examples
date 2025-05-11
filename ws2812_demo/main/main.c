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


void app_main(void)
{

    /* Configure the peripheral according to the LED type */
    ws2812_strip_t *strip = ws2812_create();
    if (!strip) {
        ESP_LOGE(TAG, "Failed to initialize WS2812 strip");
        return;
    }
    
    // led_set_on(strip, COLOR_RED);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    // led_set_on(strip, COLOR_GREEN);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    // led_set_on(strip, COLOR_YELLOW);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    // led_set_on(strip, COLOR_PINK);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    // led_set_on(strip, COLOR_PURPLE);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    // led_set_on(strip, COLOR_ORANGE);
    // vTaskDelay(pdMS_TO_TICKS(1000));

    led_set_pixel(strip, 0, COLOR_GREEN);
    led_set_pixel(strip, 1, COLOR_RED);
    led_set_pixel(strip, 2, COLOR_BLUE);

    vTaskDelay(pdMS_TO_TICKS(1000));
    //ws2812_set(strip, COLOR_BLUE, LED_EFFECT_BLINK_FAST); // LED_EFFECT_RAINBOW 
    led_set_rainbow(strip, 2); // 20ms间隔的彩虹效果
    while (1) {
        /* Toggle the LED state */
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
