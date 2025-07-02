#include "driver/gpio.h"
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "xtp_random.h"

#define UART1_PORT_NUM UART_NUM_1
#define UART1_RX_PIN GPIO_NUM_10
#define UART1_TX_PIN GPIO_NUM_9
#define UART1_BAUD_RATE 115200
#define UART1_RX_BUF_SIZE 1024

static const char *TAG = "UART1";

// 任务句柄定义
TaskHandle_t uart_tx_task_handle = NULL;
TaskHandle_t uart_rx_task_handle = NULL;

/**
 * allow_pd 是否运行在睡眠模式关闭 uart
 * 0: 睡眠时不关闭
 * 1: 睡眠时关闭
 */
static void uart_init(bool allow_pd)
{
    uart_config_t uart_config = {
        .baud_rate = UART1_BAUD_RATE,  // 波特率
        .data_bits = UART_DATA_8_BITS, // 数据位
        .parity = UART_PARITY_DISABLE, // 无校验，可选（UART_PARITY_EVEN 和 UART_PARITY_ODD）
        .stop_bits = UART_STOP_BITS_1,         // 停止位
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE, // 流控制
        .rx_flow_ctrl_thresh = 0,              //
        .source_clk = UART_SCLK_DEFAULT,       // 时钟选择
        .flags.allow_pd = allow_pd};

    ESP_ERROR_CHECK(
        uart_driver_install(UART1_PORT_NUM, UART1_RX_BUF_SIZE, 0, 0, NULL, 0)); // 安装驱动
    ESP_ERROR_CHECK(uart_param_config(UART1_PORT_NUM, &uart_config));           // 配置参数
    ESP_ERROR_CHECK(uart_set_loop_back(UART1_PORT_NUM, false)); // 是否开启回环测试
    ESP_ERROR_CHECK(uart_set_pin(UART1_PORT_NUM, UART1_TX_PIN, UART1_RX_PIN, UART_PIN_NO_CHANGE,
                                 UART_PIN_NO_CHANGE));
}

// 发送任务
void uart_tx_task(void *vparam)
{
    char tx_buf[128];
    uint32_t count = 0;
    float tmp = 0.00f;
    int len;
    while(1) {
        tmp = random_float_range(0.00f, 100.0f);
        sprintf(tx_buf, "Count: %ld，Tmp: %.2f\n", count++, tmp);
        len = uart_write_bytes(UART1_PORT_NUM, tx_buf, strlen(tx_buf));
        if (len < 0) {
            ESP_LOGE(TAG, "串口写失败");
        } else {
            ESP_LOGI(TAG, "串口写成功");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// 接收任务
void uart_rx_task(void *vparam)
{
    uint8_t rx_buf[128];
    int len;
    while(1) {
        len = uart_read_bytes(UART1_PORT_NUM, rx_buf, sizeof(rx_buf), pdMS_TO_TICKS(100));
        if (len > 0) {
            printf("Received (%d bytes): %s\n", len, rx_buf);
        }
    }
}

void xtp_create_task()
{ 
    xTaskCreatePinnedToCore(uart_tx_task, "uart_tx_task", 2048, NULL, 5, &uart_tx_task_handle, 1);
    xTaskCreatePinnedToCore(uart_rx_task, "uart_rx_task", 2048, NULL, 5, &uart_rx_task_handle, 1);
}


void app_main(void)
{
    esp_log_level_set(TAG, ESP_LOG_WARN); // 设置日志等级
    xtp_create_task(); // 创建各种任务
    
    uart_init(0);
}

