/**
 * @file serial_protocol.h
 * @author xtp (xiaotupo@163.com)
 * @brief uart 通信协议例子
 * @version V0.1
 * @date 2025-06-26
 *
 * @copyright Copyright (c) 2025
 * ***********************************************************
 * @brief
 * 串口通信分为接收和发送两个模块
 * 帧定义：
 * 1. 帧头 1Byte
 * 2. 命令 1Byte
 * 3. 数据 8Byte
 * 4. 校验 1Byte
 * 共 11 字节
 */
#ifndef __SERIAL_PROTOCOL_H
#define __SERIAL_PROTOCOL_H
#include "driver/gpio.h"
#include <stdint.h>

namespace xtp::uart_protocol
{
    constexpr gpio_num_t rx_led = GPIO_NUM_2;
    constexpr uint8_t FRAME_HEAD = 0x7e; /* 帧头 */
    constexpr uint8_t FRAME_LENGTH = 11;
    constexpr uint8_t DATA_LENGTH = 8;

    enum class Cmd
    {
        LED_STATE, /* 用于控制 LED 两灭的命令 */
    };

    struct Packet
    {
        Cmd cmd; /* 数据表代表的命令功能，如打开 LED 等 */
        uint8_t data[DATA_LENGTH]; /* 数据 */
    };

    bool decode(uint8_t byte,
                Packet *pkt); /* 解码串口发送的帧到 Packet 结构中 */
    uint8_t calc_checksum(void);
    void handle_cmd(Cmd cmd);
    void run_led(uint8_t value);

    extern "C" {
    void rxTask(void *pvParams);
    }
} // namespace xtp::uart_protocol

#endif
