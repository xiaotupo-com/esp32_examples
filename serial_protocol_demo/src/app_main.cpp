/**
 * @file app_main.cpp
 * @author xtp (xiaotupo@163.com)
 * @brief 串口协议实验
 * @version V0.1
 * @date 2025-06-26
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
#include "serial_protocol.h"
#include <stdio.h>
#include "uart_init.h"

static const char *TAG = "Demo";

/**
 * @brief 该函数用于创建 FreeRTOS 任务
 *
 */
void xtp_create_task(void)
{
    BaseType_t rel = xTaskCreatePinnedToCore(xtp::Led::led_task, "led_task",
                                             2048, nullptr, 5, nullptr, 1);

    if (rel == pdPASS) {
        ESP_LOGI(TAG, "led_task 任务创建成功!");
    } else {
        ESP_LOGI(TAG, "led_task 任务创建失败!");
    }

    rel = xTaskCreatePinnedToCore(xtp::uart_protocol::rxTask, "rxTask", 4096,
                                  nullptr, 5, nullptr, 1);

    if (rel == pdPASS) {
        ESP_LOGI(TAG, "rxTask 任务创建成功!");
    } else {
        ESP_LOGI(TAG, "rxTask 任务创建失败!");
    }
}

/**
 * @brief
 *
 */
extern "C" void app_main(void)
{

    xtp::uart::uart_init(0); /* 初始化 uart */
    xtp_create_task(); /* 创建所有任务 */

    vTaskDelay(pdMS_TO_TICKS(3000));
    xtp::Led::update_led_pattern(xtp::SystemState::WIFI_CONNECTING);

    vTaskDelay(pdMS_TO_TICKS(5000));
    xtp::Led::update_led_pattern(xtp::SystemState::WIFI_CONNECTED);

    vTaskDelay(pdMS_TO_TICKS(5000));
    xtp::Led::update_led_pattern(xtp::SystemState::NORMAL);

    vTaskDelay(pdMS_TO_TICKS(5000));
    xtp::Led::update_led_pattern(xtp::SystemState::UPDATING);

    vTaskDelay(pdMS_TO_TICKS(5000));
    xtp::Led::update_led_pattern(xtp::SystemState::ERROR);
}
