/*
 * @Author: 小土坡 xiaotupo@163.com
 * @Date: 2025-04-30 23:37:34
 * @LastEditors: 小土坡 xiaotupo@163.com
 * @LastEditTime: 2025-05-03 19:56:32
 * @FilePath: /helloworld/main/main.c
 * @Description: 
 * 
 * Copyright (c) 2025 by 小土坡, All Rights Reserved. 
 */
/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define LED_PIN GPIO_NUM_2

void create_tasks() {

}

void app_main(void)
{
    // 创建任务
    create_tasks();
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    const TickType_t xFrequency = pdMS_TO_TICKS(1000);
    TickType_t xLastWakeTime;

    xLastWakeTime = xTaskGetTickCount();

    while(1) {
        gpio_set_level(LED_PIN, 1);
        ESP_LOGI("LED", "LED is on");
        // vTaskDelay(100/portTICK_PERIOD_MS);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);

        gpio_set_level(LED_PIN, 0);
        ESP_LOGI("LED", "LED is off");
        // vTaskDelay(500/portTICK_PERIOD_MS);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}
