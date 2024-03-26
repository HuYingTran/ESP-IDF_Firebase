#include "iot_wifi.h"
#include "iot_var.h"
#include "iot_gpio.h"

void wifi_loop(){
    while (1)
    {
        if(global.wifi_sta.wifi_sta_connected){
            gpio_set_direction(IOT_GPIO_WIFI, IOT_HIGH);
        }
        if(global.wifi_change){
            printf("\nthay đổi ket noi lai");
            global.wifi_change = false;
            iot_wifi_sta_init();
        }
        vTaskDelay(1);
    }
}

void iot_loop() {
    xTaskCreatePinnedToCore(wifi_loop, "wifi_loop",4096, NULL, 19, NULL,1);
}