#include <esp_http_server.h>
#include <esp_log.h>
#include <esp_err.h>
#include "string.h"
#include "cJson.h"
#include "iot_var.h"
#include "iot_spiffs.h"

#define TAG "esp32_server_http"

#define MIN(a,b) ((a<b)?a:b)

esp_err_t post_handler(httpd_req_t *req)
{
    char buf[1024];

    size_t recv_size = MIN(req->content_len, sizeof(buf));
    int ret = httpd_req_recv(req, buf, recv_size);
    if (ret <= 0) {
        if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
            httpd_resp_send_408(req);
        }
        return ESP_FAIL;
    }
    // // Phân tích dữ liệu JSON nhận được
    // cJSON *root = cJSON_Parse(buf);
    // if (!root) {
    //     ESP_LOGE(TAG, "Error parsing JSON data");
    //     httpd_resp_send_500(req);
    //     return ESP_FAIL;
    // }

    // // Lấy giá trị từ JSON
    // cJSON *sta_ssid = cJSON_GetObjectItem(root, "sta_ssid");
    // cJSON *sta_password = cJSON_GetObjectItem(root, "sta_password");

    // // In ra giá trị nhận được
    // ESP_LOGI(TAG, "sta_ssid: %s", sta_ssid->valuestring);
    // ESP_LOGI(TAG, "sta_password: %s", sta_password->valuestring);

    // global.wifi_sta.WIFI_SSID = sta_ssid->valuestring;
    // global.wifi_sta.WIFI_PASSWORD = sta_password->valuestring;
    printf("\nOK\n");
    char *ssid = NULL;
    char *password = NULL;
    char *query = req->uri;
    char *param;
    while((param = strsep(&query, "&")) != NULL){
        char *key = strsep(&param, "=");
        char *value = param;
        if(strcmp(key,"sta_ssid") == 0){
            ssid = value;
        }else if(strcmp(key, "sta_password")){
            password = value;
        }
    }

    // In ra giá trị nhận được
    ESP_LOGI(TAG, "sta_ssid: %s", ssid);
    ESP_LOGI(TAG, "sta_password: %s", password);

    global.wifi_sta.WIFI_SSID = ssid;
    global.wifi_sta.WIFI_PASSWORD = password;

    global.wifi_change = true;

    const char resp[] = "<h1>DONE</h1>";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
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