#include "driver/gpio.h"
#include "iot_gpio.h"
#include "iot_var.h"

void iot_gpio_setting()
{
    gpio_pad_select_gpio(IOT_GPIO_WIFI);
    gpio_set_direction(IOT_GPIO_WIFI, GPIO_MODE_OUTPUT);
    // gpio_set_pull_mode(IOT_GPIO_WIFI, GPIO_PULLDOWN_ONLY);

    // Setup gpio relay mode output
    for(int i = 0; i<4; i++)
    {
        gpio_set_direction(global.IOT_GPIO_RELAY[i], GPIO_MODE_OUTPUT);
    }
    
}