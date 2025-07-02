#include "serial_protocol.h"

#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "uart_init.h"
#include <cstring>

namespace xtp::uart_protocol
{
    Packet recv_pkt;                /* 接收包 */
    uint8_t recv_buf[FRAME_LENGTH]; /* 接收缓冲 */
    int recv_index = 0;             /* 接收索引 */

    /**
     * @brief 计算校验和
     *
     * @param buf
     * @param len
     * @return uint8_t
     */
    uint8_t calc_checksum(void)
    {
        uint8_t sum = 0;
        sum ^= static_cast<uint8_t>(recv_pkt.cmd);
        for (int i = 0; i < 8; i++) { sum ^= recv_pkt.data[i]; }
        return sum;
    }

    /**
     * @brief uart 接收任务
     *
     * @param pvParams
     */
    void rxTask(void *pvParams)
    {

        gpio_set_direction(rx_led, GPIO_MODE_OUTPUT);
        uint8_t byte;

        while (1)
        {
            if (uart_read_bytes(xtp::uart::UART1_PORT_NUM, &byte, 1,
                                pdMS_TO_TICKS(100)) > 0)
            {
                if (decode(byte, &recv_pkt))
                {
                    /* 校验是否正确(帧的最后一个字节为校验值) */
                    if (recv_buf[FRAME_LENGTH - 1] == calc_checksum())
                    {
                        printf("校验成功!\n");
                    }
                    else
                    {
                        printf("校验失败!\n");
                    }

                    printf("Got Frame! CMD: 0x%02X\nData: ",
                           static_cast<uint8_t>(recv_pkt.cmd));

                    for (int i = 0; i < 8; ++i)
                    {
                        printf("%02X ", recv_pkt.data[i]);
                    }

                    printf("\n");
                    printf("sum: 0x%02X\n", recv_buf[FRAME_LENGTH - 1]);

                    handle_cmd(recv_pkt.cmd);
                }
            }

            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }

    /**
     * @brief 解码串口发送的帧到 Packet 结构中
     *
     * @param byte
     * @param pkt
     * @return true 代表一帧解码完成
     * @return false 代表一帧还没有完整解码
     */
    bool decode(uint8_t byte, Packet *pkt)
    {
        if (recv_index == 0)
        {
            if (byte != FRAME_HEAD)
            {
                return false;
            }
        }

        /* 把 byte 送到 recv_buf 中 */
        recv_buf[recv_index++] = byte;

        /* 代表把一帧的完整内容已经送到 recv_buf 了 */
        if (recv_index == FRAME_LENGTH)
        {
            if (recv_buf[0] == FRAME_HEAD)
            {
                pkt->cmd =
                    static_cast<Cmd>(recv_buf[1]); /* 将功能码转为 Cmd 枚举类 */
                memcpy(pkt->data, &recv_buf[2], 8);
                recv_index = 0;
                return true;
            }
            else
            {
                recv_index = 0;
            }
        }
        return false;
    }

    /**
     * @brief 根据收到的不同命令执行相应的功能
     *
     * @param cmd
     */
    void handle_cmd(Cmd cmd)
    {
        switch (cmd)
        {
        case Cmd::LED_STATE:
            run_led(recv_pkt.data[0]);
            break;
        default:
            break;
        }
    }

    /**
     * @brief 根据帧中的数据数组第一个元素的值开控制 LED的 亮灭
     *
     * @param value
     * 0x01: led 点亮
     * 其它: led 熄灭
     */
    void run_led(uint8_t value)
    {
        /* LED 控制 */
        if (value == 0x01)
        {
            gpio_set_level(rx_led, 0);
        }
        else
        {
            gpio_set_level(rx_led, 1);
        }
    }
} // namespace xtp::uart_protocol
