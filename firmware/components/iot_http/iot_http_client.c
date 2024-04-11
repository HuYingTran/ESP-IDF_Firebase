#include <stdio.h>
#include <string.h>
#include "cJSON.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_http_client.h"
#include "iot_var.h"

static const char *TAG = "HTTP_CLIENT";

// #define URL "http://api.thingspeak.com/channels/2491699/feeds/last.json?api_key=J026IRE0SBI4XP8G&results=2"

typedef struct {
    char protocol[6];
    char domain[64];
    char path[64];
    char query_params[256];
}url_http_get;

static url_http_get current_url_http_get;

static void process_json_thingspeak(char *data){
    cJSON *root = cJSON_Parse(data);
    if(root == NULL){
        printf("ERROR parsing JSON");
        return;
    }
    // Access fields
    cJSON *field1 = cJSON_GetObjectItem(root, "field1");
    cJSON *field2 = cJSON_GetObjectItem(root, "field2");
    cJSON *field3 = cJSON_GetObjectItem(root, "field3");
    cJSON *field4 = cJSON_GetObjectItem(root, "field4");

    // Extract values from fields
    global.relay_status[0] = strcmp(field1->valuestring,"1") == 0 ? 1:0;
    global.relay_status[1] = strcmp(field2->valuestring,"1") == 0 ? 1:0;
    global.relay_status[2] = strcmp(field3->valuestring,"1") == 0 ? 1:0;
    global.relay_status[3] = strcmp(field4->valuestring,"1") == 0 ? 1:0;
    printf("\nrelay %d",global.relay_status[0]);
}

// Xử lý các sự kiện http của client
esp_err_t _http_event_handle(esp_http_client_event_t *evt)
{
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGI(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            printf("%.*s", evt->data_len, (char*)evt->data);
            process_json_thingspeak(evt->data);
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            break;
        default:
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
    printf("\nurl:%s\n",url);

    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_GET,
        .cert_pem = NULL,
        .event_handler = _http_event_handle,
        .timeout_ms = 8000
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
    strcpy(current_url_http_get.protocol, "http");
    strcpy(current_url_http_get.domain, domain);
    strcpy(current_url_http_get.path, path);
    strcpy(current_url_http_get.query_params, query_params);
    xTaskCreate(iot_http_get_task, "http_get_task", 4096, NULL, 5, NULL);
}