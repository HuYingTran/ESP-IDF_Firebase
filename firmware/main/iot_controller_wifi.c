#include "iot_wifi.h"
#include "iot_var.h"
#include "iot_gpio.h"
#include "iot_nvs.h"

void wifi_loop(){
    while (1)
    {
        if(global.wifi_sta.wifi_sta_connected){
            gpio_set_level(IOT_GPIO_WIFI, IOT_HIGH);
            iot_nvs_write(NVS_KEY_STA_SSID, global.wifi_sta.WIFI_SSID);
            iot_nvs_write(NVS_KEY_STA_PASS, global.wifi_sta.WIFI_PASSWORD);
        }
        if(global.wifi_change){
            global.wifi_change = false;
            iot_wifi_sta_init();
        }
        vTaskDelay(1);
    }
}

void iot_loop() {
    xTaskCreatePinnedToCore(wifi_loop, "wifi_loop",4096, NULL, 19, NULL,1);
}