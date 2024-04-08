#include <stdio.h>
#include <string.h>
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_http_client.h"

static const char *TAG = "HTTP_CLIENT";

typedef struct {
    char protocol[6];
    char domain[64];
    char path[32];
    char query_params[256];
}url_http_get;

static url_http_get current_url_http_get;

// Xử lý các sự kiện http của client
esp_err_t iot_http_event_handler(esp_http_client_event_t *evt) {
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_DISCONNECTED");
            break;
    }
    return ESP_OK;
}

void iot_http_get_task(void *pvParameters) {
    char url[364];
    strcpy(url, current_url_http_get.protocol);
    strcat(url, "://");
    strcat(url, current_url_http_get.domain);
    strcat(url, current_url_http_get.path);
    strcat(url, "?");
    strcat(url, current_url_http_get.query_params);

    esp_http_client_config_t config = {
        .url = url,
        .event_handler = iot_http_event_handler,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %d",
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
    }
    esp_http_client_cleanup(client);
    vTaskDelete(NULL);
}

void iot_http_client_get(char *domain, char *path, char *query_params) {
    printf("\ndomain %s",domain);
    strcpy(current_url_http_get.domain, domain);
    strcpy(current_url_http_get.path, path);
    strcpy(current_url_http_get.query_params, query_params);
    xTaskCreate(iot_http_get_task, "http_get_task", 4096, NULL, 5, NULL);
}
