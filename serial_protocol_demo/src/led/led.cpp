#include "led.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "Led";

namespace xtp
{

    SystemState current_state = SystemState::NORMAL; /* 当前系统状态 */
    Led::LedPattern current_pattern = {false, 1000, 0};

    /**
     * @brief FreeRTOS Task
     *
     * @param vparams
     */
    void Led::led_task(void *vparams)
    {
        uint32_t led_state = 0;
        gpio_set_direction(led_pin, GPIO_MODE_OUTPUT);
        while (1) {
            if (current_pattern.interval_ms == 0) {
                vTaskDelay(pdMS_TO_TICKS(1000));
                continue; // 常亮模式，不需要闪烁
            }

            current_pattern.on_off = !current_pattern.on_off;
            gpio_set_level(led_pin, current_pattern.on_off ? 1 : 0);
            vTaskDelay(pdMS_TO_TICKS(current_pattern.interval_ms));
        }
    }

    /**
     * @brief 更新 LED 模式
     *
     * @param state
     */
    void Led::update_led_pattern(SystemState state)
    {
        current_state = state;
        switch (state) {
        case SystemState::NORMAL:
            current_pattern = {false, 1000, 0};
            break;
        case SystemState::WIFI_CONNECTING:
            current_pattern = {false, 250, 0};
            break;
        case SystemState::WIFI_CONNECTED:
            current_pattern = {true, 0, 0}; /* 常亮*/
            break;
        case SystemState::ERROR:
            current_pattern = {false, 2000, 0};
            break;
        case SystemState::UPDATING:
            current_pattern = {false, 500, 0};
            break;
        }
    }
} // namespace xtp
