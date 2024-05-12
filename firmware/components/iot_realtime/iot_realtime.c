#include "time.h" // struct tm
#include <inttypes.h>
#include <esp_sntp.h>
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "SNTP"

static void time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(TAG, "Time synced");
}

static void initialize_sntp(void)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(7, "pool.ntp.org");
    sntp_set_time_sync_notification_cb(time_sync_notification_cb);
    sntp_init();
}

void obtain_time(void)
{
    int retry = 0;
    const int retry_count = 20;

    initialize_sntp();
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count)
    {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%i/%i)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        if(retry == retry_count){
            ESP_LOGE(TAG, "sntp init fail");
        }
    }
}

void sntp_init_loop()
{
        time_t now;
        struct tm timeinfo;
        char strftime_buf[64];

        obtain_time();
        
        time(&now);

        setenv("TZ", "EST5EDT,M3.2.0/2,M11.1.0", 1);
        tzset();
        localtime_r(&now, &timeinfo);
        strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
        ESP_LOGI(TAG, "The current date/time in New York is: %s", strftime_buf);
    vTaskDelete(NULL);
}

void iot_sntp_init()
{
    xTaskCreatePinnedToCore(sntp_init_loop, "stnp_init", 4096, NULL, 19, NULL, 1);
}
