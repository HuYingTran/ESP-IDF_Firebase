#pragma once
#include "driver/gpio.h"

typedef enum {
    IOT_HIGH = 1,
    IOT_LOW = 0
} iot_gpio_level_t;

typedef enum {
    IOT_GPIO_WIFI = GPIO_NUM_2
} iot_gpio_t;

void iot_gpio_setting();