#include "iot_http.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static int counter_100s = 0;

void iot_http_thingspeak()
{
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    //https://api.thingspeak.com/channels/2491699/feeds.json?api_key=J026IRE0SBI4XP8G&results=2
    char domain[64] = "api.thingspeak.com";
    char path[32] = "/channels/2491699/feeds.json";
    char query_params[256] = "api_key=J026IRE0SBI4XP8G&results=2";
    iot_http_client_get(domain, path, query_params);
}

// xTaskCreatePinnedToCore
void iot_http_loop()
{
    xTaskCreatePinnedToCore(&iot_http_thingspeak, "iot_http_thingspeak", 4096, NULL, 19, NULL, 1);
}