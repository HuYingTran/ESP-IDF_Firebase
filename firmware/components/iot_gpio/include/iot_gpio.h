#pragma once
#include "driver/gpio.h"

typedef enum {
    IOT_HIGH = 1,
    IOT_LOW = 0
} iot_gpio_level_t;

typedef enum {
    IOT_GPIO_WIFI = GPIO_NUM_2,
    IOT_GPIO_RELAY_1 = GPIO_NUM_13,
    IOT_GPIO_RELAY_2 = GPIO_NUM_14,
    IOT_GPIO_RELAY_3 = GPIO_NUM_26,
    IOT_GPIO_RELAY_4 = GPIO_NUM_33,
} iot_gpio_t;

void iot_gpio_setting();
void iot_pwm();
void iot_gpio_pwm(int duty_cycle);