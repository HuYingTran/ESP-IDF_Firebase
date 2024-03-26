#include "driver/gpio.h"
#include "iot_gpio.h"

void iot_gpio_setting()
{
    gpio_pad_select_gpio(IOT_GPIO_WIFI);
    gpio_set_direction(IOT_GPIO_WIFI, GPIO_MODE_OUTPUT);
    // gpio_set_pull_mode(IOT_GPIO_WIFI, GPIO_PULLDOWN_ONLY);
}