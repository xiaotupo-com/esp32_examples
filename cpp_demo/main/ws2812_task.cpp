/*
 * @Author: 小土坡 xiaotupo@163.com
 * @Date: 2025-05-13 00:11:52
 * @LastEditors: 小土坡 xiaotupo@163.com
 * @LastEditTime: 2025-05-13 00:51:04
 * @FilePath: /cpp_demo/main/ws2812_task.cpp
 * @Description:
 *
 * Copyright (c) 2025 by 小土坡, All Rights Reserved.
 */
#include "ws2812_task.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


static const char *TAG = "ws2812_task.cpp";


ws2812_task::ws2812_task() {
    ESP_LOGI(TAG, "ws2812 Constructor out.");
}

void ws2812_task::task(void *param) {
  int count = 0;
  while (1) {
    count += 2;
    ESP_LOGI(TAG, "count: %d", count);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
