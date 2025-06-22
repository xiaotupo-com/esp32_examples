#include "button_gpio.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "iot_button.h"
#include <stdio.h>

static const char *TAG = "blink";

TaskHandle_t led_handle = NULL;

// GPIO 定义
#define LED_PIN_NUM GPIO_NUM_1
#define Key_PIN_NUM GPIO_NUM_0

// 单击回调函数
static void button_single_click_cb(void *arg, void *data)
{
    iot_button_print_event((button_handle_t)arg);
    printf("Button single click detected\n");

    // 改变 led 任务的挂起与恢复挂起
    if (led_handle != NULL) {
        if(eTaskGetState(led_handle) == eSuspended) { // 挂起态
            printf("LED 任务已恢复运行!\n");
            vTaskResume(led_handle); // 恢复任务
        } else {
            printf("LED 任务已被挂起!\n");
            gpio_set_level(LED_PIN_NUM, 1); // 熄灭 LED
            vTaskSuspend(led_handle); // 挂起任务
        }
    }
}

// 长按回调
static void on_button_long_press(void *arg, void *data)
{
    iot_button_print_event((button_handle_t)arg);
    printf("Button long pressed\n");
}

void xtp_button_init(uint32_t button_num)
{
    button_config_t btn_cfg = {0};
    button_gpio_config_t gpio_cfg = {
        .gpio_num = button_num,
        .active_level = 0,
        .enable_power_save = true, // 兼容省点模式
        .disable_pull = false      // 使用内部上拉电阻
    };

    button_handle_t btn;

    esp_err_t ret = iot_button_new_gpio_device(&btn_cfg, &gpio_cfg, &btn);
    assert(ret == ESP_OK);

    ret = iot_button_register_cb(btn, BUTTON_SINGLE_CLICK, NULL, button_single_click_cb, NULL);
    ret |= iot_button_register_cb(btn, BUTTON_LONG_PRESS_START, NULL, on_button_long_press, NULL);

    assert(ret == ESP_OK);
}

// 创建一个LED任务
void led_task(void *vParam)
{
    // 设置日志级别
    esp_log_level_set(TAG, ESP_LOG_WARN);

    gpio_set_direction(LED_PIN_NUM, GPIO_MODE_OUTPUT);
    uint8_t led_state = 0;

    while (1) {
        ESP_LOGI(TAG, "led task...");
        gpio_set_level(LED_PIN_NUM, led_state = !led_state);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "初始化完成");
    xTaskCreatePinnedToCore(led_task, "led_task", 2048, NULL, 5, &led_handle, 1);

    xtp_button_init(Key_PIN_NUM);
    while (1) {
        ESP_LOGI(TAG, "app_main loop");
        vTaskDelay(pdMS_TO_TICKS(1500));
    }
}
