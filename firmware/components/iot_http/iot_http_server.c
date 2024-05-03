#include <esp_http_server.h>
#include <esp_log.h>
#include <esp_err.h>
#include "string.h"
#include "cJson.h"
#include "iot_nvs.h"
#include "iot_var.h"
#include "iot_http.h"
#include "iot_spiffs.h"

#define TAG "esp32_server_http"

#define MIN(a,b) ((a<b)?a:b)

static void iot_http_status_relay(char* buf){
    iot_spiffs_readfile("control.html", buf);
    // button1.style.backgroundColor = 'red';
    for(int i = 1; i < 5; i++){
        char id[2];
        sprintf(id, "%d", i);
        strcat(buf,"button"); 
        strcat(buf,id);
        strcat(buf,".style.backgroundColor = ");
        if(global.relay_status[i-1]){
            strcat(buf,"'greenyellow';");
        }else{
            strcat(buf,"'red';");
        }
    }
    strcat(buf,"</script></body></html>");
}

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

    char buffer[BUFFER_SIZE];
    iot_spiffs_readfile("settingSSID_PASS.html", buffer);

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

void iot_http_send_thingspeak()
{
        char domain[64] = "api.thingspeak.com";
        char path[64] = "/update";
        char query_params[256] = "api_key=HNKTTFIYBZAK62LD";
        strcat(query_params,"&field1=");
        strcat(query_params,global.relay_status[0] == 0 ? "0":"1");
        strcat(query_params,"&field2=");
        strcat(query_params,global.relay_status[1] == 0 ? "0":"1"); 
        strcat(query_params,"&field3=");
        strcat(query_params,global.relay_status[2] == 0 ? "0":"1");
        strcat(query_params,"&field4=");
        strcat(query_params,global.relay_status[3] == 0 ? "0":"1");
        
        printf("\nsend ThingSpeak:");
        iot_http_client_get(domain, path, query_params);
}

esp_err_t post_control_handler(httpd_req_t *req)
{
    char buf[100];
    int buf_len = sizeof(buf);
    int ret;

    /* Read URL query string length and allocate memory for length + 1,
     * extra byte for null termination */
    if (httpd_req_get_url_query_len(req) > 0) {
        /* Get the length of the URL query string */
        if ((ret = httpd_req_get_url_query_str(req, buf, buf_len)) == ESP_OK) {
            /* URL query string is stored in buf, so now you can extract
             * the parameter you need. In this case, we'll just print it */
            ESP_LOGI(TAG, "Found URL query => %s", buf);
            /* Respond with the value passed to the 'button' parameter */
            switch (buf[7])
            {
                case '1':
                    global.relay_status[0] = global.relay_status[0] == 0 ? 1:0;
                    break;
                case '2':
                    global.relay_status[1] = global.relay_status[1] == 0 ? 1:0;
                    break;
                case '3':
                    global.relay_status[2] = global.relay_status[2] == 0 ? 1:0;
                    break;
                case '4':
                    global.relay_status[3] = global.relay_status[3] == 0 ? 1:0;
                    break;
                default:
                    break;
            } 
        } else {
            ESP_LOGE(TAG, "Buffer too small, increase buf_len to fit query string");
        }
    } else {
        ESP_LOGE(TAG, "No URL query string");
    }

    iot_http_send_thingspeak();

    char buffer[BUFFER_SIZE];
    iot_http_status_relay(buffer);

    httpd_resp_send(req, buffer, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t get_handler(httpd_req_t *req)
{
    char buffer[BUFFER_SIZE];
    iot_spiffs_readfile("settingSSID_PASS.html", buffer);

    httpd_resp_send(req, buffer, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t control_handler(httpd_req_t *req)
{
    char buffer[BUFFER_SIZE];
    iot_http_status_relay(buffer);

    httpd_resp_send(req, buffer, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

httpd_handle_t iot_http_server_init()
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    // config.stack_size = 2048;
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

    httpd_uri_t control_uri = {
        .uri = "/control",
        .method = HTTP_GET,
        .handler = control_handler,
        .user_ctx = NULL};

    httpd_register_uri_handler(server, &control_uri);

    httpd_uri_t post_control_uri = {
        .uri = "/get_control",
        .method = HTTP_GET,
        .handler = post_control_handler,
        .user_ctx = NULL};

    httpd_register_uri_handler(server, &post_control_uri);

    return server;
}

void iot_stop_server(httpd_handle_t server){
    if(server){
        httpd_stop(server);
    }
}