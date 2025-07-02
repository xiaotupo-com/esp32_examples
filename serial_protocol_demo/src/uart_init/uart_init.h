#ifndef __UART_INIT_H
#define __UART_INIT_H

#include "driver/gpio.h"
#include "driver/uart.h"

namespace xtp::uart
{
    inline constexpr uart_port_t UART1_PORT_NUM = UART_NUM_1;
    inline constexpr int UART1_RX_PIN = GPIO_NUM_10;
    inline constexpr int UART1_TX_PIN = GPIO_NUM_9;
    inline constexpr int UART1_BAUD_RATE = 115200;
    inline constexpr int UART1_RX_BUF_SIZE = 1024;

    void uart_init(bool allow_pd);

} // namespace xtp::uart

#endif
