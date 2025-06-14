/*
 * @Author: 小土坡 xiaotupo@163.com
 * @Date: 2025-06-09 00:15:16
 * @LastEditors: 小土坡 xiaotupo@163.com
 * @LastEditTime: 2025-06-10 23:57:06
 * @FilePath: /led_blick/home/zsf/projects/esp32_projects/lcd_demo/main/lcd_demo.c
 * /led_blick/home/zsf/projects/esp32_projects/lcd_demo/main/lcd_demo.c
 * /led_blick/home/zsf/projects/esp32_projects/lcd_demo/main/lcd_demo.c
 * /led_blick/home/zsf/projects/esp32_projects/lcd_demo/main/lcd_demo.c
 * /led_blick/home/zsf/projects/esp32_projects/lcd_demo/main/lcd_demo.c
 * /led_blick/home/zsf/projects/esp32_projects/lcd_demo/main/lcd_demo.c
 * /led_blick/home/zsf/projects/esp32_projects/lcd_demo/main/lcd_demo.c
 * /led_blick/home/zsf/projects/esp32_projects/lcd_demo/main/lcd_demo.c
 * @Description:
 *
 * Copyright (c) 2025 by 小土坡, All Rights Reserved.
 */
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_types.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs.h"
#include "nvs_flash.h"

#define LCD_HOST SPI2_HOST
#define PIN_NUM_MOSI 11
#define PIN_NUM_CLK 12
#define PIN_NUM_DC 40
#define PIN_NUM_CS 39
#define PIN_NUM_RST 38
#define PIN_NUM_BK_LIGHT 41

#define ST7789_Y_OFFSET 26
#define ST7789_X_OFFSET 1

#define LCD_WIDTH 160
#define LCD_HEIGHT 80

#define LED_NUM 1

static const char *TAG = "lcd_example";

/**
 * @brief Fill the entire LCD with a specific color.
 *
 * @param _panel_handle
 * @param color_buf
 * @param color
 * @param size
 */
void lcd_draw_full(esp_lcd_panel_handle_t _panel_handle, uint16_t *color_buf,
                   uint16_t color, size_t size) {
  for (size_t i = 0; i < size; i++) {
    color_buf[i] = color;
  }

  esp_lcd_panel_draw_bitmap(_panel_handle, 0, 0, LCD_WIDTH, LCD_HEIGHT,
                            color_buf);
}

void lcd_task(void *arg) {
  // 这里可以添加 LCD 显示相关的代码
  ESP_LOGI(TAG, "Initialize SPI bus");
  spi_bus_config_t buscfg = {.sclk_io_num = PIN_NUM_CLK,
                             .mosi_io_num = PIN_NUM_MOSI,
                             .miso_io_num = -1,
                             .quadwp_io_num = -1,
                             .quadhd_io_num = -1,
                             .max_transfer_sz = 4096};
  ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO));

  ESP_LOGI(TAG, "Initialize panel IO");
  esp_lcd_panel_io_handle_t io_handle = NULL;
  esp_lcd_panel_io_spi_config_t io_config = {.dc_gpio_num = PIN_NUM_DC,
                                             .cs_gpio_num = PIN_NUM_CS,
                                             .pclk_hz = 40 * 1000 * 1000,
                                             .lcd_cmd_bits = 8,
                                             .lcd_param_bits = 8,
                                             .spi_mode = 0,
                                             .trans_queue_depth = 7};
  ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST,
                                           &io_config, &io_handle));

  ESP_LOGI(TAG, "Initialize ST7789 panel driver");
  esp_lcd_panel_handle_t panel_handle = NULL;
  esp_lcd_panel_dev_config_t panel_config = {.reset_gpio_num = PIN_NUM_RST,
                                             .color_space =
                                                 ESP_LCD_COLOR_SPACE_RGB,
                                             .bits_per_pixel = 16,
                                             .vendor_config = NULL};
  ESP_ERROR_CHECK(
      esp_lcd_new_panel_st7789((io_handle), &panel_config, &panel_handle));

  // Reset & init
  esp_lcd_panel_reset(panel_handle);
  esp_lcd_panel_init(panel_handle);

  // Set rotation if needed
  esp_lcd_panel_swap_xy(panel_handle, true);
  esp_lcd_panel_mirror(panel_handle, true, false);
  esp_lcd_panel_set_gap(panel_handle, ST7789_X_OFFSET, ST7789_Y_OFFSET);
  esp_lcd_panel_disp_on_off(panel_handle, true);

  // Turn on backlight (optional)
  gpio_reset_pin(PIN_NUM_BK_LIGHT);
  gpio_set_direction(PIN_NUM_BK_LIGHT, GPIO_MODE_OUTPUT);
  gpio_set_level(PIN_NUM_BK_LIGHT, 1);

  // Fill screen with red (RGB565: 0xF800)
  ESP_LOGI(TAG, "Filling screen with red");
  uint16_t *color_buf = heap_caps_malloc(
      LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t), MALLOC_CAP_DMA);
  assert(color_buf);

  // ESP_LOGI(TAG, "Deinitialize panel IO");
  // esp_lcd_panel_io_del(io_handle);
  // ESP_LOGI(TAG, "Deinitialize panel driver");
  // esp_lcd_panel_del(panel_handle);
  // ESP_LOGI(TAG, "Deinitialize SPI bus");
  // ESP_ERROR_CHECK(spi_bus_free(LCD_HOST));
  // ESP_LOGI(TAG, "Free color buffer");
  // 释放 color_buf
  // 注意：如果 color_buf 是通过 heap_caps_malloc 分配的，则需要使用
  // heap_caps_free 释放 heap_caps_free(color_buf); 如果 color_buf 是通过 malloc
  // 分配的，则使用 free 释放 这里假设 color_buf 是通过 heap_caps_malloc 分配的
  // heap_caps_free(color_buf);
  // free(color_buf);

  ESP_LOGI(TAG, "Done");
  uint16_t color_change_count = 500;

  lcd_draw_full(panel_handle, color_buf, 0x0000,
                LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t));
  vTaskDelay(pdMS_TO_TICKS(color_change_count));

  while (1) {
    ESP_LOGI(TAG, "lcd_task()...");
    lcd_draw_full(panel_handle, color_buf, 0xfc04,
                  LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t));
    vTaskDelay(pdMS_TO_TICKS(color_change_count));
    lcd_draw_full(panel_handle, color_buf, 0x1d41,
                  LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t));
    vTaskDelay(pdMS_TO_TICKS(color_change_count));
    lcd_draw_full(panel_handle, color_buf, 0x001f,
                  LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t));
    vTaskDelay(pdMS_TO_TICKS(color_change_count));
    lcd_draw_full(panel_handle, color_buf, 0x07e0,
                  LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t));
    vTaskDelay(pdMS_TO_TICKS(color_change_count));
    lcd_draw_full(panel_handle, color_buf, 0xffff,
                  LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t));
    vTaskDelay(pdMS_TO_TICKS(color_change_count));
    lcd_draw_full(panel_handle, color_buf, 0x0000,
                  LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t));
    vTaskDelay(pdMS_TO_TICKS(color_change_count));
  }
}

void app_main(void) {

  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    // 如果 NVS 初始化失败（如版本不兼容），则擦除重试
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  int32_t boot_count = 0;
  nvs_handle_t nvs_handle;

  // 打开命名空间 "storage"
  ret = nvs_open("storage", NVS_READWRITE, &nvs_handle);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "打开 NVS 失败: (%s)", esp_err_to_name(ret));
    return;
  }

  // 读取之前保存的 boot_count（如果有）
  ret = nvs_get_i32(nvs_handle, "boot_count", &boot_count);
  switch (ret) {
  case ESP_OK:
    ESP_LOGI(TAG, "读取成功，boot_count = %lx", boot_count);
    break;
  case ESP_ERR_NVS_NOT_FOUND:
    ESP_LOGI(TAG, "没有找到 boot_count，设置为 0");
    boot_count = 0;
    break;
  default:
    ESP_LOGE(TAG, "读取错误: (%s)", esp_err_to_name(ret));
  }

  // 递增并写入新值
  boot_count++;
  ret = nvs_set_i32(nvs_handle, "boot_count", boot_count);
  ESP_ERROR_CHECK(ret);

  // 提交更改
  ret = nvs_commit(nvs_handle);
  ESP_ERROR_CHECK(ret);

  // 关闭 NVS 句柄
  nvs_close(nvs_handle);

  ESP_LOGI(TAG, "boot_count 已更新为 %lx", boot_count);

  gpio_reset_pin(LED_NUM);
  gpio_set_direction(LED_NUM, GPIO_MODE_OUTPUT);

  xTaskCreatePinnedToCore(lcd_task, "lcd_task", 10240, NULL, 5, NULL, 1);

  while (1) {
    // ESP_LOGI(TAG, "app_main()...");
    // ESP_LOGI(TAG, "LED ON");
    gpio_set_level(LED_NUM, 1);
    vTaskDelay(pdMS_TO_TICKS(1000));
    // ESP_LOGI(TAG, "LED OFF");
    gpio_set_level(LED_NUM, 0);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}