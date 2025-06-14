idf_component_register(SRCS "ws2812_task.cpp" "cpp_demo.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES ws2812_control)
target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++14)