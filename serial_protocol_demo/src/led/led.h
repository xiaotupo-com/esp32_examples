/**
 * @file led.h
 * @author xtp (xiaotupo@163.com)
 * @brief
 * @version V0.1
 * @date 2025-06-25
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef __LED_H
#define __LED_H

#include "driver/gpio.h"

namespace xtp
{
    /**
     * @brief 系统状态枚举类
     *
     */
    enum class SystemState { NORMAL, WIFI_CONNECTING, WIFI_CONNECTED, ERROR, UPDATING };

    /**
     * @brief LED Class
     *
     */
    class Led
    {
        public:
        static constexpr gpio_num_t led_pin = GPIO_NUM_1; /* GPIO Pin 定义 */
        static void led_task(void *vparams);
        static void update_led_pattern(SystemState state);

        public:
        struct LedPattern {
            bool on_off;     /* 当前亮灭状态 */
            int interval_ms; /* 切换间隔 */
            int count;       /* 用于计数（例如闪几下） */
        };
    };
} // namespace xtp
#endif
