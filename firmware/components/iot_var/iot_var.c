#include "iot_var.h"

void iot_var_init(){
    global.IOT_GPIO_RELAY[0] = IOT_GPIO_RELAY_1;
    global.IOT_GPIO_RELAY[1] = IOT_GPIO_RELAY_2;
    global.IOT_GPIO_RELAY[2] = IOT_GPIO_RELAY_3;
    global.IOT_GPIO_RELAY[3] = IOT_GPIO_RELAY_4;
}