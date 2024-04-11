#include "iot_http.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "iot_var.h"

static int counter_100s = 0;

void iot_http_thingspeak()
{
    while(1) {
        vTaskDelay(20*pdMS_TO_TICKS(1000));
        //https://api.thingspeak.com/channels/2491699/feeds.json?api_key=J026IRE0SBI4XP8G&results=2
        char domain[64] = "api.thingspeak.com";
        char path[64] = "/channels/2491699/feeds/last.json";
        char query_params[256] = "api_key=J026IRE0SBI4XP8G&results=2";
        printf("\nrequest ThingSpeak:");
        iot_http_client_get(domain, path, query_params);
    }
    vTaskDelete(NULL);
}

// xTaskCreatePinnedToCore
void iot_http_loop()
{
    xTaskCreatePinnedToCore(iot_http_thingspeak, "iot_http_thingspeak", 4096, NULL, 19, NULL, 1);
}