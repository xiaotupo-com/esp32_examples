set(SOURCE_FILES
    app_main.cpp
    ./serial_protocol/serial_protocol.cpp
    ./led/led.cpp
    ./uart_init/uart_init.cpp
)

set(INCLUDE_PATHS
    .
    ./serial_protocol
    ./led
    ./uart_init
)

idf_component_register(SRCS ${SOURCE_FILES}
    INCLUDE_DIRS ${INCLUDE_PATHS}
    REQUIRES driver
    )

target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++17)