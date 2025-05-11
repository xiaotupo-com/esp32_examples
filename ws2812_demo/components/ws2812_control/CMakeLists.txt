idf_component_register(SRCS "ws2812_control.cpp"
                            "led_strip_rmt_ws2812.c"
                    INCLUDE_DIRS "include"
                    REQUIRES driver esp_timer
                    )

