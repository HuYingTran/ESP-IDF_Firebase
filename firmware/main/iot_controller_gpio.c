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
    for(int i=1; i<4; i++){
        if(global.relay_status[i] == 1){
            gpio_set_level(global.IOT_GPIO_RELAY[i],IOT_HIGH);
        }else{
            gpio_set_level(global.IOT_GPIO_RELAY[i],IOT_LOW);
        }
    }
    if(global.relay_status[0] == 1){
        iot_gpio_pwm(global.pwm_value);
    }
        
}

void iot_control_gpio()
{   while(1)
    {
        iot_wifi_control_led();
        iot_control_relay();
        vTaskDelay(1);
    }
}

void iot_gpio_loop(){
    xTaskCreatePinnedToCore(iot_control_gpio, "iot_control_gpio", 4096, NULL, 19, NULL, 1);
}