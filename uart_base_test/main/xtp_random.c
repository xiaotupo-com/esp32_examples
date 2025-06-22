#include "xtp_random.h"

#include "esp_random.h"

// 生成 [0.0, 1.0) 范围内的随机浮点数
float random_float() {
    uint32_t r = esp_random();
    return (float)r / UINT32_MAX;
}

// 生成 [min, max) 范围内的随机浮点数
float random_float_range(float min, float max) {
    return min + random_float() * (max - min);
}
