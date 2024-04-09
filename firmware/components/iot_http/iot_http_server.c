#include <esp_http_server.h>
#include <esp_log.h>
#include <esp_err.h>
#include "string.h"
#include "cJson.h"
#include "iot_nvs.h"
#include "iot_var.h"
#include "iot_spiffs.h"

#define TAG "esp32_server_http"
#define BUFFER_SIZE 1024
#define MIN(a,b) ((a<b)?a:b)

esp_err_t post_handler(httpd_req_t *req)
{
// Read the URI line and get the host
    char *buf;
    size_t buf_len;
    char ssid[32], password[32];
    buf_len = httpd_req_get_hdr_value_len(req, "Host") + 1;
    if (buf_len > 1)
    {
        buf = malloc(buf_len);
        if (httpd_req_get_hdr_value_str(req, "Host", buf, buf_len) == ESP_OK)
        {
            ESP_LOGI(TAG, "Host: %s", buf);
        }
        free(buf);
    }

    // Read the URI line and get the parameters
     buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found URL query: %s", buf);
            if (httpd_query_key_value(buf, "sta_ssid", ssid, sizeof(ssid)) == ESP_OK) {
                ESP_LOGI(TAG, "sta_ssid: %sx", ssid);
            }
            if (httpd_query_key_value(buf, "sta_pass", password, sizeof(password)) == ESP_OK) {
                ESP_LOGI(TAG, "sta_password: %sx", password);
            }
        }
        free(buf);
    }
    strcpy(global.wifi_sta.WIFI_SSID, ssid);
    strcpy(global.wifi_sta.WIFI_PASSWORD, password);

    global.wifi_change = true;
    global.wifi_sta.inited = false;
    while(!global.wifi_sta.inited){
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    FILE *f = fopen("/spiffs/settingSSID_PASS.html", "r");
    if (f == NULL) {
        printf("Failed to open file for reading\n");
        return ESP_FAIL;
    }
    // Tạo một vùng nhớ đệm để lưu trữ dữ liệu từ tệp
    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    // Đọc dữ liệu từ tệp và lưu vào vùng nhớ đệm
    bytes_read = fread(buffer, 1, BUFFER_SIZE, f);
    fclose(f);

    if(global.wifi_sta.wifi_sta_connected){
        strcat(buffer,"<h3>WIFI Connected.</h3>");
        ESP_LOGI(TAG, "rep WIFI connected");
        iot_nvs_write(NVS_KEY_STA_SSID, global.wifi_sta.WIFI_SSID);
        iot_nvs_write(NVS_KEY_STA_PASS, global.wifi_sta.WIFI_PASSWORD);
    }else{
        strcat(buffer,"<h3>WIFI Faile. Please re-enter your information</h3>");
        ESP_LOGI(TAG, "rep WIFI falie");
    }
    httpd_resp_send(req, buffer, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t get_handler(httpd_req_t *req)
{
    FILE *f = fopen("/spiffs/settingSSID_PASS.html", "r");
    if (f == NULL) {
        printf("Failed to open file for reading\n");
        return ESP_FAIL;
    }
    // Tạo một vùng nhớ đệm để lưu trữ dữ liệu từ tệp
    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    // Đọc dữ liệu từ tệp và lưu vào vùng nhớ đệm
    bytes_read = fread(buffer, 1, BUFFER_SIZE, f);
    fclose(f);
    buffer[bytes_read] = '\0';
    httpd_resp_send(req, buffer, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

httpd_handle_t iot_http_server_init()
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;
    if (httpd_start(&server, &config) == ESP_OK)
    {
        // Do something
    }
    httpd_uri_t get_uri = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = get_handler,
        .user_ctx = NULL};

    httpd_register_uri_handler(server, &get_uri);

    httpd_uri_t post_uri = {
        .uri = "/get",
        .method = HTTP_GET,
        .handler = post_handler,
        .user_ctx = NULL};

    httpd_register_uri_handler(server, &post_uri);

    return server;
}

void iot_stop_server(httpd_handle_t server){
    if(server){
        httpd_stop(server);
    }
}