#include "uart_init.h"

namespace xtp::uart
{
    /**
     * allow_pd 是否运行在睡眠模式关闭 uart
     * 0: 睡眠时不关闭
     * 1: 睡眠时关闭
     */
    void uart_init(bool allow_pd)
    {
        uart_config_t uart_config = {
            .baud_rate = UART1_BAUD_RATE,  // 波特率
            .data_bits = UART_DATA_8_BITS, // 数据位
            .parity = UART_PARITY_DISABLE, // 无校验，可选（UART_PARITY_EVEN 和
                                           // UART_PARITY_ODD）
            .stop_bits = UART_STOP_BITS_1,         // 停止位
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE, // 流控制
            .rx_flow_ctrl_thresh = 0,              //
            .source_clk = UART_SCLK_DEFAULT,       // 时钟选择
            .flags = {.allow_pd = allow_pd}};

        ESP_ERROR_CHECK(uart_driver_install(UART1_PORT_NUM, UART1_RX_BUF_SIZE, 0,
                                            0, NULL, 0)); // 安装驱动
        ESP_ERROR_CHECK(
            uart_param_config(UART1_PORT_NUM, &uart_config)); // 配置参数
        ESP_ERROR_CHECK(
            uart_set_loop_back(UART1_PORT_NUM, false)); // 是否开启回环测试
        ESP_ERROR_CHECK(uart_set_pin(UART1_PORT_NUM, UART1_TX_PIN, UART1_RX_PIN,
                                     UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    }
} // namespace xtp::uart
