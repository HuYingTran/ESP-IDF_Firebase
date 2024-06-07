#include "iot_wifi.h"
#include "iot_var.h"
#include "iot_gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "iot_gpio.h"
#include "iot_realtime.h"

#define TAG "IOT_CONTROL_WIFI"


void wifi_loop(){
    while (1)
    {
        if(global.wifi_change){
            global.wifi_change = false;
            ESP_LOGI(TAG, "ssid_pass_wifi_changed");
            iot_wifi_sta_init();
        }
        vTaskDelay(1);
    }
}

void iot_wifi_loop() {
    xTaskCreatePinnedToCore(wifi_loop, "wifi_loop",4096, NULL, 19, NULL,1);
}