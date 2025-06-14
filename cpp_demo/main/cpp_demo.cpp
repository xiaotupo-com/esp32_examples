/*
 * @Author: 小土坡 xiaotupo@163.com
 * @Date: 2025-05-12 23:10:14
 * @LastEditors: 小土坡 xiaotupo@163.com
 * @LastEditTime: 2025-05-13 00:58:43
 * @FilePath: /cpp_demo/main/cpp_demo.cpp
 * @Description: 
 * 
 * Copyright (c) 2025 by 小土坡, All Rights Reserved. 
 */
#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ws2812_task.h"


extern "C" void app_main(void)
{
    xTaskCreate(ws2812_task::task, "app_ws2812", 2048, NULL, 1, NULL);
}
