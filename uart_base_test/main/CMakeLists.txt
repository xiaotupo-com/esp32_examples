set(SOURCE_FILES 
    "uart_base_test.c"
    "xtp_random.c"
)

idf_component_register(SRCS ${SOURCE_FILES}
                    INCLUDE_DIRS ".")
