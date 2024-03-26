#pragma once
#include "driver/gpio.h"

typedef enum {
    IOT_HIGH = 1,
    IOT_LOW = 0
} iot_gpio_level_t;

typedef enum {
    IOT_GPIO_WIFI = GPIO_NUM_25
} iot_gpio_t;