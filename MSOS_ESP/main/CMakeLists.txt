idf_component_register(SRCS "hello_world_main.c" "iic.c" "oled.c"
                       PRIV_REQUIRES spi_flash esp_driver_i2c log
                       INCLUDE_DIRS ".")
