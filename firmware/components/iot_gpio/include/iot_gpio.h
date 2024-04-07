#pragma once
#include "driver/gpio.h"

typedef enum {
    IOT_HIGH = 1,
    IOT_LOW = 0
} iot_gpio_level_t;

typedef enum {
    IOT_GPIO_WIFI = GPIO_NUM_2,
    IOT_GPIO_RELAY_1 = GPIO_NUM_11,
    IOT_GPIO_RELAY_2 = GPIO_NUM_12,
    IOT_GPIO_RELAY_3 = GPIO_NUM_13,
    IOT_GPIO_RELAY_4 = GPIO_NUM_14,
} iot_gpio_t;

void iot_gpio_setting();