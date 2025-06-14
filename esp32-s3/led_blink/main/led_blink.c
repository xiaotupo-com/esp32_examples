/*
 * @Author: 小土坡 xiaotupo@163.com
 * @Date: 2025-06-07 01:38:50
 * @LastEditors: 小土坡 xiaotupo@163.com
 * @LastEditTime: 2025-06-08 22:11:19
 * @FilePath: /led_blick/home/zsf/projects/esp32_projects/esp32-s3/led_blink/main/led_blink.c
 * @Description: 
 * 
 * Copyright (c) 2025 by 小土坡, All Rights Reserved. 
 */
#include <stdio.h>

// 手动添加的头文件
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define TAG "LED_BLINK"

#define LED_GPIO GPIO_NUM_1 // 定义LED引脚

void led_blink_task(void *pvParameter)
{
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT); // 设置LED引脚为输出模式

    while (1) {
        gpio_set_level(LED_GPIO, 1); // 点亮LED
        ESP_LOGI(TAG, "LED ON"); // 打印日志
        vTaskDelay(pdMS_TO_TICKS(1000)); // 延时1秒
        gpio_set_level(LED_GPIO, 0); // 熄灭LED
        ESP_LOGI(TAG, "LED OFF"); // 打印日志
        vTaskDelay(pdMS_TO_TICKS(1000)); // 延时1秒
    }
}

void app_main(void)
{

    xTaskCreatePinnedToCore(
        led_blink_task,   // 任务函数
        "led_blink_task", // 任务名称
        2048,             // 堆栈大小
        NULL,             // 任务参数
        1,                // 任务优先级
        NULL,              // 任务句柄,
        1 // 指定到 App CPU 核心
    );
    
    while(1){
        vTaskDelay(pdMS_TO_TICKS(1000)); // 主任务延时1秒
        ESP_LOGI(TAG, "Main task running"); // 打印日志
        // 这里可以添加其他主任务的代码
    }
}
