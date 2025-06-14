#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#include "driver/gpio.h"
#include "lvgl.h"

// 硬件配置
#define LCD_HOST    SPI2_HOST
#define PIN_NUM_MOSI 11
#define PIN_NUM_MISO -1
#define PIN_NUM_CLK  12
#define PIN_NUM_CS   39
#define PIN_NUM_DC   40
#define PIN_NUM_RST  38
#define PIN_NUM_BCKL 41

// 显示分辨率
#define LCD_H_RES   160
#define LCD_V_RES   80

// LVGL 缓冲区大小（根据 RAM 调整）
#define LVGL_BUFFER_SIZE (LCD_H_RES * 10)

// 互斥锁，保护 LVGL 访问
static SemaphoreHandle_t lvgl_mutex = NULL;
static esp_lcd_panel_handle_t panel_handle = NULL;
static lv_disp_t *disp = NULL;

// LVGL 显示缓冲区
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[LVGL_BUFFER_SIZE];
static lv_color_t buf2[LVGL_BUFFER_SIZE];

// LVGL 定时器回调
static void lvgl_tick_task(void *arg) {
    (void)arg;
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1));
        lv_tick_inc(1);
    }
}

// LVGL 显示刷新回调
static void disp_flush(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    esp_lcd_panel_draw_bitmap(panel_handle, 
                              area->x1, area->y1, 
                              area->x2, area->y2, 
                              color_p);

    lv_disp_flush_ready(drv);
}

// 初始化 LCD 和 LVGL
void lcd_lvgl_init(void) {
    // 1. 初始化 SPI 总线
    spi_bus_config_t buscfg = {
        .sclk_io_num = PIN_NUM_CLK,
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = LCD_H_RES * 80 * 2,
    };
    spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO);

    // 2. 配置 LCD 接口
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = PIN_NUM_DC,
        .cs_gpio_num = PIN_NUM_CS,
        .pclk_hz = 60 * 1000 * 1000,  // 60MHz
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
        .spi_mode = 0,
        .trans_queue_depth = 10,
    };
    esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &io_handle);

    // 3. 配置 LCD 面板参数（ST7789 示例）
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = PIN_NUM_RST,
        .rgb_endian = LCD_RGB_ENDIAN_RGB,
        .bits_per_pixel = 16,  // RGB565
    };
    esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle);

    // 4. 重置并初始化面板
    esp_lcd_panel_reset(panel_handle);
    esp_lcd_panel_init(panel_handle);

    // 5. 设置显示方向
    esp_lcd_panel_swap_xy(panel_handle, 1);
    esp_lcd_panel_mirror(panel_handle, 0, 1);

    // 6. 打开背光
    gpio_config_t bkl_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << PIN_NUM_BCKL,
    };
    gpio_config(&bkl_gpio_config);
    gpio_set_level(PIN_NUM_BCKL, 1);

    // 7. 初始化 LVGL
    lv_init();

    // 8. 配置 LVGL 显示缓冲区
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, LVGL_BUFFER_SIZE);

    // 9. 注册显示驱动
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = LCD_H_RES;
    disp_drv.ver_res = LCD_V_RES;
    disp_drv.flush_cb = disp_flush;
    disp_drv.draw_buf = &draw_buf;
    disp = lv_disp_drv_register(&disp_drv);

    // 10. 创建 LVGL 互斥锁
    lvgl_mutex = xSemaphoreCreateMutex();

    // 11. 创建 LVGL 定时器任务
    xTaskCreate(lvgl_tick_task, "lvgl_tick", 1024 * 2, NULL, 10, NULL);
}

// LVGL 任务处理
void lvgl_task(void *arg) {
    while (1) {
        xSemaphoreTake(lvgl_mutex, portMAX_DELAY);
        lv_task_handler();
        xSemaphoreGive(lvgl_mutex);
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

// 创建 LVGL UI 界面
void create_ui(void) {
    // 创建样式
    static lv_style_t style_btn;
    lv_style_init(&style_btn);
    lv_style_set_radius(&style_btn, 10);
    lv_style_set_bg_color(&style_btn, lv_color_make(0x33, 0x66, 0x99));
    lv_style_set_bg_opa(&style_btn, LV_OPA_COVER);
    lv_style_set_border_color(&style_btn, lv_color_make(0x00, 0x00, 0x00));
    lv_style_set_border_width(&style_btn, 2);
    lv_style_set_text_color(&style_btn, lv_color_make(0xFF, 0xFF, 0xFF));

    // 创建按钮
    lv_obj_t *btn = lv_btn_create(lv_scr_act());
    lv_obj_add_style(btn, &style_btn, 0);
    lv_obj_set_size(btn, 120, 50);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);

    // 创建标签
    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, "Click Me");
    lv_obj_center(label);

    // 添加点击事件
    lv_obj_add_event_cb(btn, [](lv_event_t *e) {
        static uint32_t cnt = 0;
        cnt++;
        char buf[16];
        sprintf(buf, "Clicked: %d", cnt);
        lv_label_set_text(lv_obj_get_child(lv_event_get_target(e), 0), buf);
        lv_obj_center(lv_obj_get_child(lv_event_get_target(e), 0));
    }, LV_EVENT_CLICKED, NULL);

    // 创建标题
    lv_obj_t *title = lv_label_create(lv_scr_act());
    lv_label_set_text(title, "ESP32 + LVGL Demo");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
}

void app_main(void) {
    // 初始化 LCD 和 LVGL
    lcd_lvgl_init();
    
    // 创建 UI
    create_ui();
    
    // 创建 LVGL 任务
    xTaskCreate(lvgl_task, "lvgl_task", 1024 * 4, NULL, 5, NULL);
}