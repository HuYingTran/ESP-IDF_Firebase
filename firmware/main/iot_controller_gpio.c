#include "iot_gpio.h"
#include "iot_var.h"

static void iot_wifi_control_led(){
    if(global.wifi_sta.wifi_sta_connected){
        gpio_set_level(IOT_GPIO_WIFI, IOT_HIGH);
    }else{
        gpio_set_level(IOT_GPIO_WIFI, IOT_LOW);
    }
}

static void iot_control_relay(){
    for(int i=0; i<4; i++){
        if(global.relay_status[i]){
            gpio_set_level(global.IOT_GPIO_RELAY[i],IOT_HIGH);
        }else{
            gpio_set_level(global.IOT_GPIO_RELAY[i],IOT_LOW);
        }
    }
}

void iot_control_gpio()
{
    iot_wifi_control_led();
    iot_control_relay();
}