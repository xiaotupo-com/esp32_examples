/*
 * @Author: 小土坡 xiaotupo@163.com
 * @Date: 2025-06-14 00:22:19
 * @LastEditors: 小土坡 xiaotupo@163.com
 * @LastEditTime: 2025-06-15 01:22:50
 * @FilePath: /task_demo/main/task_demo.c
 * @Description:
 *
 * Copyright (c) 2025 by 小土坡, All Rights Reserved.
 */
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

static const char *TAG = "TaskDemo";

// FreeRTOS 任务句柄
TaskHandle_t led_task_handle = NULL;
TaskHandle_t msg_task_handle = NULL;

/* 引脚定义 */
#define LED_GPIO_PIN    GPIO_NUM_1  /* LED连接的GPIO端口 */

void led_task(void *pvParameters)
{
    // gpio_reset_pin(LED_GPIO_PIN); // 要把这行注释起来，否则运行会出错，因为 GPIO_NUM_1 是保留用途
    gpio_set_direction(LED_GPIO_PIN, GPIO_MODE_INPUT_OUTPUT);
    gpio_set_level(LED_GPIO_PIN, 1); // 初始化 LED 为关闭状态,高电平熄灭
    uint8_t led_state = 0;

    while (1) {
        ESP_LOGI(TAG, "LED Task Running");
        gpio_set_level(LED_GPIO_PIN, led_state = !led_state);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void msg_task(void *pvParameters)
{
    while (1) {
        ESP_LOGI(TAG, "Message Task Running");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void app_main(void)
{

    xTaskCreate(led_task, "LED Task", 2048, NULL, 5, &led_task_handle);
    xTaskCreatePinnedToCore(msg_task, "Message Task", 2048, NULL, 5, &msg_task_handle, 1);

    // 判断 led_task 是否创建成功
    if (led_task_handle == NULL) {
        ESP_LOGI(TAG, "Failed to create LED task");
    } else {
        ESP_LOGI(TAG, "LED task created successfully");
    }

    if (msg_task_handle == NULL) {
        ESP_LOGI(TAG, "Failed to create Message task");
    } else {
        ESP_LOGI(TAG, "Message task created successfully");
    }

    while (1) {
        ESP_LOGI(TAG, "Main Task Running");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
