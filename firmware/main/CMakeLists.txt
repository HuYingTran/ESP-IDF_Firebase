idf_component_register(SRCS "main.c" "iot_controller.c" "iot_controller_wifi.c" "iot_controller_gpio.c" "iot_controller_http.c"
                    INCLUDE_DIRS ".")
spiffs_create_partition_image(explorer ../../data FLASH_IN_PROJECT)